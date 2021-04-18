#include "DRobotMCU.h"
#include "AgentSignals.h"
#include "IDepleting.h"
#include "IMoveMechanism.h"
#include "MotorAction.h"
#include "NetworkAdapter.h"
#include "ObservableEnvironment.h"
#include "Pod.h"
#include "PodHolder.h"
#include "RackMotor.h"

DRobotMCU::DRobotMCU(IMoveMechanism &moveMechanism,
                     NetworkAdapter &networkAdapter,
                     RackMotor &rackMotor,
                     PodHolder &podHolder,
                     IDepleting &energySource,
                     ObservableNavEnvironment &env)

    : status(Status::IDLE),
      moveMechanism(moveMechanism),
      networkAdapter(networkAdapter),
      rackMotor(rackMotor),
      podHolder(podHolder),
      energySource(energySource),
      environment(env),
      controlData(std::make_unique<AgentControlData>(energySource, moveMechanism))
{
    connect_PodMovementToAgentMovement();
}

DRobotMCU::~DRobotMCU() {}

void DRobotMCU::tick(int time)
{
    switch (status)
    {
    case Status::IDLE:
        processMessages(); // Status may change
        if (status == Status::IDLE)
            requestControl();
        break;

    case Status::RUNNING:
        processMessages();
        doActions();
        break;

    case Status::CHARGING:
        doFullCharge();
        break;

    case Status::ERROR:
    default:
        throw std::runtime_error("Delivery robot has been set to ERROR STATUS");
        break;
    }
}

void DRobotMCU::receive(const MoveAgentMessage &message)
{
    if (moveMechanism.canMove(message.getMoveDirection()))
    {
        std::queue<MotorAction *> motorActions = moveMechanism.move(message.getMoveDirection());
        while (!motorActions.empty())
        {
            actionQueue.push(motorActions.front());
            motorActions.pop();
        }
    }
}

void DRobotMCU::receive(const ChargeAgentMessage &message)
{
    std::queue<AgentAction *>().swap(actionQueue);
    status = Status::CHARGING;
    tick(0); // Begin Charging immediately;
}

void DRobotMCU::receive(const PickupPodMessage &message)
{
    actionQueue.push(rackMotor.pickUpPodAction());
}

void DRobotMCU::receive(const PutDownPodMessage &message)
{
    actionQueue.push(rackMotor.putDownPodAction());
}

void DRobotMCU::receive(const PutDownOrderMessage &message)
{
}

void DRobotMCU::receive(const AgentControlGrantedMessage &message)
{
    status = Status::RUNNING;
    tick(0); // Begin Performing Actions immediately
}

void DRobotMCU::requestControl()
{
    controlData->address = networkAdapter.getAddress();
    networkAdapter.send(std::make_unique<AgentControlRequestMessage>(
                            controlData.get(),
                            networkAdapter.getAddress()),
                        0x1);
}

void DRobotMCU::processMessages()
{
    while (!networkAdapter.isMessageQueueEmpty())
    {
        std::shared_ptr<AbstractNetworkMessage> message = networkAdapter.poll();
        message->dispatch(*this);
    }
}

void DRobotMCU::doActions()
{
    if (actionQueue.empty())
        return;

    while (true)
    {
        AgentAction *action = actionQueue.front();
        (*action)();

        actionQueue.pop();
        if (action->getDuration() != 0)
            break;
    }
}

void DRobotMCU::doFullCharge()
{
    if (energySource.getCharge() < energySource.getMaxCharge())
    {
        environment.getVolume(moveMechanism.getBody()->getPose().getPosition())
            ->receive(energySource, ChargeSignal());
    }
    else
    {
        status = Status::IDLE;
        tick(0); // Request control immediately
    }
}

void DRobotMCU::connect_PodMovementToAgentMovement()
{
    rackMotor.onPodPickedUp.connect([&](const Body &body) {
        // Setup Connection
        AgentMovement2PodMovement = moveMechanism.onBodyMoved.connect([&](const Body &body) {
            podHolder.getChildPod()->onBodyMoved(*podHolder.getChildPod()->getBody());
        });
    });

    rackMotor.onPodPutDown.connect([&](const Body &body) {
        // TearDown Connection
        AgentMovement2PodMovement.disconnect();
    });
}