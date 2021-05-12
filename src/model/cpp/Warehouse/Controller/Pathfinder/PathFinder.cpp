#include "PathFinder.h"
#include "IMoveMechanism.h"
#include "State.h"
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>

PathFinder::PathFinder(const State &state)

    : ROWS(state.getRowCount()),
      COLS(state.getColCount()),
      reservationTable{}
{
    reservationTable.reserve(10000);

    //################################ Setup static obstacles #################################
    //####################################### PodDocks ########################################
    for (auto &podDock : state.getPodDocks())
    {
        hardStaticObstacles.emplace(std::make_pair(podDock->getPosition().getPosX(),
                                                   podDock->getPosition().getPosY()));
    }
    //#################################  Charging Stations ####################################
    for (auto &ChargingStation : state.getChargingStations())
    {
        softStaticObstacles.emplace(std::make_pair(ChargingStation->getPosition().getPosX(),
                                                   ChargingStation->getPosition().getPosY()));

        hardStaticObstacles.emplace(std::make_pair(ChargingStation->getPosition().getPosX(),
                                                   ChargingStation->getPosition().getPosY()));
    }
    //################################## Delivery Stations ####################################
    for (auto &DeliveryStation : state.getDeliveryStations())
    {
        hardStaticObstacles.emplace(std::make_pair(DeliveryStation->getPosition().getPosX(),
                                                   DeliveryStation->getPosition().getPosY()));

        //softStaticObstacles.emplace(std::make_pair(DeliveryStation->getPosition().getPosX(),
                                                   //DeliveryStation->getPosition().getPosY()));
    }
    //############################## Setup Semi-static Obstacles ##############################
    /******************************************************************************************
    * Robot Locations are initially semi-static obstacles
    * from time 0 to Infinity. Robots can change this interval
    * when they decide to move making it a shorter interval semi-static obstacle
    *******************************************************************************************/
    for (auto &robot : state.getRobots())
    {
        semiStaticObstacles.emplace(std::make_pair(std::make_pair(robot->getBody()->getPose().getPosition().getPosX(),
                                                                  robot->getBody()->getPose().getPosition().getPosY()),
                                                   std::make_tuple(0, INT_MAX, robot->getMoveMechanism())));
    }
}

bool PathFinder::safeDynamic(const std::tuple<int,int,int>& st3, const Node* node, const Node* cameFrom) const
{
    // Moment before arrival must be safe so the side of the robots won't collide!
    std::tuple<int, int, int> beforeMoment = std::make_tuple(std::get<0>(st3), std::get<1>(st3), std::get<2>(st3) - 1);

    // Arrival check
    if (reservationTable.find(st3) != reservationTable.end() ||
        reservationTable.find(beforeMoment) != reservationTable.end())
        return false;

    // Travel check
    for(int i = cameFrom->gCost + 1; i < cameFrom->gCost + node->byTime; ++i)
    {
        std::tuple<int,int,int> travelSt3 = std::make_tuple(cameFrom->coords.first, cameFrom->coords.second, i);
        if(reservationTable.find(travelSt3) != reservationTable.end())
            return false;
    }

    return true;
}

bool PathFinder::safeSoftStatic(const std::pair<int, int> &coord, const Point<> &destination) const
{
    return PathFinder::PointToPairEqualityAdapter(destination, coord) ||
           (coord.first >= 0 && coord.first < ROWS &&
            coord.second >= 0 && coord.second < COLS &&
            softStaticObstacles.find(coord) == softStaticObstacles.end());
}

bool PathFinder::safeHardStatic(const std::pair<int, int> &coord, const Point<> &destination) const
{
    return PathFinder::PointToPairEqualityAdapter(destination, coord) ||
           (coord.first >= 0 && coord.first < ROWS &&
            coord.second >= 0 && coord.second < COLS &&
            hardStaticObstacles.find(coord) == hardStaticObstacles.end());
}

bool PathFinder::safeSemiStatic(const std::pair<int, int> &coord, int timestamp, const IMoveMechanism* moveMechanism) const
{
    auto obstacles = semiStaticObstacles.equal_range(coord);
    
    // Safe if there's no registered obstacle at all
    if(obstacles.first == semiStaticObstacles.end())
        return true;

    // Safe if all registrations are safe or if the same agent tries to enter
    for(auto it = obstacles.first; it != obstacles.second; ++it)
    {
        bool sameAgent = moveMechanism == std::get<2>(it->second);
        bool safe = (timestamp + 2 <= std::get<0>(it->second)) || (timestamp - 2 >= std::get<1>(it->second));

        if(sameAgent)
            continue;

        if(!safe)
            return false;
    }
    return true;
}

