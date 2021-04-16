#ifndef STATE__H
#define STATE__H

#include "Agent.h"
#include "ChargingStation.h"
#include "DeliveryRobot.h"
#include "DeliveryStation.h"
#include "ObservableEnvironment.h"
#include "PodDock.h"
#include <memory>

// ######################## FORWARD DECLARATIONS ######################### //
// ###################################################################### //
class State
{
public:
    //Construct With Copy
    State(const std::shared_ptr<ObservableNavEnvironment> &environment,
          const std::vector<std::shared_ptr<ChargingStation>> &chStations,
          const std::vector<std::shared_ptr<DeliveryStation>> &deliveryStations,
          const std::vector<std::shared_ptr<PodDock>> &podDocks,
          const std::vector<std::shared_ptr<DeliveryRobot>> &robots,
          int rowCount, int colCount);

    //Construct With Move (preferred)
    State(std::shared_ptr<ObservableNavEnvironment> &&environment,
          std::vector<std::shared_ptr<ChargingStation>> &&chStations,
          std::vector<std::shared_ptr<DeliveryStation>> &&deliveryStations,
          std::vector<std::shared_ptr<PodDock>> &&podDocks,
          std::vector<std::shared_ptr<DeliveryRobot>> &&robots,
          int rowCount, int colCount);

    virtual ~State();

public:
    int getRowCount() const;
    int getColCount() const;
    const std::vector<std::shared_ptr<ChargingStation>> &getChargingStations() const;
    const std::vector<std::shared_ptr<PodDock>> &getPodDocks() const;
    const std::vector<std::shared_ptr<DeliveryStation>> &getDeliveryStations() const;
    const std::vector<std::shared_ptr<DeliveryRobot>> &getRobots() const;

public:
    void tick(int timeStamp);

private:
    size_t rowCount;
    size_t colCount;

    std::shared_ptr<ObservableNavEnvironment> environment;
    std::vector<std::shared_ptr<ChargingStation>> chStations;
    std::vector<std::shared_ptr<PodDock>> podDocks;
    std::vector<std::shared_ptr<DeliveryStation>> deliveryStations;
    std::vector<std::shared_ptr<DeliveryRobot>> robots;
};

#endif /* STATE__H */
