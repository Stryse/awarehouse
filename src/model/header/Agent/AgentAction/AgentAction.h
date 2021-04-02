#ifndef AGENT_ACTION__H
#define AGENT_ACTION__H

#include <stdexcept>

namespace agent_action_util
{
    /***************************************************************************
     * @brief Thrown when an agent is unable to perform the associated action.
     ***************************************************************************/
    class ActionFailedException : public std::runtime_error
    {
    public:
        explicit ActionFailedException(const char *message) : std::runtime_error(message) {}
    };
};

/*******************************************************************************
 * @brief Agents can perform different kinds of actions during their lifetimes.
 * Actions are added to the agent's action queue which is polled each tick and
 * if the agent's controll mechanism decides it can start to perform the action.
 *
 * Each action has a duration which indicates the amount of time needed to execute it.
 * When an agent starts to execute the action, it automatically progresses, if
 * the full progress is achieved then the action is executed.
 *******************************************************************************/
class AgentAction
{
public:
    using ActionFailedException = agent_action_util::ActionFailedException;

protected:
    explicit AgentAction(int duration)
        : duration(duration), progress(0)
    {
    }

public:
    void operator()()
    {
        if (!canExecute())
            throw ActionFailedException("Agent action failed");

        if (progress < duration)
        {
            ++progress;
        }

        if (progress >= duration)
            action();
    }

    int getProgress() const { return progress; }
    int getDuration() const { return duration; }

protected:
    virtual bool canExecute() const = 0;
    virtual void action() = 0;

private:
    const int duration;
    int progress;
};

#endif /* AGENT_ACTION__H */