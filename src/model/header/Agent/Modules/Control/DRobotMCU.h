#ifndef D_ROBOT_MCU__H
#define D_ROBOT_MCU__H

#include "AMicroController.h"
#include "NetworkAdapter.h"
#include <queue>

// ############################ FORWARD DECLARATIONS ##############################
class AgentAction;
// ################################################################################
class DRobotMCU : public AMicroController
{
public:
    virtual void tick(int time) override
    {
    }

private:
    NetworkAdapter networkAdapter;
    std::queue<AgentAction *> actionQueue;
};

#endif