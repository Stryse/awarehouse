#include "Simulator.h"
#include "Warehouse.h"
#include <stdexcept>

Simulator::Simulator(QObject* parent)
    : QObject(parent), isRunning(false), warehouse(nullptr), timer(this), tickRate(1000)
{
    timer.setInterval(tickRate);
    timer.callOnTimeout([=]() {
        warehouse->tick();
        ISimulator<Warehouse>::onTick();
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

void Simulator::setTickRate(int tickrate)
{
    bool wasRunning = isRunning;

    if(wasRunning)
        stopSimulation();

    this->tickRate = tickrate;
    timer.setInterval(tickRate);

    if(wasRunning)
        startSimulation();
}

void Simulator::setWarehouseStateAt(int)
{
    throw std::runtime_error("Operation setWarehouseStateAt() is not supported");
}
