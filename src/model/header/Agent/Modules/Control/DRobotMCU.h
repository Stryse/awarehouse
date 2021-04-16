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
#include "NetworkMessage.h"
#include "NetworkMessageHandler.h"
#include "RackMotor.h"
#include <queue>
#include <stdexcept>

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

    /*******************************************************
     * @brief Delivery Robot makes decisions based on status
     *******************************************************/
    enum class Status
    {
        IDLE,
        RUNNING,
        CHARGING,
        ERROR
    };

public:
    DRobotMCU(IMoveMechanism &moveMechanism,
              NetworkAdapter &networkAdapter,
              RackMotor &rackMotor,
              PodHolder &podHolder,
              IDepleting &energySource,
              Environment &env)

        : status(Status::IDLE),
          moveMechanism(moveMechanism),
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
        switch (status)
        {
        case Status::IDLE:
            requestControl();
            processMessages();
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
     * on the connected network. The agent clears its action queue and
     * goes into CHARGING status so all control is suspended until
     * a full recharge.
     ****************************************************************/
    virtual void receive(const ChargeAgentMessage &message) override
    {
        std::queue<AgentAction *>().swap(actionQueue);
        status = Status::CHARGING;
        tick(0); // Begin Charging immediately;
    }

    /*****************************************************************
     * @brief Called when the Agent received a PickupPodMessage
     * on the connected network. The agent instructs its RackMotor
     * to return a motor action that leading to pick up a pod from
     * the environment. The action is pushed into the actionqueue.
     *****************************************************************/
    virtual void receive(const PickupPodMessage &message) override
    {
        actionQueue.push(rackMotor.pickUpPodAction());
    }

    /******************************************************************
     * @brief Called when the Agent received a PutDownPodMessage
     * on the connected network. The agent instructs its RackMotor
     * to return a motor action that leading to put down a pod in
     * the environment. The action is pushed into the actionqueue.
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

    virtual void receive(const AgentControlGrantedMessage &message)
    {
        status = Status::RUNNING;
        tick(0); // Begin Performing Actions immediately
    }

    // #####################################################################################################
private:
    /*************************************************************************
     * @brief The agent sends a AgentControlRequestMessage message in the
     * network to an Agent Controlling entity and if it responds positively
     * it goes into running status, thus performing received actions.
     *************************************************************************/
    void requestControl()
    {
        networkAdapter.send(std::make_unique<AgentControlRequestMessage>(
                                AgentControlData(energySource,
                                                 moveMechanism),
                                networkAdapter.getAddress()),
                            0x1);
    }

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

    /************************************************************************
     * @brief The agent keeps sending charge signals to the environment
     * until it has been fully recharged. Then Status is changed to IDLE
     ************************************************************************/
    void doFullCharge()
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

    /************************************************************************
     * @brief When pod is picked up connect pod movement to agent movement,
     * and broadcast both agent and pod movement as different events.
     ************************************************************************/
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
    Status status;
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
