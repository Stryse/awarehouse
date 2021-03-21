#ifndef AGENT_ACTION__H
#define AGENT_ACTION__H

#include <stdexcept>

namespace agent_action_util
{
    class ActionFailedException : public std::runtime_error
    {
    public:
        explicit ActionFailedException(const char *message) : std::runtime_error(message) {}
    };
};

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

#endif