#ifndef D_ROBOT_MCU__H
#define D_ROBOT_MCU__H

#include "AMicroController.h"

class DRobotMCU : public AMicroController
{
public:
    virtual void tick(int time) override
    {
    }
};

#endif