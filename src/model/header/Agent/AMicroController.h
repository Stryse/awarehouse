#ifndef A_MICRO_CONTROLLER__H
#define A_MICRO_CONTROLLER__H

#include <sstream>
#include <string>
#include <vector>

class AMicroController
{
public:
    virtual void tick(int time) = 0;

private:
};

#endif