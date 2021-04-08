#ifndef CONTROLLER_IMPL__H
#define CONTROLLER_IMPL__H

#include "AController.h"
#include "NetworkAdapter.h"

class ControllerImpl : public AController
{
public:
    ControllerImpl();
    virtual ~ControllerImpl();
    
public:
    virtual void tick() override;
    virtual void doActionAssignment() override;

    const NetworkAdapter& getNetworkAdapter() const override;
    NetworkAdapter& getNetworkAdapter() override;

private:
    NetworkAdapter networkAdapter;
};

#endif /* CONTROLLER_IMPL__H */