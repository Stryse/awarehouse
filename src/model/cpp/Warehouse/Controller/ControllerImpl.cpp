#include "ControllerImpl.h"
#include "AMotor.h"
#include "IMoveMechanism.h"
#include "NetworkMessage.h"
#include "PathFinder.h"
#include "SchedulerImpl.h"
#include "Task.h"
#include <iostream>
#include <limits>

ControllerImpl::ControllerImpl(PathFinder *pathFinder)
    : pathFinder(pathFinder),
      MControlGranted(std::make_shared<AgentControlGrantedMessage>(0x2)),
      MControlGiveUp(std::make_shared<AgentControlGiveUpMessage>(0x2)),
      MMoveAgentUp(std::make_shared<MoveAgentMessage>(DirectionVector<>::UP(), 0x2)),
      MMoveAgentDown(std::make_shared<MoveAgentMessage>(DirectionVector<>::DOWN(), 0x2)),
      MMoveAgentLeft(std::make_shared<MoveAgentMessage>(DirectionVector<>::LEFT(), 0x2)),
      MMoveAgentRight(std::make_shared<MoveAgentMessage>(DirectionVector<>::RIGHT(), 0x2)),
      MPickupPod(std::make_shared<PickupPodMessage>(0x2)),
      MPutdownPod(std::make_shared<PutDownPodMessage>(0x2)),
      MPutdownOrder(std::make_shared<PutDownOrderMessage>(0x2))
{
    controlMessages.reserve(500);
    directionToMessage.emplace(std::make_pair(DirectionVector<>::UP(), MMoveAgentUp));
    directionToMessage.emplace(std::make_pair(DirectionVector<>::DOWN(), MMoveAgentDown));
    directionToMessage.emplace(std::make_pair(DirectionVector<>::LEFT(), MMoveAgentLeft));
    directionToMessage.emplace(std::make_pair(DirectionVector<>::RIGHT(), MMoveAgentRight));
}

ControllerImpl::~ControllerImpl() { networkAdapter.disconnect(); }

void ControllerImpl::tick(int timeStamp) { broadcastMessages(timeStamp); }

void ControllerImpl::reset() { controlMessages.clear(); }

const NetworkAdapter &ControllerImpl::getNetworkAdapter() const { return networkAdapter; }

NetworkAdapter &ControllerImpl::getNetworkAdapter() { return networkAdapter; }

void ControllerImpl::broadcastMessages(int timeStamp)
{
    auto messages = controlMessages.equal_range(timeStamp);
    for (auto it = messages.first; it != messages.second; ++it)
        networkAdapter.send(it->second.message, it->second.address);

    controlMessages.erase(messages.first, messages.second);
}

void ControllerImpl::translatePath(const std::vector<std::shared_ptr<Node>> &path, int address)
{
    for (size_t i = 0; i < path.size(); ++i)
    {
        if (path[i]->moved == false)
            continue;

        controlMessages.emplace(std::make_pair(path[i]->gCost - path[i]->byTime,
                                               TargetedMessage(address, directionToMessage.find(path[i]->arriveOrientation)->second)));
    }
}

