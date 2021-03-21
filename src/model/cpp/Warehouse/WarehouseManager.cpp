#include "WarehouseManager.h"
#include "Warehouse.h"
#include "Simulator.h"

WarehouseManager::WarehouseManager()
    : displayed(nullptr)
{
    init();
}

void WarehouseManager::init()
{
    // Add one default empty warehouse to manager
    warehouses.emplace_back();

    // Set default as displayed
    displayed = warehouses[0].get();

    // Add one default simulator to manager
    simulators.emplace_back();

    // Associate default warehouse with default simulator
    wh2sim.emplace(warehouses[0].get(),simulators[0].get());
}
