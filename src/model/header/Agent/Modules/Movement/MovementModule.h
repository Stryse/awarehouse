#ifndef MOVEMENT_MODULE__H
#define MOVEMENT_MODULE__H

#include "IMoveMechanism.h"
#include <memory>

/*******************************************************
 * @brief A module which contains a move mechanism.
 * An agent can derive from this class so it can use
 * and forward a move mechanism.
 * A move mechanism enables an agent to move by itself
 * independently.
 *******************************************************/
class MovementModule
{
public:
    explicit MovementModule(std::unique_ptr<IMoveMechanism> &&moveMechanism)
        : moveMechanism(std::move(moveMechanism))
    {
    }

public:
    // GETTER
    const IMoveMechanism *getMoveMechanism() const { return moveMechanism.get(); }
    IMoveMechanism *getMoveMechanism() { return moveMechanism.get(); }

    // SETTER
    void setMoveMechanism(std::unique_ptr<IMoveMechanism> &&moveMechanism) { this->moveMechanism = std::move(moveMechanism); }

private:
    std::unique_ptr<IMoveMechanism> moveMechanism;
};

#endif /* ROBOT_MOVEMENT_MODULE__H */