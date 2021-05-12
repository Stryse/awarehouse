#ifndef A_MICRO_CONTROLLER__H
#define A_MICRO_CONTROLLER__H

#include <sstream>
#include <string>
#include <vector>
#include <boost/signals2.hpp>

/*******************************************************
* @brief Agent makes decisions based on status
*******************************************************/
enum class Status
{
    IDLE,
    RUNNING,
    CHARGING,
    ERROR
};

/*******************************************************************************
 * @brief Part of an agent which is reponsible for control and decision making.
 * Each tick it makes a decision or polls it from a central controller.
 *******************************************************************************/
class AMicroController
{
public:

    /*************************************************************
     * @brief MCU makes decision each timestamp (when tick called)
     *************************************************************/
    virtual void tick(int time) = 0;

    /*************************************************************
     * @brief Returns the Status of the Agent.
     *************************************************************/
    virtual const Status& getStatus() const = 0;

    /*************************************************************
     * @brief Emitted when status changed.
     * @return The new status
     *************************************************************/
    mutable boost::signals2::signal<void(const Status&)> onStatusChanged;
};

#endif