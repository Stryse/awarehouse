#include "State.h"
#include <iostream>

State::State(const std::shared_ptr<ObservableNavEnvironment<> >& environment,
             const std::vector<std::shared_ptr<ChargingStation<> > >& chStations,
             const std::vector<std::shared_ptr<PodDock<> > >& podDocks,
             const std::vector<std::shared_ptr<DeliveryRobot<> > >& robots,
             int rowCount, int colCount)

    : rowCount(rowCount),colCount(colCount),
      environment(environment),
      chStations(chStations),
      podDocks(podDocks),
      robots(robots)
{

}

State::State(std::shared_ptr<ObservableNavEnvironment<> >&& environment,
             std::vector<std::shared_ptr<ChargingStation<> > >&& chStations,
             std::vector<std::shared_ptr<PodDock<> > >&& podDocks,
             std::vector<std::shared_ptr<DeliveryRobot<> > >&& robots,
             int rowCount,int colCount)

    : rowCount(rowCount), colCount(colCount),
      environment(std::move(environment)),
      chStations(std::move(chStations)),
      podDocks(std::move(podDocks)),
      robots(std::move(robots))
{

}

State::~State() {}


void State::tick()
{
    std::cerr << "State::tick()" << std::endl;
    //for (auto &agent : agents)
    //agent->tick(1);
}


int State::getRowCount() const { return rowCount; }
int State::getColCount() const { return colCount; }

const std::vector<std::shared_ptr<ChargingStation<>>>& State::getChargingStations() const { return chStations; }
const std::vector<std::shared_ptr<PodDock<>>>& State::getPodDocks() const { return podDocks; }
const std::vector<std::shared_ptr<DeliveryRobot<> > >& State::getRobots() const { return robots; }
