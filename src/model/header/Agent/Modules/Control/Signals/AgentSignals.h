#ifndef AGENT_SIGNALS__H
#define AGENT_SIGNALS__H

/*******************************************************
 * @brief Agents can send signals to IAgentSignalHandler
 * subclasses. Signals may or may not carry additional
 * arguments.
 *
 *******************************************************/
struct AgentSignal
{
};

/*******************************************************
 * @brief Sent when agent request a charge from a charger
 * entity.
 *******************************************************/
struct ChargeSignal : public AgentSignal
{
};

/*******************************************************
 * @brief Sent when agent request to pick up a pod from
 * a pod containing entity
 *******************************************************/
struct PickupPodSignal : public AgentSignal
{
};

/*******************************************************
 * @brief Sent when agent request to put down a pod to
 * a pod containing entity
 *******************************************************/
struct PutDownPodSignal : public AgentSignal
{
};

#endif
