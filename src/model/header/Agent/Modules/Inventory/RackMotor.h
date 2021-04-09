#ifndef RACK_MOTOR__H
#define RACK_MOTOR__H

#include "AMotor.h"
#include "IDepleting.h"
#include "LibConfig.h"
#include "MotorAction.h"
#include "MotorCommand.h"
#include "MotorDrive.h"
#include <memory>
#include <stdexcept>
#include <vector>

template <typename TBody, typename TEnergy = config::agent::DefaultEnergy>
class RackMotor : public AMotor<TBody>
{
public:
    using Body = TBody;
    using Energy = TEnergy;
    using IDepleting = ::IDepleting<Energy>;
    using AMotor = ::AMotor<Body>;
    using MotorDirection = typename AMotor::MotorDirection;
    using MotorAction = ::MotorAction<Body, Energy>;
    using MotorCommand = ::MotorCommand<AMotor>;
    using MotorDrive = ::MotorDrive<AMotor, Energy>;

public:
    explicit RackMotor(Body &body, IDepleting &energySource)
        : AMotor(body),
          _pickUpPodAction(std::make_unique<MotorAction>(pickUpMotorDrive(body, *this), energySource)),
          _putDownPodAction(std::make_unique<MotorAction>(putDownMotorDrive(body, *this), energySource))
    {
    }

    // AMotor implementation
    virtual void activate(const MotorDirection &motorDirection) override
    {
        switch (motorDirection)
        {
        case MotorDirection::CLOCKWISE:
            break;

        case MotorDirection::COUNTER_CLOCKWISE:
            break;

        default:
            throw std::runtime_error("Unhandled enum in RackMotor::activate()");
            break;
        }
    }

public:
    MotorAction *pickUpPodAction()
    {
        return _pickUpPodAction.get();
    }

    MotorAction *putDownPodAction()
    {
        return _putDownPodAction.get();
    }

public:
    static std::unique_ptr<MotorDrive> pickUpMotorDrive(Body &body, RackMotor &rackMotor)
    {
        std::vector<MotorCommand> commands;
        commands.reserve(1);
        commands.emplace_back(rackMotor, MotorDirection::CLOCKWISE);
        return std::make_unique<MotorDrive>(body, std::move(commands), pickUpCost, pickUpDuration);
    }

    static std::unique_ptr<MotorDrive> putDownMotorDrive(Body &body, RackMotor &rackMotor)
    {
        std::vector<MotorCommand> commands;
        commands.reserve(1);
        commands.emplace_back(rackMotor, MotorDirection::COUNTER_CLOCKWISE);
        return std::make_unique<MotorDrive>(body, std::move(commands), putDownCost, putDownDuration);
    }

private:
    std::unique_ptr<MotorAction> _pickUpPodAction;
    std::unique_ptr<MotorAction> _putDownPodAction;

    static const Energy pickUpCost;
    static const int pickUpDuration;

    static const Energy putDownCost;
    static const int putDownDuration;
};

/********************* Settings ***************************/
template <typename TBody, typename TEnergy>
const TEnergy RackMotor<TBody, TEnergy>::pickUpCost = 1;

template <typename TBody, typename TEnergy>
const int RackMotor<TBody, TEnergy>::pickUpDuration = 1;

template <typename TBody, typename TEnergy>
const TEnergy RackMotor<TBody, TEnergy>::putDownCost = 1;

template <typename TBody, typename TEnergy>
const int RackMotor<TBody, TEnergy>::putDownDuration = 1;
/**********************************************************/

#endif /* RACK_MOTOR__H */