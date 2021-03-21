#ifndef CONTROLLER_IMPL__H
#define CONTROLLER_IMPL__H

#include "AController.h"

class ControllerImpl : public AController
{
public:
    virtual void tick() override {}
    virtual void doActionAssignment() override {}
};

#endif /* CONTROLLER_IMPL__H */