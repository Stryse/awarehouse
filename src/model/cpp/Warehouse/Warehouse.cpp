#include "Warehouse.h"
#include "ControllerImpl.h"
#include "IWarehousePersistence.h"
#include "SchedulerImpl.h"
#include <iostream>

Warehouse::Warehouse(std::unique_ptr<IWarehousePersistence<QString>> &&persistence)
    : timeStamp(0),
      scheduler(new SchedulerImpl()),
      controller(new ControllerImpl()),
      persistence(std::move(persistence)),
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

bool Warehouse::loadState(const QString &srcPath)
{
    state.reset(persistence->load(srcPath));
    timeStamp = 0;

    return state != nullptr;
}

bool Warehouse::saveState(const QString &destPath)
{
    return persistence->save(*state, destPath);
}

const std::unique_ptr<State> &Warehouse::getState() const
{
    return state;
}