std::array<std::shared_ptr<Node>, 5> PathFinder::findNeighbours(Node &node, const IMoveMechanism &moveMechanism) const
{
    std::array<std::shared_ptr<Node>, 5> neighbours{

        // Up movement with MoveMechanism Delta time and energy cost
        std::make_shared<Node>(std::make_pair(node.coords.first, node.coords.second - 1),
                               DirectionVector<>::UP(),
                               (moveMechanism.getTimeCost(node.arriveOrientation, DirectionVector<>::UP())),
                               (node.byEnergy + moveMechanism.getEnergyCost(node.arriveOrientation, DirectionVector<>::UP())), true),

        // Down movement with MoveMechanism Delta time and energy cost
        std::make_shared<Node>(std::make_pair(node.coords.first, node.coords.second + 1),
                               DirectionVector<>::DOWN(),
                               (moveMechanism.getTimeCost(node.arriveOrientation, DirectionVector<>::DOWN())),
                               (node.byEnergy + moveMechanism.getEnergyCost(node.arriveOrientation, DirectionVector<>::DOWN())), true),

        // Left movement with MoveMechanism Delta time and energy cost
        std::make_shared<Node>(std::make_pair(node.coords.first - 1, node.coords.second),
                               DirectionVector<>::LEFT(),
                               (moveMechanism.getTimeCost(node.arriveOrientation, DirectionVector<>::LEFT())),
                               (node.byEnergy + moveMechanism.getEnergyCost(node.arriveOrientation, DirectionVector<>::LEFT())), true),

        //  Right movement with Movemechanism Delta time and energy cost
        std::make_shared<Node>(std::make_pair(node.coords.first + 1, node.coords.second),
                               DirectionVector<>::RIGHT(),
                               (moveMechanism.getTimeCost(node.arriveOrientation, DirectionVector<>::RIGHT())),
                               (node.byEnergy + moveMechanism.getEnergyCost(node.arriveOrientation, DirectionVector<>::RIGHT())), true),

        // Waiting at current position with 1 time cost 0 energy cost
        std::make_shared<Node>(node.coords, node.arriveOrientation, 1, node.byEnergy, false)};

    return neighbours;
}

bool PathFinder::PointToPairEqualityAdapter(const Point<> &point, const std::pair<int, int> &pair)
{
    return point.getPosX() == pair.first && point.getPosY() == pair.second;
}

int PathFinder::ManhattanHeuristic(const Node &node, const Point<> &point)
{
    return std::abs(node.coords.first - point.getPosX()) + std::abs(node.coords.second - point.getPosY());
}

void PathFinder::claimPath(const std::vector<std::shared_ptr<Node>> &path)
{
    // TODO: remove runtime error
    for (int i = path.size() - 1; i > 0; --i)
    {
        reservationTable.emplace(std::make_tuple(path[i]->coords.first, path[i]->coords.second, path[i]->gCost));
        reservationTable.emplace(std::make_tuple(path[i]->coords.first, path[i]->coords.second, path[i]->gCost - 1));

        for (int travelTime = 1; travelTime <= path[i - 1]->byTime - 1; ++travelTime)
            reservationTable.emplace(std::make_tuple(path[i]->coords.first, path[i]->coords.second, path[i]->gCost + travelTime));
    }

    reservationTable.emplace(std::make_tuple(path[0]->coords.first, path[0]->coords.second, path[0]->gCost));
    reservationTable.emplace(std::make_tuple(path[0]->coords.first, path[0]->coords.second, path[0]->gCost - 1));
}

void PathFinder::claimST3(const std::tuple<int, int, int> &st3)
{
    reservationTable.insert(st3);
}

void PathFinder::claimST3Interval(const std::pair<int,int> &point, int beginTime, int endTime)
{
    for(int time = beginTime; time <= endTime; ++time)
        reservationTable.emplace(std::make_tuple(point.first,point.second, time));
}

void PathFinder::emplaceSemiStatic(const std::pair<int,int> &point, int startTime, int endTime, const IMoveMechanism* moveMechanism)
{
    semiStaticObstacles.emplace(std::make_pair(point,std::make_tuple(startTime,endTime,moveMechanism)));
}

void PathFinder::alterEndOfInfiniteSemiStatic(const std::pair<int,int> &point, int newEndTime)
{
    auto semiStatics = semiStaticObstacles.equal_range(point);
    for(auto it = semiStatics.first; it != semiStatics.second; ++it)
    {
        // Reset infinity to new end time
        if(std::get<1>(it->second) == INT_MAX)
        {
            std::get<1>(it->second) = newEndTime;
            return;
        }
    }
}

