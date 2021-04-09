#include "Warehouse.h"
#include "ControllerImpl.h"
#include "IWarehousePersistence.h"
#include "Network.h"
#include "SchedulerImpl.h"
#include <iostream>

Warehouse::Warehouse(std::unique_ptr<IWarehousePersistence<QString>> &&persistence)
    : timeStamp(0),
      scheduler(std::make_unique<SchedulerImpl>()),
      controller(std::make_unique<ControllerImpl>()),
      persistence(std::move(persistence)),
      state(nullptr),
      network(std::make_shared<Network>())
{
}

Warehouse::~Warehouse() = default;

void Warehouse::tick()
{
    std::cerr << "Warehouse tick! Timestamp: " << timeStamp << std::endl;
    scheduler->tick(timeStamp);
    controller->tick(timeStamp);
    state->tick(timeStamp);
    ++timeStamp;
}

bool Warehouse::loadState(const QString &srcPath)
{
    state.reset(persistence->load(srcPath));
    if (state)
    {
        timeStamp = 0;
        network->reset();
        controller->getNetworkAdapter().connectWithAddress(network, 0x0);
        scheduler->getNetworkAdapter().connectWithAddress(network, 0x1);

        for (auto &agent : state->getRobots())
            agent->getNetworkAdapter().connect(network);
    }

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
