#include "SimulatorTest.h"
#include "Simulator.h"
#include "Warehouse.h"
#include <QTimer>
#include <QtTest/qtest.h>
#include <memory>

void SimulatorTest::toUpper()
{
    std::unique_ptr<Warehouse> warehouse = nullptr;
    warehouse = std::make_unique<Warehouse>();

    std::unique_ptr<ISimulator<Warehouse>> simulator = nullptr;
    simulator = std::make_unique<Simulator>();

    Simulator *sim = (Simulator *)simulator.get();
    simulator->bindSimTypeData(warehouse.get());

    // Thread stufff
    QThread thread;
    thread.start();
    sim->moveToThread(&thread);
    //

    QMetaObject::invokeMethod(sim, "startSimulation");
    thread.wait(3000);
    QMetaObject::invokeMethod(sim, "stopSimulation");
    thread.wait(3000);
    QMetaObject::invokeMethod(sim, "startSimulation");
    thread.wait(3000);
    QMetaObject::invokeMethod(sim, "stopSimulation");
    thread.wait(1000);
    thread.terminate();
}
QTEST_MAIN(SimulatorTest)
