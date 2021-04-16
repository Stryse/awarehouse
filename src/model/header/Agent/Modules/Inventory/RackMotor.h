#ifndef RACK_MOTOR__H
#define RACK_MOTOR__H

#include "AMotor.h"
#include "AgentSignals.h"
#include "IDepleting.h"
#include "MotorAction.h"
#include "boost/signals2.hpp"
#include <memory>
#include <vector>
// ########################## FORWARD DECLARATIONS ###############################
class PodHolder;
class Body;
class MotorDrive;
// ###############################################################################
/*********************************************************************************
 * @brief A Motor that is able to PickUp order from the environment and place it
 * in an associated PodHolder.
 * 
 * It can also put down a pod from the holder in the environment.
 * It can only put down pods to tiles which accepts the pod.
 *********************************************************************************/
class RackMotor : public AMotor
{
public:
    using MotorDirection = AMotor::MotorDirection;

public:
    boost::signals2::signal<void(const Body &)> onPodPickedUp;
    boost::signals2::signal<void(const Body &)> onPodPutDown;

public:
    explicit RackMotor(Body &body, IDepleting &energySource, PodHolder &podHolder);
    explicit RackMotor(RackMotor &&other) = default;
    virtual ~RackMotor() = default;

    explicit RackMotor(const RackMotor &other) = delete;
    RackMotor &operator=(const RackMotor &other) = delete;

    // ################################### AMotor implementation #######################################
    /***************************************************************************************************
     * @brief The RackMotor's two configurations are for picking up and putting down pods.
     * Configuration:
     * 
     * CLOCKWISE         -> Picks up pod from the environment.
     * COUNTER_CLOCKWISE -> Puts down pod into the environment.
     ***************************************************************************************************/
    virtual void activate(const MotorDirection &motorDirection) override;

public:
    /************************************************************************
     * @brief A motor action that instructs the RackMotor to pick up a pod
     ************************************************************************/
    MotorAction *pickUpPodAction() const;

    /*************************************************************************
     * @brief A motor action that instructs the RackMotor to put down the pod
     *************************************************************************/
    MotorAction *putDownPodAction() const;

public:
    /**************************************************************************
     * @brief A Motor Drive that instructs the Rackmotor to run CLOCKWISE
     * which leads it to pick up a pod.
     **************************************************************************/
    static std::unique_ptr<MotorDrive> pickUpMotorDrive(Body &body, RackMotor &rackMotor);

    /************************************************************************************
     * @brief A Motor Drive that instructs the Rackmotor to run COUNTER_CLOCKWISE
     * which leads it to put down a pod.
     ************************************************************************************/
    static std::unique_ptr<MotorDrive> putDownMotorDrive(Body &body, RackMotor &rackMotor);

private:
    std::unique_ptr<MotorAction> _pickUpPodAction;
    std::unique_ptr<MotorAction> _putDownPodAction;
    PodHolder &podHolder;

    static const int pickUpCost;
    static const int pickUpDuration;

    static const int putDownCost;
    static const int putDownDuration;
};
#endif /* RACK_MOTOR__H */
