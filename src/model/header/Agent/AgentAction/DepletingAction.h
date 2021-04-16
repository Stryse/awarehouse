#ifndef DEPLETING_ACTION__H
#define DEPLETING_ACTION__H

#include "AgentAction.h"
#include "IDepleting.h"
#include <functional>

/********************************************************************************
 * @brief A kind of Agent Action that needs energy to be performed.
 * It is neccessary to provide an Energy resource of IDepleting interface to
 * consume the amount of energy needed.
 *
 ********************************************************************************/
class DepletingAction : public AgentAction
{
public:
    using EnergyDepletedException = typename IDepleting::EnergyDepletedException;

public:
    DepletingAction(IDepleting &resource, const int &energyCost, int duration);
    virtual ~DepletingAction();
    explicit DepletingAction(const DepletingAction &other) = delete;
    explicit DepletingAction(DepletingAction &&other) = delete;
    DepletingAction &operator=(const DepletingAction &other) = delete;

protected:
    virtual void action() override;
    virtual bool canExecute() const override = 0;
    virtual void depletingAction() = 0;

protected:
    std::reference_wrapper<IDepleting> resource;
    int energyCost;
};

#endif /* DEPLETING_ACTION__H */
