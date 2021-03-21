#include "Warehouse.h"
#include "SchedulerImpl.h"
#include "ControllerImpl.h"
#include "WarehouseFilePersistence.h"
#include <iostream>

Warehouse::Warehouse()
    : timeStamp(0),
      scheduler(new SchedulerImpl()),
      controller(new ControllerImpl()),
      persistence(new WarehouseFilePersistence()),
      state(nullptr)
{

}

Warehouse::~Warehouse() = default;

void Warehouse::tick()
{
    std::cerr << "Warehouse tick! Timestamp: "<< timeStamp << std::endl;
    scheduler->tick();
    controller->tick();
    state->tick();
    ++timeStamp;
}

void Warehouse::loadState(std::string& srcPath)
{
    State state = persistence->load(srcPath);
    this->state.reset(std::move(&state)); // TODO: statenek írni move szemantikát!
}

void Warehouse::saveState(std::string& destPath)
{
    persistence->save(*state,destPath);
}
