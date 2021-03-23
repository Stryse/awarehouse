#include "State.h"
#include <iostream>

State::State()
{
}

void State::tick()
{
    std::cerr << "State::tick()" << std::endl;
    //for (auto &agent : agents)
    //agent->tick(1);
}

State::~State() {}
