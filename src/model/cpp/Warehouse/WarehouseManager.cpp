#include "WarehouseManager.h"
#include "Warehouse.h"
#include "Simulator.h"
#include "ISimulator.h"
#include <algorithm>
#include <utility>
#include "WarehouseFilePersistence.h"

WarehouseManager::WarehouseManager()
    : displayed(nullptr)
{
    init();
}

WarehouseManager::~WarehouseManager()
{}

void WarehouseManager::init()
{
    // Add one default empty warehouse to manager
    warehouses.push_back(std::make_unique<Warehouse>(
                         std::make_unique<WarehouseFilePersistence>()));

    // Set default as displayed
    displayed = warehouses[0].get();

    // Add one default simulator to manager
    simulators.push_back(std::make_unique<Simulator>());

    // Associate default warehouse with default simulator
    wh2sim.insert(std::pair<Warehouse*,ISimulator*>(displayed,simulators[0].get()));
    simulators[0]->bindSimTypeData(displayed);
}

void WarehouseManager::simulationStart(Warehouse* warehouse)
{
    auto simulatorIt = wh2sim.find(warehouse);
    if(simulatorIt != wh2sim.end() /* Has corresponding simulator object */)
    {
        ISimulator* sim = simulatorIt->second;
        sim->startSimulation();
    }
    else
    {
        ISimulator* sim = findFreeSimulator();
        if(sim != nullptr)
        {
            wh2sim.insert(std::pair<Warehouse*,ISimulator*>(warehouse,sim));
            sim->bindSimTypeData(warehouse);
            sim->startSimulation();
        }
        else
            throw std::runtime_error("All simulators are used");
    }
}

void WarehouseManager::simulationStart()
{
    simulationStart(displayed);
}

void WarehouseManager::simulationStop(Warehouse* warehouse)
{
    auto simulatorIt = wh2sim.find(warehouse);
    if(simulatorIt != wh2sim.end() /* Has corresponding simulator object */)
    {
        ISimulator* sim = simulatorIt->second;

        if(simulatorIt->first != displayed)
            wh2sim.erase(simulatorIt);

        sim->stopSimulation();
    }
}

void WarehouseManager::simulationStop()
{
    simulationStop(displayed);
}

void WarehouseManager::setTickRate(int tickrate, Warehouse* warehouse)
{
    auto simulatorIt = wh2sim.find(warehouse);
    if(simulatorIt != wh2sim.end() /* Has corresponding simulator object */)
    {
        ISimulator* sim = simulatorIt->second;
        sim->setTickRate(tickrate);
    }
}

void WarehouseManager::setTickRate(int tickrate)
{
    setTickRate(tickrate,displayed);
}

void WarehouseManager::setWarehouseStateAt(int timeStamp, Warehouse* warehouse)
{
    auto simulatorIt = wh2sim.find(warehouse);
    if(simulatorIt != wh2sim.end() /* Has corresponding simulator object */)
    {
        ISimulator* sim = simulatorIt->second;
        sim->setWarehouseStateAt(timeStamp);
    }
}

Warehouse* WarehouseManager::getDisplayedWarehouse()
{
    return displayed;
}

const Warehouse* WarehouseManager::getDisplayedWarehouse() const
{
    return displayed;
}

const WarehouseManager::ISimulator* WarehouseManager::getDisplayedWarehouseSimulator() const
{
    auto simulatorIt = wh2sim.find(displayed);
    if(simulatorIt != wh2sim.end() /* Has corresponding simulator object */)
    {
        ISimulator* sim = simulatorIt->second;
        return sim;
    }
    return nullptr;
}

void WarehouseManager::setWarehouseStateAt(int timeStamp)
{
    setWarehouseStateAt(timeStamp,displayed);
}

WarehouseManager::ISimulator* WarehouseManager::findFreeSimulator()
{
    auto simulatorIt = std::find_if(simulators.begin(),simulators.end(),
         [](const std::unique_ptr<ISimulator>& sim){
           return sim->isAvailable();
    });

    return (simulatorIt == simulators.end()) ? nullptr : simulatorIt->get();
}
