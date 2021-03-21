#ifndef SCHEDULER_IMPL__H
#define SCHEDULER_IMPL__H

#include "AScheduler.h"

class SchedulerImpl : public AScheduler
{
public:
    virtual void tick() override {}
    virtual void doTaskAssignment() override {}
};

#endif /* SCHEDULER_IMPL__H */
