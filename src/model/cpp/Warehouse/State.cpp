#include "State.h"
#include "Agent.h"
#include "Tile.h"
#include <QJsonObject>
#include <iostream>

State::State()
    : environment(8, 8, 2)
{
}

void State::tick()
{
    std::cerr << "State::tick()" << std::endl;
    //for (auto &agent : agents)
    //agent->tick(1);
}

State::~State() {}
