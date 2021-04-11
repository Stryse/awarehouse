#ifndef SCHEDULER_IMPL__H
#define SCHEDULER_IMPL__H

#include "AScheduler.h"
#include "NetworkAdapter.h"

class SchedulerImpl : public AScheduler
{
public:
    SchedulerImpl();
    virtual ~SchedulerImpl();

public:
    virtual void tick(int timeStamp) override;
    virtual void doTaskAssignment() override;
    virtual const NetworkAdapter &getNetworkAdapter() const override;
    virtual NetworkAdapter &getNetworkAdapter() override;

private:
    NetworkAdapter networkAdapter;
};

#endif /* SCHEDULER_IMPL__H */
