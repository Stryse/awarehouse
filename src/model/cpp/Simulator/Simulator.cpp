#include "Simulator.h"
#include "Warehouse.h"
#include <stdexcept>

Simulator::Simulator()
    : isRunning(false), warehouse(nullptr), timer(this), tickRate(TickRate::NORMAL)
{
    timer.setInterval(static_cast<int>(tickRate));
    timer.callOnTimeout([=]() {
        warehouse->tick();
    });
}

Simulator::~Simulator(){}

bool Simulator::isAvailable() const { return !isRunning; }
void Simulator::makeAvailable() { isRunning = false; }
void Simulator::makeUnAvailable() { isRunning = true; }

void Simulator::bindSimTypeData(Warehouse *warehouse)
{
    if (isAvailable())
        this->warehouse = warehouse;
}

void Simulator::startSimulation()
{
    if (isAvailable() && warehouse != nullptr)
    {
        timer.start();
        makeUnAvailable();
    }
}

void Simulator::stopSimulation()
{
    if (!isAvailable() && warehouse != nullptr)
    {
        timer.stop();
        makeAvailable();
    }
}

void Simulator::setTickRate(TickRate tickrate)
{
    stopSimulation();
    this->tickRate = tickrate;
    startSimulation();
}

void Simulator::setWarehouseStateAt(int)
{
    throw std::runtime_error("Operation setWarehouseStateAt() is not supported");
}
