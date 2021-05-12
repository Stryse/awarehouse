#ifndef AGENT__H
#define AGENT__H

#include <memory>
#include <string>

// ######################## Forward Declarations #########################
class AMicroController;
class Body;
class ObservableNavEnvironment;
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
class Agent
{
public:
    using Configuration = agent_util::Configuration;

public:
    /*********************************************************
     * @brief Construct a new Agent subclass object.
     *
     * @param id_category Should be the name of the subclass
     * @param environment Shared pointer of the environment.
     * (Since agent has a body which leaves environment on
     * destruction, it needs to keep the environment alive
     * with a resource sharing.
     * @param body Agent's physical form.
     * @param mcu A subclass of AMicroController.
     *********************************************************/
    explicit Agent(const std::string &id_category,
                   const std::shared_ptr<ObservableNavEnvironment> &environment,
                   const std::shared_ptr<Body> &body,
                   std::unique_ptr<AMicroController> &&mcu);

    explicit Agent(const Agent &other) = delete;
    explicit Agent(Agent &&other) = delete;
    virtual ~Agent();
    Agent &operator=(const Agent &other) = delete;

public:
    /*********************************************************
     * @brief Signal that indicates, that time elapsed
     * and the agent might need to act.
     *********************************************************/
    void tick(int timeStamp);

    /*********************************************************
     * @brief Returns the agent's unique identifier
     *********************************************************/
    const std::string &getId() const;

    /*********************************************************
     * @brief Returns the Agent's serial number (creation number)
     *********************************************************/
    int getSerialNumber() const;

    /*********************************************************
     * @brief Returns the agent's body
     *********************************************************/
    const std::shared_ptr<Body> &getBody() const;

    /*********************************************************
     * @brief Returns the Micro Controller Unit of the Agent
     *********************************************************/
    const std::unique_ptr<AMicroController>& getMCU() const;

private:
    /*********************************************************
     * @brief Used to acquire a serial number each time an
     * agent is constructed.
     *********************************************************/
    static int getNewSerialNumber();

    /*********************************************************
     * @brief Used to acquire a new agent ID based on category
     * with appropriate serial number.
     *********************************************************/
    static std::string getNewId(const std::string &id_category);

protected:
    /******************************************************
     * @brief Environment in which the agent acts.
     ******************************************************/
    std::shared_ptr<ObservableNavEnvironment> environment;

    /******************************************************
     * @brief The agent's physical body,
     * has position,orientation,shape etc.
     ******************************************************/
    std::shared_ptr<Body> body;

    /******************************************************
     * @brief A mechanism controlling the agent's modules,
     * each tick it makes a decision.
     * eg. motors  -> movement
     *     sensors -> observing
     *     communications -> message receiving and dispatch
     ******************************************************/
    std::unique_ptr<AMicroController> mcu;

private:
    /******************************************************
     * @brief A unique ID identifying the agent.
     * Format: [AGENT]{CATEGORY}_SERIALID
     ******************************************************/
    const std::string id;

    /******************************************************
     * @brief Represents the creation number of the agent
     ******************************************************/
    const int serialNumber;

    
public:
    /******************************************************
     * @brief Incremented each time an agent is constructed.
     ******************************************************/
    static int newSerialNumber;
};
#endif /* AGENT__H */
