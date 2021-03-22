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
class State : public ISerializable<QJsonObject>
{
public:
    State();
    virtual ~State();

public:
    void tick();

public:
    virtual void read(const QJsonObject &json) override;
    virtual void write(QJsonObject &json) const override;

private:
    ObservableNavEnvironment<Tile> environment;
    std::vector<std::unique_ptr<Agent<ObservableNavEnvironment<Tile>>>> agents;
};

#endif /* STATE__H */
