#ifndef WAREHOUSE__H
#define WAREHOUSE__H

#include "IWarehousePersistence.h"
#include "State.h"
#include "Settings.h"
#include "TaskManager.h"
#include <memory>

// ################# FORWARD DECLARATIONS #######################
class QString;
class SchedulerImpl;
class ControllerImpl;
class Network;
class Warehouse;
class QString;
// ##############################################################
/***************************************************************
 * @brief The class of an automated warehouse.
 * A warehouse has a changing state which can be invoked with tick() method.
 * 
 * A warehouse has a:
 * Controller - Translates task assignments to command messages and relays them
 * Scheduler  - Assigns Tasks to agents.
 * Network    - Network which connects all actors in the warehouse
 * State      - Wrapper of time changing objects
 *****************************************************************/
class Warehouse
{
public:
    Warehouse(std::unique_ptr<IWarehousePersistence<QString>> &&persistence = nullptr);
    virtual ~Warehouse();

public:

    /**************************************************************************************
     * @brief Signal that indicates that time elapsed and next warehouse state is imminent.
     **************************************************************************************/
    void tick();

    /**************************************************************************************
     * @brief Load a state from a file located at srcPath.
     **************************************************************************************/
    bool loadState(const QString &srcPath, const Settings* = nullptr);

    /**************************************************************************************
     * @brief Save a state to a file located at destPath.
     **************************************************************************************/
    bool saveState(const QString &destPath);

    /**************************************************************************************
     * @brief Returns the current timestamp of the simulation.
     **************************************************************************************/
    int getTimeStamp() const;

public:
    const std::unique_ptr<State> &getState() const;

private:
    /***************************************************
     * @brief Resets the network. Then connects:
     * - Controller with address: 1
     * - Scheduler  with address: 2
     * - All agents with dynamic address.
     ***************************************************/
    void setupNetwork(State &state);

private:
    int timeStamp;
    std::unique_ptr<ControllerImpl> controller;
    std::unique_ptr<SchedulerImpl> scheduler;
    std::shared_ptr<Network> network;
    std::unique_ptr<State> state;

    std::unique_ptr<IWarehousePersistence<QString>> persistence;
};

#endif /* WAREHOUSE__H */
