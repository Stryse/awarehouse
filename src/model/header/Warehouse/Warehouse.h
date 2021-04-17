#ifndef WAREHOUSE__H
#define WAREHOUSE__H

#include "IWarehousePersistence.h"
#include "State.h"
#include "TaskManager.h"
#include <memory>

// ################# FORWARD DECLARATIONS #######################
class QString;
class AScheduler;
class AController;
class Network;
class Warehouse;
class QString;
// ##############################################################

class Warehouse
{
public:
    Warehouse(std::unique_ptr<IWarehousePersistence<QString>> &&persistence = nullptr);
    virtual ~Warehouse();

public:
    void tick();
    bool loadState(const QString &srcPath);
    bool saveState(const QString &destPath);

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
    std::unique_ptr<AScheduler> scheduler;
    std::unique_ptr<AController> controller;
    std::shared_ptr<Network> network;
    std::unique_ptr<State> state;

    std::unique_ptr<IWarehousePersistence<QString>> persistence;
};

#endif /* WAREHOUSE__H */
