#ifndef A_MICRO_CONTROLLER__H
#define A_MICRO_CONTROLLER__H

#include <sstream>
#include <string>
#include <vector>

/*******************************************************************************
 * @brief Part of an agent which is reponsible for control and decision making.
 * Each tick it makes a decision or polls it from a central controller.
 *******************************************************************************/
class AMicroController
{
public:
    virtual void tick(int time) = 0;
};

#endif