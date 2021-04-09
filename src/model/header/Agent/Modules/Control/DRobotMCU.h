#ifndef D_ROBOT_MCU__H
#define D_ROBOT_MCU__H

#include "AMicroController.h"
#include "AgentAction.h"
#include "AgentSignals.h"
#include "IDepleting.h"
#include "IMoveMechanism.h"
#include "LibConfig.h"
#include "MotorAction.h"
#include "NetworkAdapter.h"
#include "NetworkMessageHandler.h"
#include "RackMotor.h"
#include <queue>

// ####################### FORWARD DECLARATIONS ############################ //
template <typename Body, typename Energy>
class IMoveMechanism;
template <typename Environment>
class PodHolder;
class NetworkAdapter;
// #########################################################################

/**********************************************************************************
 * @brief Micro Controller Unit of a DeliveryRobot.
 * This module is responsible for the DeliveryRobot's decision making.
 * Each Tick it does the following:
 * - Refresh Sensor Data.
 * - Refresh messages coming from a network. (eg. Instructions from a Controller)
 * - Polls action queue
 * - Performs the polled action.
 *
 **********************************************************************************/
template <typename TEnvironment, typename TBody, typename TEnergy = config::agent::DefaultEnergy>
class DRobotMCU : public AMicroController, public NetworkMessageHandler
{
public:
    using Body = TBody;
    using Energy = TEnergy;
    using IMoveMechanism = ::IMoveMechanism<Body, Energy>;
    using IDepleting = ::IDepleting<Energy>;
    using Environment = TEnvironment;
    using RackMotor = ::RackMotor<Body, Energy>;
    using PodHolder = ::PodHolder<Environment>;
    using MotorAction = ::MotorAction<Body, Energy>;

public:
    DRobotMCU(IMoveMechanism &moveMechanism,
              NetworkAdapter &networkAdapter,
              RackMotor &rackMotor,
              PodHolder &podHolder,
              IDepleting &energySource,
              Environment &env)

        : moveMechanism(moveMechanism),
          networkAdapter(networkAdapter),
          rackMotor(rackMotor),
          podHolder(podHolder),
          energySource(energySource),
          environment(env)
    {
    }

    virtual ~DRobotMCU() {}

public:
    virtual void tick(int time) override
    {
        processMessages();
        doActions();
    }

public:
    // ########################## NetworkMessageHandler Implementation ################################ //

    virtual void receive(const MoveAgentMessage &message) override
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

    virtual void receive(const ChargeAgentMessage &message) override
    {
    }

    virtual void receive(const PickupPodMessage &message) override
    {
    }

    virtual void receive(const PutDownPodMessage &message) override
    {
    }

    virtual void receive(const PutDownOrderMessage &message) override
    {
    }

    // #####################################################################################################
private:
    void processMessages()
    {
        while (!networkAdapter.isMessageQueueEmpty())
        {
            std::unique_ptr<AbstractNetworkMessage> message = networkAdapter.poll();
            message->dispatch(*this);
        }
    }

    void doActions()
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

private:
    // ############ Modules ################ //
    IMoveMechanism &moveMechanism;
    NetworkAdapter &networkAdapter;
    RackMotor &rackMotor;
    PodHolder &podHolder;
    IDepleting &energySource;
    Environment &environment;

    // ##################################### //
    std::queue<AgentAction *> actionQueue;
};

#endif
