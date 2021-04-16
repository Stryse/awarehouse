#include "RackMotor.h"
#include "Body.h"
#include "MotorAction.h"
#include "MotorCommand.h"
#include "MotorDrive.h"
#include "Pod.h"
#include "PodHolder.h"
#include <stdexcept>

RackMotor::RackMotor(Body &body, IDepleting &energySource, PodHolder &podHolder)
    : AMotor(body),
      _pickUpPodAction(std::make_unique<MotorAction>(pickUpMotorDrive(body, *this), energySource, &onPodPickedUp)),
      _putDownPodAction(std::make_unique<MotorAction>(putDownMotorDrive(body, *this), energySource, &onPodPutDown)),
      podHolder(podHolder)
{
}

void RackMotor::activate(const MotorDirection &motorDirection)
{
    switch (motorDirection)
    {
    case MotorDirection::CLOCKWISE:
        AMotor::getBody().getEnvironment().getVolume(AMotor::getBody().getPose().getPosition())->receive(podHolder, PickupPodSignal());

        // Attach Pod's body if pod is present
        if (podHolder.getChildPod())
            AMotor::getBody().attachBody(*podHolder.getChildPod()->getBody());
        else
            throw std::runtime_error("PodPickUp action invoked where it is invalid. (Tile doesn't have a pod)");

        break;

    case MotorDirection::COUNTER_CLOCKWISE:

        AMotor::getBody().detachBody(*podHolder.getChildPod()->getBody());
        AMotor::getBody().getEnvironment().getVolume(AMotor::getBody().getPose().getPosition())->receive(podHolder.getChildPod(), PutDownPodSignal());

        if (podHolder.getChildPod())
            throw std::runtime_error("PodPutDown action invoked where it is invalid. (Tile doesn't accept pod");
        break;
    default:
        throw std::runtime_error("Unhandled enum in RackMotor::activate()");
        break;
    }
}

MotorAction *RackMotor::pickUpPodAction() const
{
    return _pickUpPodAction.get();
}

MotorAction *RackMotor::putDownPodAction() const
{
    return _putDownPodAction.get();
}

std::unique_ptr<MotorDrive> RackMotor::pickUpMotorDrive(Body &body, RackMotor &rackMotor)
{
    std::vector<MotorCommand> commands;
    commands.reserve(1);
    commands.emplace_back(rackMotor, MotorDirection::CLOCKWISE);
    return std::make_unique<MotorDrive>(body, std::move(commands), pickUpCost, pickUpDuration);
}

std::unique_ptr<MotorDrive> RackMotor::putDownMotorDrive(Body &body, RackMotor &rackMotor)
{
    std::vector<MotorCommand> commands;
    commands.reserve(1);
    commands.emplace_back(rackMotor, MotorDirection::COUNTER_CLOCKWISE);
    return std::make_unique<MotorDrive>(body, std::move(commands), putDownCost, putDownDuration);
}

/********************* Settings ***************************/
const int RackMotor::pickUpCost = 1;
const int RackMotor::pickUpDuration = 1;
const int RackMotor::putDownCost = 1;
const int RackMotor::putDownDuration = 1;
/**********************************************************/