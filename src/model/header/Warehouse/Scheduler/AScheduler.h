#ifndef A_SCHEDULER__H
#define A_SCHEDULER__H

#include "NetworkAdapter.h"

// ########################## FORWARD DECLARATIONS ##########################
class TaskManager;
// ##########################################################################

class AScheduler
{
public:
    AScheduler(TaskManager *taskManager = nullptr)
        : taskManager(taskManager) {}

public:
    virtual void tick(int timeStamp) = 0;

    const NetworkAdapter &getNetworkAdapter() const { return networkAdapter; }
    NetworkAdapter &getNetworkAdapter() { return networkAdapter; }
    void setTaskManager(TaskManager *taskManager) { this->taskManager = taskManager; }

protected:
    TaskManager *taskManager;
    NetworkAdapter networkAdapter;
};

#endif /* A_SHCEDULER__H */