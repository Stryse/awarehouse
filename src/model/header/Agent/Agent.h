#ifndef AGENT__H
#define AGENT__H

#include "AMicroController.h"
#include "Body.h"
#include "LibConfig.h"
#include <memory>
#include <string>

// ######################## Forward Declarations #########################
class AMicroController;
// #######################################################################
namespace agent_util
{
    class Configuration
    {
    };
};

/*********************************************************************************
 * @brief An entity which is to perform tasks in an environment either controlled
 * by a central controller or acting autonomously.
 * 
 * @tparam TEnvironment Type of environment.
 *********************************************************************************/
template <typename TEnvironment = config::navigation::DefaultEnvironment>
class Agent
{
public:
    using Configuration = agent_util::Configuration;

    /*********************************************************
     * @brief Type of the environment in which the agent acts
     *********************************************************/
    using Environment = TEnvironment;

public:
    /*********************************************************
     * @brief Construct a new Agent object
     *********************************************************/
    explicit Agent(const std::string &id_category,
                   Environment &environment,
                   std::unique_ptr<Body<Environment>> body,
                   std::unique_ptr<AMicroController> mcu)

        : environment(environment),
          body(std::move(body)),
          mcu(std::move(mcu)),
          serialNumber(getNewSerialNumber()),
          id(std::string(getNewId(id_category)))
    {
    }

public:
    /*********************************************************
     * @brief Signal that indicates, that time elapsed
     * and the agent might need to act.
     *********************************************************/
    void tick(int timeStamp) { mcu->tick(timeStamp); }

    /*********************************************************
     * @brief 
     *********************************************************/
    const std::string &getId() const { return id; }

    /*********************************************************
     * @brief Get the Serial Number object.
     *********************************************************/
    int getSerialNumber() const { return serialNumber; }

private:
    /*********************************************************
     * @brief Used to acquire a serial number each time an
     * agent is constructed.
     *********************************************************/
    static int getNewSerialNumber() { return Agent<Environment>::newSerialNumber++; }

    /*********************************************************
     * @brief Used to acquire a new agent ID based on category
     * with appropriate serial number.
     *********************************************************/
    static std::string getNewId(const std::string &id_category)
    {
        return std::string("[AGENT]{") +
               std::string(id_category) +
               std::string("}_");
    }

protected:
    /******************************************************
     * @brief Environment in which the agent acts.
     ******************************************************/
    Environment &environment;

    /******************************************************
     * @brief The agent's physical body,
     * has position,orientation,shape etc.
     ******************************************************/
    std::unique_ptr<Body<Environment>> body;

    /******************************************************
     * @brief A mechanism controlling the agent's modules
     * eg. motors  -> movement
     *     sensors -> observing
     *     communications -> message receiving and dispatch
     ******************************************************/
    std::unique_ptr<AMicroController> mcu;

private:
    /******************************************************
     * @brief Represents the creation number of the agent
     ******************************************************/
    const int serialNumber;

    /******************************************************
     * @brief A unique ID identifying the agent.
     * Format: [AGENT]{CATEGORY}_SERIALID
     ******************************************************/
    const std::string id;

    /******************************************************
     * @brief Incremented each time an agent is constructed.
     ******************************************************/
    static int newSerialNumber;
};

template <typename Environment>
int Agent<Environment>::newSerialNumber = 0x0;
#endif /* AGENT__H */