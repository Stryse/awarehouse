#include "MotorAction.h"
#include "Body.h"
#include "MotorDrive.h"

MotorAction::MotorAction(std::unique_ptr<MotorDrive> &&motorDrive,
                         IDepleting &resource,
                         const boost::signals2::signal<void(const Body &)> *event)

    : DepletingAction(resource, motorDrive->getEnergySum(), motorDrive->getTimeSum()),
      motorDrive(std::move(motorDrive)), event_(event)
{
}

MotorAction::~MotorAction() {}

void MotorAction::depletingAction()
{
    motorDrive->executeMovement();

    if (event_)
        (*event_)(motorDrive->getBody());
}

bool MotorAction::canExecute() const
{
    return true;
}