std::vector<std::shared_ptr<Node>> PathFinder::tracePath(const std::shared_ptr<Node> &node) const
{
    std::vector<std::shared_ptr<Node>> path;
    std::shared_ptr<Node> n = node;
    while (n != nullptr)
    {
        std::cerr << n->coords.first << " " << n->coords.second << " " << n->gCost << std::endl;
        path.push_back(n);
        n = n->cameFrom;
    }
    std::cerr << "===============================" << std::endl;
    return path;
}

std::vector<std::shared_ptr<Node>> PathFinder::findPathSoft(const Point<> &startPos,
                                                            const Point<> &endPos,
                                                            const DirectionVector<> &startOr,
                                                            int startTime,
                                                            const IMoveMechanism &moveMechanism) const
{
    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, NodeComparator> open;
    std::unordered_set<const Node *> openLookup;

    std::unordered_set<std::tuple<int, int, int>, boost::hash<std::tuple<int, int, int>>> closed;

    std::shared_ptr<Node> current = std::make_shared<Node>(std::make_pair(startPos.getPosX(), startPos.getPosY()), startOr, 0, 0, false, startTime, 0, nullptr);
    openLookup.insert(current.get());
    open.push(current);

    while (!open.empty())
    {
        current = open.top();
        openLookup.erase(open.top().get());
        open.pop();

        closed.emplace(std::make_tuple(current->coords.first, current->coords.second, current->gCost));

        // ##################### Found path ######################
        if (PathFinder::PointToPairEqualityAdapter(endPos, current->coords))
            return tracePath(current);

        // ################ Explore STA* Neighbours ###############
        std::array<std::shared_ptr<Node>, 5> neighbours = findNeighbours(*current, moveMechanism);
        for (auto node : neighbours)
        {
            node->gCost = current->gCost + node->byTime;
            std::tuple<int, int, int> st3(std::make_tuple(node->coords.first, node->coords.second, node->gCost));

            // ################### Continue if node is in closed ####################
            if (closed.find(st3) != closed.end())
                continue;

            // ####################### Continue if not safe #########################
            if (!safeSoftStatic(node->coords, endPos))
                continue;

            if (!safeSemiStatic(node->coords, node->gCost, &moveMechanism))
                continue;

            if (!safeDynamic(st3, node.get(), current.get()))
                continue;

            // ####################### If not in open ###############################
            if (openLookup.find(node.get()) == openLookup.end())
            {
                node->cameFrom = current;
                node->hCost = PathFinder::ManhattanHeuristic(*node, endPos);

                openLookup.insert(node.get());
                open.push(node);
            }
        }
    }
    throw std::runtime_error("No Path");
}

std::vector<std::shared_ptr<Node>> PathFinder::findPathHard(const Point<> &startPos, const Point<> &endPos, const DirectionVector<> &startOr,
                                                            int startTime, const IMoveMechanism &moveMechanism) const
{
    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, NodeComparator> open;
    std::unordered_set<const Node *> openLookup;

    std::unordered_set<std::tuple<int, int, int>, boost::hash<std::tuple<int, int, int>>> closed;

    std::shared_ptr<Node> current = std::make_shared<Node>(std::make_pair(startPos.getPosX(), startPos.getPosY()), startOr, 0, 0, false, startTime, 0, nullptr);
    openLookup.insert(current.get());
    open.push(current);

    while (!open.empty())
    {
        current = open.top();
        openLookup.erase(open.top().get());
        open.pop();

        closed.emplace(std::make_tuple(current->coords.first, current->coords.second, current->gCost));

        // ##################### Found path ######################
        if (PathFinder::PointToPairEqualityAdapter(endPos, current->coords))
            return tracePath(current);

        // ################ Explore STA* Neighbours ###############
        std::array<std::shared_ptr<Node>, 5> neighbours = findNeighbours(*current, moveMechanism);
        for (auto node : neighbours)
        {
            node->gCost = current->gCost + node->byTime;
            std::tuple<int, int, int> st3(std::make_tuple(node->coords.first, node->coords.second, node->gCost));

            // ################### Continue if node is in closed ####################
            if (closed.find(st3) != closed.end())
                continue;

            // ####################### Continue if not safe #########################
            if (!safeHardStatic(node->coords, endPos))
                continue;

            if (!safeSemiStatic(node->coords, node->gCost, &moveMechanism))
                continue;

            if (!safeDynamic(st3, node.get(), current.get()))
                continue;

            // ####################### If not in open ###############################
            if (openLookup.find(node.get()) == openLookup.end())
            {
                node->cameFrom = current;
                node->hCost = PathFinder::ManhattanHeuristic(*node, endPos);

                openLookup.insert(node.get());
                open.push(node);
            }
        }
    }
    throw std::runtime_error("No path");
}