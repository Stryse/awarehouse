#ifndef I_SIMULATOR__H
#define I_SIMULATOR__H

#include "boost/signals2.hpp"

/****************************************************************
 * @brief An interface which can be bound to a changing state.
 * @tparam SimType the type of which contains the state altering
 * method.
 * 
 * ISimulator emits a onTick signal after each state transition.
 ****************************************************************/
template <typename SimType>
class ISimulator
{

public:
    virtual ~ISimulator() = default;
    virtual bool isAvailable() const = 0;

    /*************************************************************************
     * @brief Sets the data which can be altered periodically by the Simulator
     *************************************************************************/
    virtual void bindSimTypeData(SimType *) = 0;

    /***************************************
     * @brief Starts the simulation process
     ***************************************/
    virtual void startSimulation() = 0;

    /****************************************
     * @brief Stops the simulation process
     ****************************************/
    virtual void stopSimulation() = 0;

    /******************************************
     * @brief Sets the tick rate (speed) of the
     * simulation process.
     ******************************************/
    virtual void setTickRate(int tickRate) = 0;

    /**************************************************
     * @brief Sets the simulatiion state at a provided
     * time stamp.
     **************************************************/
    virtual void setWarehouseStateAt(int timeStamp) = 0;

public:
    mutable boost::signals2::signal<void()> onTick;

};
#endif /* I_SIMULATOR__H */
