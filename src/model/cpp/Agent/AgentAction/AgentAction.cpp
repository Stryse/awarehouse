#include "AgentAction.h"

AgentAction::AgentAction(int duration)
    : duration(duration), progress(0)
{
}

AgentAction::~AgentAction()
{
}

void AgentAction::operator()()
{
    if (!canExecute())
        throw ActionFailedException("Agent action failed");

    if (progress < duration)
        ++progress;

    if (progress >= duration)
        action();
}

int AgentAction::getProgress() const { return progress; }
int AgentAction::getDuration() const { return duration; }