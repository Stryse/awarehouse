#include "Agent.h"
#include "AMicroController.h"
#include "Body.h"

Agent::Agent(const std::string &id_category,
             const std::shared_ptr<ObservableNavEnvironment> &environment,
             const std::shared_ptr<Body> &body,
             std::unique_ptr<AMicroController> &&mcu)

    : environment(environment),
      body(body),
      mcu(std::move(mcu)),
      id(std::string(getNewId(id_category))),
      serialNumber(getNewSerialNumber())
{
}

Agent::~Agent()
{
}

void Agent::tick(int timeStamp)
{
    mcu->tick(timeStamp);
}

const std::string &Agent::getId() const { return id; }

int Agent::getSerialNumber() const { return serialNumber; }

const std::shared_ptr<Body> &Agent::getBody() const { return body; }

int Agent::getNewSerialNumber() { return Agent::newSerialNumber++; }

std::string Agent::getNewId(const std::string &id_category)
{
    return std::string("[AGENT]{") +
           std::string(id_category) +
           std::string("}_") +
           std::to_string(Agent::newSerialNumber);
}

int Agent::newSerialNumber = 0x0;