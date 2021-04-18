#include "Warehouse.h"
#include "ControllerImpl.h"
#include "IWarehousePersistence.h"
#include "Network.h"
#include "SchedulerImpl.h"
#include <iostream>

Warehouse::Warehouse(std::unique_ptr<IWarehousePersistence<QString>> &&persistence)
    : timeStamp(0),
      controller(std::make_unique<ControllerImpl>()),
      scheduler(std::make_unique<SchedulerImpl>()),
      network(std::make_shared<Network>()),
      state(nullptr),
      persistence(std::move(persistence))
{
}

Warehouse::~Warehouse() = default;

void Warehouse::tick()
{
    std::cerr << "Warehouse tick! Timestamp: " << timeStamp << std::endl;

    state->tick(timeStamp);
    scheduler->tick(timeStamp);
    controller->tick(timeStamp);

    ++timeStamp;
}

bool Warehouse::loadState(const QString &srcPath, const Settings* settings)
{
    state.reset(persistence->load(srcPath, settings));
    if (state)
    {
        timeStamp = 0;
        setupNetwork(*state);
        scheduler->setTaskManager(&state->getTaskManager());
        scheduler->setController(controller.get());
        tick();

        // TODO REMOVE
        //controller->getNetworkAdapter().send(std::make_unique<AgentControlGrantedMessage>(0x1), 100);
        //controller->getNetworkAdapter().send(std::make_unique<MoveAgentMessage>(DirectionVector<>::UP(), 0x1), 100);

        //controller->getNetworkAdapter().send(std::make_unique<AgentControlGrantedMessage>(0x1), 101);
        //controller->getNetworkAdapter().send(std::make_unique<MoveAgentMessage>(DirectionVector<>::LEFT(), 0x1), 101);

        //controller->getNetworkAdapter().send(std::make_unique<AgentControlGrantedMessage>(0x1), 102);
        //controller->getNetworkAdapter().send(std::make_unique<MoveAgentMessage>(DirectionVector<>::DOWN(), 0x1), 102);
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

void Warehouse::setupNetwork(State &state)
{
    network->reset();
    scheduler->getNetworkAdapter().connectWithAddress(network, 0x1);
    controller->getNetworkAdapter().connectWithAddress(network, 0x2);

    for (auto &agent : state.getRobots())
        agent->getNetworkAdapter().connect(network);
}
