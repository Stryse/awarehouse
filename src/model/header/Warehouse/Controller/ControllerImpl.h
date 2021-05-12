#ifndef CONTROLLER_IMPL__H
#define CONTROLLER_IMPL__H

#include "NetworkAdapter.h"
#include <memory>
#include <unordered_map>
#include <map>

// ###################### FORWARD DECLARATIONS ####################### //
class AbstractNetworkMessage;
class SchedulerImpl;
class TaskAssignment;
class PathFinder;
class Node;
class ChargingStation;
// ################################################################### //

class ControllerImpl
{
public:

    /**************************************************************************************************************************
     * @brief Construct a new Controller Impl object
     * 
     * @param chargingStations Charging stations are needed to be known by the controller because it is responsible
     * for claiming these for agents and controller needs to plan the charging action sequences.
     * 
     * @param pathFinder Pathfinder is needed to be known by the controller because paths are needed to be translated
     * as action sequences and sent to the agents so they can perform them.
     **************************************************************************************************************************/
    ControllerImpl(std::vector<std::shared_ptr<ChargingStation>>* chargingStations = nullptr ,PathFinder *pathFinder = nullptr);
    virtual ~ControllerImpl();

public:

    /***************************************************************************************
     * @brief Signal that indicates that time elapsed and the Controller might need to act.
     * Called each tick of the simulation.
     ***************************************************************************************/
    void tick(int timeStamp);

    /**************************************************************
     * @brief Deletes all control messages from the message queue.
     **************************************************************/
    void reset();

    /*******************************************************************************
     * @brief Plans a delivery task for an agent startint at the provided timestamp.
     * @throws No Path error if planning is unsuccessful.
     *******************************************************************************/
    bool PlanTask(TaskAssignment *assignment, int timeStamp);

    /*******************************************************************************
     * @brief Plans a charge task for an agent starting at the provided timestamp.
     * Charge plans should not be unsuccessful since agents need to reach charging
     * stations before depletion.
     *******************************************************************************/
    bool PlanCharge(const AgentControlData &assignment, int timeStamp);

    /*******************************************************************************
     * @brief Translates a path provided by the planner to control messages which
     * the agents interpret and translate them into action sequences.
     *******************************************************************************/
    void translatePath(const std::vector<std::shared_ptr<Node>> &path, int address);

    /********************************************************************************
     * @brief Registers a sequence of paths and actions in the reservation table
     * so the planner can plan collision free paths.
     *********************************************************************************/
    void registerRoundTrip(const std::vector<std::vector<std::shared_ptr<Node>>> &roundTrip, 
                           TaskAssignment *assignment, 
                           int startTime, int waypointCount);

    // ############################ Setter ####################################
    void setPathFinder(PathFinder *pathfinder);
    void setChargingStations(std::vector<std::shared_ptr<ChargingStation>>* chargingStations);

    // ############################ Getter ####################################
    const NetworkAdapter &getNetworkAdapter() const;
    NetworkAdapter &getNetworkAdapter();

private:
    void broadcastMessages(int timeStamp);

private:
    std::vector<std::shared_ptr<ChargingStation>>* chargingStations;
    PathFinder *pathFinder;
    NetworkAdapter networkAdapter;
    std::unordered_multimap<int, TargetedMessage> controlMessages;
private:
    // ################## Messages #############################
    std::shared_ptr<AgentControlGrantedMessage> MControlGranted;
    std::shared_ptr<AgentControlGiveUpMessage> MControlGiveUp;
    std::shared_ptr<ChargeAgentMessage> MChargeAgent;
    std::shared_ptr<MoveAgentMessage> MMoveAgentUp;
    std::shared_ptr<MoveAgentMessage> MMoveAgentDown;
    std::shared_ptr<MoveAgentMessage> MMoveAgentLeft;
    std::shared_ptr<MoveAgentMessage> MMoveAgentRight;
    std::shared_ptr<PickupPodMessage> MPickupPod;
    std::shared_ptr<PutDownPodMessage> MPutdownPod;
    std::shared_ptr<PutDownOrderMessage> MPutdownOrder;
    // #########################################################
    std::map<DirectionVector<>,std::shared_ptr<MoveAgentMessage>> directionToMessage;

    static const int MIN_ENERGY_LEFT = 20;
};

#endif /* CONTROLLER_IMPL__H */