#include "Warehouse.h"
#include "ControllerImpl.h"
#include "SchedulerImpl.h"
#include "WarehouseFilePersistence.h"
#include <iostream>

Warehouse::Warehouse()
    : timeStamp(0),
      scheduler(new SchedulerImpl()),
      controller(new ControllerImpl()),
      state(nullptr)
{
}

Warehouse::~Warehouse() = default;

void Warehouse::tick()
{
    std::cerr << "Warehouse tick! Timestamp: " << timeStamp << std::endl;
    scheduler->tick();
    controller->tick();
    state->tick();
    ++timeStamp;
}