#ifndef RACK_MOTOR__H
#define RACK_MOTOR__H

#include "AMotor.h"
#include "AgentSignals.h"
#include "IDepleting.h"
#include "LibConfig.h"
#include "MotorAction.h"
#include "MotorCommand.h"
#include "MotorDrive.h"
#include "PodHolder.h"
#include "boost/signals2.hpp"
#include <memory>
#include <stdexcept>
#include <vector>

/*********************************************************************************
 * @brief A Motor that is able to PickUp order from the environment and place it
 * in an associated PodHolder.
 * 
 * It can also put down a pod from the holder in the environment.
 * It can only put down pods to tiles which accepts the pod.
 *********************************************************************************/
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
    using PodHolder = ::PodHolder<typename Body::Environment>;

public:
    boost::signals2::signal<void(const Body &)> onPodPickedUp;
    boost::signals2::signal<void(const Body &)> onPodPutDown;

public:
    explicit RackMotor(Body &body, IDepleting &energySource, PodHolder &podHolder)
        : AMotor(body),
          _pickUpPodAction(std::make_unique<MotorAction>(pickUpMotorDrive(body, *this), energySource, &onPodPickedUp)),
          _putDownPodAction(std::make_unique<MotorAction>(putDownMotorDrive(body, *this), energySource, &onPodPutDown)),
          podHolder(podHolder)
    {
    }

    // ################################### AMotor implementation #######################################
    /***************************************************************************************************
     * @brief The RackMotor's two configurations are for picking up and putting down pods.
     * Configuration:
     * 
     * CLOCKWISE         -> Picks up pod from the environment.
     * COUNTER_CLOCKWISE -> Puts down pod into the environment.
     ***************************************************************************************************/
    virtual void activate(const MotorDirection &motorDirection) override
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

public:
    /************************************************************************
     * @brief A motor action that instructs the RackMotor to pick up a pod
     ************************************************************************/
    MotorAction *pickUpPodAction()
    {
        return _pickUpPodAction.get();
    }

    /*************************************************************************
     * @brief A motor action that instructs the RackMotor to put down the pod
     *************************************************************************/
    MotorAction *putDownPodAction()
    {
        return _putDownPodAction.get();
    }

public:
    /**************************************************************************
     * @brief A Motor Drive that instructs the Rackmotor to run CLOCKWISE
     * which leads it to pick up a pod.
     **************************************************************************/
    static std::unique_ptr<MotorDrive> pickUpMotorDrive(Body &body, RackMotor &rackMotor)
    {
        std::vector<MotorCommand> commands;
        commands.reserve(1);
        commands.emplace_back(rackMotor, MotorDirection::CLOCKWISE);
        return std::make_unique<MotorDrive>(body, std::move(commands), pickUpCost, pickUpDuration);
    }

    /************************************************************************************
     * @brief A Motor Drive that instructs the Rackmotor to run COUNTER_CLOCKWISE
     * which leads it to put down a pod.
     ************************************************************************************/
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
    PodHolder &podHolder;

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
