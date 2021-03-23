#ifndef WAREHOUSE_MANAGER__H
#define WAREHOUSE_MANAGER__H

#include <map>
#include <memory>
#include <vector>
#include "ISimulator.h"

// ############################# FORWARD DECLARATIONS ################################
class Warehouse;
// ###################################################################################

class WarehouseManager
{
public:
    using ISimulator = ISimulator<Warehouse>;

private:
    Warehouse *displayed;
    std::vector<std::unique_ptr<Warehouse>> warehouses;
    std::vector<std::unique_ptr<ISimulator>> simulators;
    std::map<Warehouse *, ISimulator *> wh2sim;

public:
    WarehouseManager();
    virtual ~WarehouseManager();

public:
    void simulationStart();
    void simulationStart(Warehouse *);

    void simulationStop();
    void simulationStop(Warehouse *);

    void setTickRate(int);
    void setTickRate(int, Warehouse *);

    void setWarehouseStateAt(int timeStamp);
    void setWarehouseStateAt(int timeStamp, Warehouse *);

private:
    void init();
    ISimulator* findFreeSimulator();
};

#endif /* WAREHOUSE_MAMAGER__H */
