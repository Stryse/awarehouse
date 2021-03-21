#ifndef A_SCHEDULER__H
#define A_SCHEDULER__H

class AScheduler
{
public:
    virtual void tick() = 0;
    virtual void doTaskAssignment() = 0;
};

#endif /* A_SHCEDULER__H */