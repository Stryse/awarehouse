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
    for (const std::shared_ptr<Node> &node : path)
        controlMessages.emplace(std::make_pair(node->gCost - node->byTime,TargetedMessage(address,directionToMessage.find(node->arriveOrientation)->second)));
}

void ControllerImpl::setPathFinder(PathFinder *pathFinder) { this->pathFinder = pathFinder; }

bool ControllerImpl::PlanTask(TaskAssignment *assignment)
{
    if (assignment->controlData->address == 100)
    {
        controlMessages.emplace(std::make_pair(0, TargetedMessage(100, MControlGranted)));
        std::vector<std::shared_ptr<Node>> pathToTask = pathFinder->findPath(assignment->controlData->moveMechanism.getBody()->getPose().getPosition(),
                                                                             Point<>(7, 8, 0),
                                                                             assignment->controlData->moveMechanism.getBody()->getPose().getOrientation(),
                                                                             0, assignment->controlData->moveMechanism);

        pathFinder->claimPath(pathToTask);

        
        std::vector<std::shared_ptr<Node>> pathToTask2 = pathFinder->findPath(Point<>(7, 8, 0),
                                                                             Point<>(0, 0, 0),
                                                                             pathToTask[0]->arriveOrientation,
                                                                             pathToTask[0]->gCost, assignment->controlData->moveMechanism);
        
        pathFinder->claimPath(pathToTask2);

        translatePath(pathToTask, assignment->controlData->address);
        translatePath(pathToTask2,assignment->controlData->address);
        std::cout << pathToTask[0]->byEnergy + pathToTask2[0]->byEnergy << std::endl;
    }
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