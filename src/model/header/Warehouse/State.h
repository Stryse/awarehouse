#ifndef STATE__H
#define STATE__H

#include "ObservableEnvironment.h"
#include <memory>
#include <vector>

// ######################## FORWARD DECLARATIONS ######################### //
class Tile;
template <typename Environment>
class Agent;
class QJsonObject;
// ###################################################################### //
class State
{
public:
    State();
    virtual ~State();

public:
    void tick();

private:
    ObservableNavEnvironment<Tile> environment;
    std::vector<std::unique_ptr<Agent<ObservableNavEnvironment<Tile>>>> agents;
};

#endif /* STATE__H */
