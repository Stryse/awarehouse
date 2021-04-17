#ifndef D_ROBOT_MCU__H
#define D_ROBOT_MCU__H

#include "AMicroController.h"
#include "AgentAction.h"
#include "NetworkMessage.h"
#include "NetworkMessageHandler.h"
#include "boost/signals2.hpp"
#include <queue>
#include <stdexcept>
// ############################# FORWARD DECLARATIONS ##############################
class AgentAction;
class IDepleting;
class IMoveMechanism;
class RackMotor;
class MotorAction;
class NetworkAdapter;
class PodHolder;
class ObservableNavEnvironment;
// #################################################################################
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
class DRobotMCU : public AMicroController,
                  public NetworkMessageHandler
{
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
              ObservableNavEnvironment &env);

    explicit DRobotMCU(const DRobotMCU &other) = delete;
    explicit DRobotMCU(DRobotMCU &&other) = delete;
    DRobotMCU &operator=(const DRobotMCU &other) = delete;

    virtual ~DRobotMCU();

public:
    /*****************************************************************************
     * @brief Called when time elapsed.
     * The agent processes all incoming messages, and finally
     * does its actions from the action queue which take one time unit altogether.
     *****************************************************************************/
    virtual void tick(int time) override;

public:
    // ########################## NetworkMessageHandler Implementation ################################ //

    /***************************************************************
     * @brief Called when the Agent received a MoveAgentMessage
     * on the connected network. The agent instructs its MoveMechanism
     * to return a sequence of motor actions leading to the desired
     * direction. The motor actions are pushed into the actionqueue.
     ***************************************************************/
    virtual void receive(const MoveAgentMessage &message) override;

    /****************************************************************
     * @brief Called when the Agent received a ChargeAgentMessage
     * on the connected network. The agent clears its action queue and
     * goes into CHARGING status so all control is suspended until
     * a full recharge.
     ****************************************************************/
    virtual void receive(const ChargeAgentMessage &message) override;

    /*****************************************************************
     * @brief Called when the Agent received a PickupPodMessage
     * on the connected network. The agent instructs its RackMotor
     * to return a motor action that leading to pick up a pod from
     * the environment. The action is pushed into the actionqueue.
     *****************************************************************/
    virtual void receive(const PickupPodMessage &message) override;

    /******************************************************************
     * @brief Called when the Agent received a PutDownPodMessage
     * on the connected network. The agent instructs its RackMotor
     * to return a motor action that leading to put down a pod in
     * the environment. The action is pushed into the actionqueue.
     ******************************************************************/
    virtual void receive(const PutDownPodMessage &message) override;

    /******************************************************************
     * @brief Called when the Agent received a PutDownOrderMessage
     * on the connected network. The agent sends a PutDownOrderSignal to
     * the appropriate Tile, and if it accepts it an order is transferred
     * from the Agent's holded Pod inventory to the Tile.
     ******************************************************************/
    virtual void receive(const PutDownOrderMessage &message) override;

    /*******************************************************************
     * @brief 
     *******************************************************************/
    virtual void receive(const AgentControlGrantedMessage &message) override;

    // #####################################################################################################
private:
    /*************************************************************************
     * @brief The agent sends a AgentControlRequestMessage message in the
     * network to an Agent Controlling entity and if it responds positively
     * it goes into running status, thus performing received actions.
     *************************************************************************/
    void requestControl();

    /*******************************************************************
     * @brief Poll and dispatch all messages from the NetworkAdapters 
     * messageQueue to the appropriate NetworkMessagehandler.
     *******************************************************************/
    void processMessages();

    /***********************************************************************
     * @brief Poll and perform actions from the action queue until an action
     * is performed which does not take 0 time units.
     ***********************************************************************/
    void doActions();

    /************************************************************************
     * @brief The agent keeps sending charge signals to the environment
     * until it has been fully recharged. Then Status is changed to IDLE
     ************************************************************************/
    void doFullCharge();

    /************************************************************************
     * @brief When pod is picked up connect pod movement to agent movement,
     * and broadcast both agent and pod movement as different events.
     ************************************************************************/
    void connect_PodMovementToAgentMovement();

private:
    Status status;
    // ############ Modules ################ //
    IMoveMechanism &moveMechanism;
    NetworkAdapter &networkAdapter;
    RackMotor &rackMotor;
    PodHolder &podHolder;
    IDepleting &energySource;
    ObservableNavEnvironment &environment;

    std::unique_ptr<AgentControlData> controlData;
    // ########### Connections ############# //
    boost::signals2::connection AgentMovement2PodMovement;

    // ##################################### //
    std::queue<AgentAction *> actionQueue;
};

#endif