void ControllerImpl::registerRoundTrip(const std::vector<std::vector<std::shared_ptr<Node>>> &roundTrip, 
                                       TaskAssignment *assignment, 
                                       int startTime, 
                                       int waypointCount)
{
    // ALTER SEMI STATIC
    pathFinder->alterEndOfInfiniteSemiStatic(roundTrip[0].back()->coords, roundTrip[0][roundTrip[0].size()-2]->gCost);

    controlMessages.emplace(std::make_pair(startTime, TargetedMessage(assignment->controlData->address, MControlGranted))); // Turn control on
    controlMessages.emplace(std::make_pair(roundTrip[0][0]->gCost, TargetedMessage(assignment->controlData->address, MPickupPod))); // Pickup Pod when arrive
    controlMessages.emplace(std::make_pair(roundTrip[waypointCount - 1][0]->gCost, TargetedMessage(assignment->controlData->address, MPutdownPod))); // Put down pod at finish

    // To Pod
    pathFinder->claimPath(roundTrip[0]); // Claim path ST3-s
    pathFinder->claimST3Interval(roundTrip[0][0]->coords,roundTrip[0][0]->gCost, roundTrip[1][roundTrip[1].size()-2]->gCost); // Claim path inbetween timeframe
    translatePath(roundTrip[0], assignment->controlData->address); // Enqueue commands
    //

    // To stations
    for(int i = 1; i < waypointCount - 1; ++i)
    {
        pathFinder->claimPath(roundTrip[i]); // Claim path ST3-s
        pathFinder->claimST3Interval(roundTrip[i][0]->coords,roundTrip[i][0]->gCost, roundTrip[i+1][roundTrip[i+1].size()-2]->gCost); // Claim path inbetween timeframe
        translatePath(roundTrip[i], assignment->controlData->address); // Enqueue commands
        controlMessages.emplace(std::make_pair(roundTrip[i][0]->gCost, TargetedMessage(assignment->controlData->address, MPutdownOrder)));
    }

    // Back to Pod
    pathFinder->claimPath(roundTrip.back()); // Claim path ST3-s
    translatePath(roundTrip.back(), assignment->controlData->address);
    //

    // EMPLACE_SEMI_STATIC
    pathFinder->emplaceSemiStatic(roundTrip.back()[0]->coords,roundTrip.back()[0]->gCost, INT_MAX, &(assignment->controlData->moveMechanism));

    controlMessages.emplace(std::make_pair(roundTrip.back()[0]->gCost, TargetedMessage(assignment->controlData->address, MControlGiveUp))); // Turn control off
}

void ControllerImpl::setPathFinder(PathFinder *pathFinder) { this->pathFinder = pathFinder; }

bool ControllerImpl::PlanTask(TaskAssignment *assignment, int startTime)
{
    std::vector<std::vector<std::shared_ptr<Node>>> roundTrip;
    int maxEnergySum = assignment->controlData->energySource.getCharge() - MIN_ENERGY_LEFT;
    int energySum = 0;

    // ############################################ Trip To Pod ####################################################
    roundTrip.emplace_back(pathFinder->findPathSoft(assignment->controlData->moveMechanism.getBody()->getPose().getPosition(),
                                                    assignment->task->getWayPoints()[0],
                                                    assignment->controlData->moveMechanism.getBody()->getPose().getOrientation(),
                                                    startTime, assignment->controlData->moveMechanism));

    energySum += roundTrip[0][0]->byEnergy;
    ++energySum; // Pod pickup energy
    if(energySum >= maxEnergySum) // Cannot plen if energy need exceed limit
        return false;
    
    // ######################################### Trip to Destinations #################################################
    int wayPointCount = assignment->task->getWayPoints().size();
    for (int i = 1; i < wayPointCount - 1; ++i)
    {
        roundTrip.emplace_back(pathFinder->findPathHard(Point<>(roundTrip[i - 1][0]->coords.first, roundTrip[i - 1][0]->coords.second),
                                                        assignment->task->getWayPoints()[i],
                                                        roundTrip[i - 1][0]->arriveOrientation,
                                                        roundTrip[i - 1][0]->gCost + 1, assignment->controlData->moveMechanism));

        energySum += roundTrip[i][0]->byEnergy;
        if(energySum >= maxEnergySum)
            return false;
    }
    //################################################ Travel back #########################################################
    roundTrip.emplace_back(pathFinder->findPathHard(Point<>(roundTrip[wayPointCount - 2][0]->coords.first, roundTrip[wayPointCount - 2][0]->coords.second),
                                                    assignment->task->getWayPoints()[wayPointCount - 1],
                                                    roundTrip[wayPointCount - 2][0]->arriveOrientation,
                                                    roundTrip[wayPointCount - 2][0]->gCost + 1, assignment->controlData->moveMechanism));

    energySum += roundTrip[wayPointCount - 1][0]->byEnergy;
    ++energySum;
    if(energySum >= maxEnergySum)
            return false;


    registerRoundTrip(roundTrip,assignment,startTime,wayPointCount);
    std::cout << "SumEnergy: " << energySum << " Address: " << assignment->controlData->address << std::endl;

    return true;
}

bool ControllerImpl::PlanCharge(const AgentControlData &controlData) { return true; }