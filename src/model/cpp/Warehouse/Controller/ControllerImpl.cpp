#include "ControllerImpl.h"
#include "AMotor.h"
#include "IMoveMechanism.h"
#include "NetworkMessage.h"
#include "PathFinder.h"
#include "SchedulerImpl.h"
#include "Task.h"

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

    // controlMessages.emplace(std::make_pair(0, TargetedMessage(100, MControlGranted)));

    // //To Task
    // controlMessages.emplace(std::make_pair(0, TargetedMessage(100, MMoveAgentUp)));
    // controlMessages.emplace(std::make_pair(1, TargetedMessage(100, MMoveAgentUp)));

    // //Pod Pickup
    // controlMessages.emplace(std::make_pair(2, TargetedMessage(100, MPickupPod)));

    // //To Task waypoint 0
    // controlMessages.emplace(std::make_pair(3, TargetedMessage(100, MMoveAgentRight)));
    // controlMessages.emplace(std::make_pair(5, TargetedMessage(100, MMoveAgentRight)));
    // controlMessages.emplace(std::make_pair(6, TargetedMessage(100, MMoveAgentRight)));
    // controlMessages.emplace(std::make_pair(7, TargetedMessage(100, MMoveAgentRight)));
    // controlMessages.emplace(std::make_pair(8, TargetedMessage(100, MMoveAgentRight)));
    // controlMessages.emplace(std::make_pair(9, TargetedMessage(100, MMoveAgentRight)));
    // controlMessages.emplace(std::make_pair(10, TargetedMessage(100, MMoveAgentRight)));
    // controlMessages.emplace(std::make_pair(11, TargetedMessage(100, MMoveAgentUp)));
    // controlMessages.emplace(std::make_pair(12, TargetedMessage(100, MPutdownOrder)));
    // controlMessages.emplace(std::make_pair(12, TargetedMessage(100, MMoveAgentLeft)));
    // controlMessages.emplace(std::make_pair(14, TargetedMessage(100, MMoveAgentLeft)));
    // controlMessages.emplace(std::make_pair(15, TargetedMessage(100, MMoveAgentLeft)));
    // controlMessages.emplace(std::make_pair(16, TargetedMessage(100, MMoveAgentLeft)));
    // controlMessages.emplace(std::make_pair(17, TargetedMessage(100, MMoveAgentLeft)));
    // controlMessages.emplace(std::make_pair(18, TargetedMessage(100, MMoveAgentLeft)));
    // controlMessages.emplace(std::make_pair(19, TargetedMessage(100, MMoveAgentLeft)));
    // controlMessages.emplace(std::make_pair(20, TargetedMessage(100, MMoveAgentDown)));
    // controlMessages.emplace(std::make_pair(21, TargetedMessage(100, MPutdownPod)));
    // controlMessages.emplace(std::make_pair(22, TargetedMessage(100, MMoveAgentDown)));
    // controlMessages.emplace(std::make_pair(23, TargetedMessage(100, MMoveAgentDown)));
    // controlMessages.emplace(std::make_pair(0, TargetedMessage(101, MControlGranted)));
    // controlMessages.emplace(std::make_pair(0, TargetedMessage(101, MMoveAgentLeft)));
    // controlMessages.emplace(std::make_pair(2, TargetedMessage(101, MMoveAgentLeft)));
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

void ControllerImpl::setPathFinder(PathFinder *pathFinder) { this->pathFinder = pathFinder; }

bool ControllerImpl::PlanTask(TaskAssignment *assignment)
{
    if(assignment->controlData->address == 100)
    {
        std::vector<std::shared_ptr<Node>> pathToTask = pathFinder->findPath(assignment->controlData->moveMechanism.getBody()->getPose().getPosition(),
                                                                             Point<>(7, 8, 0),
                                                                             assignment->controlData->moveMechanism.getBody()->getPose().getOrientation(),
                                                                             0, assignment->controlData->moveMechanism);

        pathFinder->claimPath(pathToTask);
    }
    //pathFinder->claimPath(pathToTask);
    //Plan Agent To Task waypoint 0
    //Command PickupPod
    //Plan waypoint 0 to waypoint 1
    //Command PutDownOrder
    //Plan waypoint n to n+1
    //Command PutDownOrder
    //Plan to last waypoint
    //PutDownPod
    //controlMessages.emplace(std::make_pair(0, TargetedMessage(assignment->controlData->address, MControlGranted)));
    //controlMessages.emplace(std::make_pair(0, TargetedMessage(assignment->controlData->address, MMoveAgentLeft)));
    //controlMessages.emplace(std::make_pair(2, TargetedMessage(assignment->controlData->address, MMoveAgentLeft)));
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