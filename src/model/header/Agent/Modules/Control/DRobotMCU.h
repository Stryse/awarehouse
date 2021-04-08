#ifndef D_ROBOT_MCU__H
#define D_ROBOT_MCU__H

#include "AMicroController.h"
#include "NetworkAdapter.h"
#include <queue>

// ############################ FORWARD DECLARATIONS ##############################
class AgentAction;
// ################################################################################

/**********************************************************************************
 * @brief Micro Controller Unit of a DeliveryRobot.
 * This module is responsible for the DeliveryRobot's decision making.
 * Each Tick it does the following:
 * - Refresh Sensor Data.
 * - Refresh messages coming from a network. (eg. Instructions from a Controller)
 * - Polls action queue
 * - Performs the polled action.
 *
 **********************************************************************************/
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
