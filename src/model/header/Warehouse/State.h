#ifndef STATE__H
#define STATE__H

#include "ObservableEnvironment.h"
#include "DeliveryRobot.h"
#include "Agent.h"
#include "PodDock.h"
#include "ChargingStation.h"
#include <memory>

// ######################## FORWARD DECLARATIONS ######################### //
// ###################################################################### //
class State
{
public:
    State();
    virtual ~State();

public:
    void tick();

private:
    std::shared_ptr<ObservableNavEnvironment<>> environment;
};

#endif /* STATE__H */
