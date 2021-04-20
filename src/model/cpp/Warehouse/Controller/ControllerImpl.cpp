#include "ControllerImpl.h"
#include "AMotor.h"
#include "IMoveMechanism.h"
#include "NetworkMessage.h"
#include "PathFinder.h"
#include "SchedulerImpl.h"
#include "Task.h"
#include <iostream>

ControllerImpl::ControllerImpl(PathFinder *pathFinder)
    : pathFinder(pathFinder),
      MControlGranted(std::make_shared<AgentControlGrantedMessage>(0x2)),
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

ControllerImpl::~ControllerImpl()
{
    networkAdapter.disconnect();
}

void ControllerImpl::tick(int timeStamp)
{
    broadcastMessages(timeStamp);
}

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

void ControllerImpl::setPathFinder(PathFinder *pathFinder) { this->pathFinder = pathFinder; }

bool ControllerImpl::PlanTask(TaskAssignment *assignment)
{
    std::vector<std::vector<std::shared_ptr<Node>>> roundTrip;

    // ############################################ Trip To Pod ####################################################
    controlMessages.emplace(std::make_pair(0, TargetedMessage(assignment->controlData->address, MControlGranted)));
    roundTrip.emplace_back(pathFinder->findPath(assignment->controlData->moveMechanism.getBody()->getPose().getPosition(),
                                                assignment->task->getWayPoints()[0],
                                                assignment->controlData->moveMechanism.getBody()->getPose().getOrientation(),
                                                0, assignment->controlData->moveMechanism));

    pathFinder->claimPath(roundTrip[0]);
    translatePath(roundTrip[0], assignment->controlData->address);

    // ######################################### Trip to Destinations #################################################
    int sumEnergy = roundTrip[0][0]->byEnergy;
    for (int i = 1; i < 3; ++i)
    {
        roundTrip.emplace_back(pathFinder->findPath(Point<>(roundTrip[i - 1][0]->coords.first, roundTrip[i - 1][0]->coords.second),
                                                    assignment->task->getWayPoints()[i],
                                                    roundTrip[i - 1][0]->arriveOrientation,
                                                    roundTrip[i - 1][0]->gCost, assignment->controlData->moveMechanism));

        sumEnergy += roundTrip[i][0]->byEnergy;
        pathFinder->claimPath(roundTrip[i]);
        //pathFinder->claimST3(std::make_tuple(roundTrip[i][0]->coords.first, roundTrip[i][0]->coords.second, roundTrip[i][0]->gCost+3));

        translatePath(roundTrip[i], assignment->controlData->address);
    }

    // ################################################ Finish #########################################################
    std::cout << sumEnergy << "IP " << assignment->controlData->address << std::endl;
    return true;
}

bool ControllerImpl::PlanCharge(const AgentControlData &controlData)
{
    return true;
}

const NetworkAdapter &ControllerImpl::getNetworkAdapter() const
{
    return networkAdapter;
}

NetworkAdapter &ControllerImpl::getNetworkAdapter()
{
    return networkAdapter;
}