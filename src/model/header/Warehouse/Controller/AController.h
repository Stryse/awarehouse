#ifndef A_CONTROLLER__H
#define A_CONTROLLER__H

class AController
{
public:
    virtual void tick() = 0;
    virtual void doActionAssignment() = 0;
};

#endif