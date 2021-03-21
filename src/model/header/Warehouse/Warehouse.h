#ifndef WAREHOUSE__H
#define WAREHOUSE__H

#include <memory>
#include <string>
#include "State.h"

// ################# FORWARD DECLARATIONS #######################
class AScheduler;
class AController;
class Warehouse;
template <typename rsc> class IWarehousePersistence;
// ##############################################################

class Warehouse
{
public:
    Warehouse();
    Warehouse(State&& state);
    Warehouse(const State& state);
    ~Warehouse();
public:
    void tick();
    void loadState(std::string& srcPath);
    void saveState(std::string& destPath);

private:
    int timeStamp;
    std::unique_ptr<AScheduler> scheduler;
    std::unique_ptr<AController> controller;
    std::unique_ptr<IWarehousePersistence<std::string>> persistence;
    std::unique_ptr<State> state;
};

#endif /* WAREHOUSE__H */
