#ifndef D_ROBOT_MCU__H
#define D_ROBOT_MCU__H

#include "AMicroController.h"
#include "AgentAction.h"
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
// ######################################################################### //

/**********************************************************************************
 * @brief Micro Controller Unit of a DeliveryRobot.
 * This module is responsible for the DeliveryRobot's decision making.
 * Each Tick it does the following:
 * - Refresh Sensor Data. // Soon
 * - Handle messages coming from a network. (eg. Instructions from a Controller)
 * - Polls action queue
 * - Performs the polled action(s).
 *
 **********************************************************************************/
template <typename TEnvironment, typename TBody, typename TEnergy = config::agent::DefaultEnergy>

class DRobotMCU : public AMicroController,
                  public NetworkMessageHandler
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
        connect_PodMovementToAgentMovement();
    }

    virtual ~DRobotMCU() {}

public:
    /*****************************************************************************
     * @brief Called when time elapsed.
     * The agent processes all incoming messages, and finally
     * does its actions from the action queue which take one time unit altogether.
     *****************************************************************************/
    virtual void tick(int time) override
    {
        processMessages();
        doActions();
    }

public:
    // ########################## NetworkMessageHandler Implementation ################################ //

    /***************************************************************
     * @brief Called when the Agent received a MoveAgentMessage
     * on the connected network. The agent instructs its MoveMechanism
     * to return a sequence of motor actions leading to the desired
     * direction. The motor actions are pushed into the actionqueue.
     ***************************************************************/
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

    /****************************************************************
     * @brief Called when the Agent received a ChargeAgentMessage
     * on the connected network. With the pushed action the agent sends
     * a ChargeSignal to the appropriate Tile, and if it's a Charger 
     * entity it charges the Agent with some Energy.
     ****************************************************************/
    virtual void receive(const ChargeAgentMessage &message) override
    {
    }

    /*****************************************************************
     * @brief Called when the Agent received a PickupPodMessage
     * on the connected network. The agent sends a PickUpPodSignal to
     * the appropriate Tile, and if it accepts it a pod is transferred
     * to the agent's Pod Holder;
     *****************************************************************/
    virtual void receive(const PickupPodMessage &message) override
    {
        actionQueue.push(rackMotor.pickUpPodAction());
    }

    /******************************************************************
     * @brief Called when the Agent received a PutDownPodMessage
     * on the connected network. The agent sends a PutDownPodSignal to
     * the appropriate Tile, and if it accepts it a pod is transferred
     * from the Agent's Pod Holder to the Tile.
     ******************************************************************/
    virtual void receive(const PutDownPodMessage &message) override
    {
        actionQueue.push(rackMotor.putDownPodAction());
    }

    /******************************************************************
     * @brief Called when the Agent received a PutDownOrderMessage
     * on the connected network. The agent sends a PutDownOrderSignal to
     * the appropriate Tile, and if it accepts it an order is transferred
     * from the Agent's holded Pod inventory to the Tile.
     ******************************************************************/
    virtual void receive(const PutDownOrderMessage &message) override
    {
    }

    // #####################################################################################################
private:
    /*******************************************************************
     * @brief Poll and dispatch all messages from the NetworkAdapters 
     * messageQueue to the appropriate NetworkMessagehandler.
     *******************************************************************/
    void processMessages()
    {
        while (!networkAdapter.isMessageQueueEmpty())
        {
            std::unique_ptr<AbstractNetworkMessage> message = networkAdapter.poll();
            message->dispatch(*this);
        }
    }

    /***********************************************************************
     * @brief Poll and perform actions from the action queue until an action
     * is performed which does not take 0 time units.
     ***********************************************************************/
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

    void connect_PodMovementToAgentMovement()
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

private:
    // ############ Modules ################ //
    IMoveMechanism &moveMechanism;
    NetworkAdapter &networkAdapter;
    RackMotor &rackMotor;
    PodHolder &podHolder;
    IDepleting &energySource;
    Environment &environment;

    // ########### Connections ############# //
    boost::signals2::connection AgentMovement2PodMovement;

    // ##################################### //
    std::queue<AgentAction *> actionQueue;
};

#endif
