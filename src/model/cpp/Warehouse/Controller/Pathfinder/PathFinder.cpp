#include "PathFinder.h"
#include "IMoveMechanism.h"
#include "State.h"
#include <cmath>
#include <iostream>
#include <stdexcept>

PathFinder::PathFinder(const State &state)
    : ROWS(state.getRowCount()), COLS(state.getColCount()),
      reservationTable{}
{
    //reservationTable.reserve(10000);

    //########################## Setup static obstacles ###########################
    //PodDocks
    for (auto &podDock : state.getPodDocks())
    {
        hardStaticObstacles.emplace(std::make_pair(podDock->getPosition().getPosX(),
                                               podDock->getPosition().getPosY()));
    }
    // Charging Stations
    for (auto &ChargingStation : state.getChargingStations())
    {
        softStaticObstacles.emplace(std::make_pair(ChargingStation->getPosition().getPosX(),
                                               ChargingStation->getPosition().getPosY()));

        hardStaticObstacles.emplace(std::make_pair(ChargingStation->getPosition().getPosX(),
                                               ChargingStation->getPosition().getPosY()));
    }
    //Delivery Stations
    for (auto &DeliveryStation : state.getDeliveryStations())
    {
        hardStaticObstacles.emplace(std::make_pair(DeliveryStation->getPosition().getPosX(),
                                               DeliveryStation->getPosition().getPosY()));

        softStaticObstacles.emplace(std::make_pair(DeliveryStation->getPosition().getPosX(),
                                               DeliveryStation->getPosition().getPosY()));
    }
    //######################### Setup Semi-static Obstacles ##############################
    // Ötletem sincs ez hogy legyen....
    for (auto &robot : state.getRobots())
    {
        semiStaticObstacles.emplace(std::make_pair(std::make_pair(robot->getBody()->getPose().getPosition().getPosX(),
                                                                  robot->getBody()->getPose().getPosition().getPosY()),
                                                   0));
    }
}

bool PathFinder::safeDynamic(const std::tuple<int, int, int> &st3) const
{
    return reservationTable.find(st3) == reservationTable.end();
}

bool PathFinder::safeSoftStatic(const std::pair<int, int> &coord, const Point<>& destination) const
{
    return PathFinder::PointToPairEqualityAdapter(destination,coord) ||
           (coord.first >= 0 && coord.first < ROWS &&
           coord.second >= 0 && coord.second < COLS &&
           softStaticObstacles.find(coord) == softStaticObstacles.end());
}

bool PathFinder::safeHardStatic(const std::pair<int, int> &coord, const Point<>& destination) const
{
    return PathFinder::PointToPairEqualityAdapter(destination,coord) ||
           (coord.first >= 0 && coord.first < ROWS &&
           coord.second >= 0 && coord.second < COLS &&
           hardStaticObstacles.find(coord) == hardStaticObstacles.end());
}

bool PathFinder::safeSemiStatic(const std::pair<int, int> &coord, int timestamp) const
{
    auto obstacle = semiStaticObstacles.find(coord);
    return obstacle == semiStaticObstacles.end() || timestamp + 4 < obstacle->second;
}

std::array<std::shared_ptr<Node>, 5> PathFinder::findNeighbours(Node &node, const IMoveMechanism &moveMechanism) const
{
    std::array<std::shared_ptr<Node>, 5> neighbours{

        // Up movement with MoveMechanism Delta time and energy
        std::make_shared<Node>(std::make_pair(node.coords.first, node.coords.second - 1),
                               DirectionVector<>::UP(),
                               (moveMechanism.getTimeCost(node.arriveOrientation, DirectionVector<>::UP())),
                               (node.byEnergy + moveMechanism.getEnergyCost(node.arriveOrientation, DirectionVector<>::UP())), true),

        // Down movement with MoveMechanism Delta time and energy
        std::make_shared<Node>(std::make_pair(node.coords.first, node.coords.second + 1),
                               DirectionVector<>::DOWN(),
                               (moveMechanism.getTimeCost(node.arriveOrientation, DirectionVector<>::DOWN())),
                               (node.byEnergy + moveMechanism.getEnergyCost(node.arriveOrientation, DirectionVector<>::DOWN())), true),

        // Left movement with MoveMechanism Delta time and energy
        std::make_shared<Node>(std::make_pair(node.coords.first - 1, node.coords.second),
                               DirectionVector<>::LEFT(),
                               (moveMechanism.getTimeCost(node.arriveOrientation, DirectionVector<>::LEFT())),
                               (node.byEnergy + moveMechanism.getEnergyCost(node.arriveOrientation, DirectionVector<>::LEFT())), true),

        //  Right movement with Movemechanism Delta time and energy
        std::make_shared<Node>(std::make_pair(node.coords.first + 1, node.coords.second),
                               DirectionVector<>::RIGHT(),
                               (moveMechanism.getTimeCost(node.arriveOrientation, DirectionVector<>::RIGHT())),
                               (node.byEnergy + moveMechanism.getEnergyCost(node.arriveOrientation, DirectionVector<>::RIGHT())), true),

        // Waiting at current position
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
    for (int i = path.size() - 2; i > 0; --i)
    {

        std::tuple<int,int,int> st3 = std::make_tuple(path[i]->coords.first, path[i]->coords.second, path[i]->gCost);
        std::tuple<int,int,int> st32 = std::make_tuple(path[i]->coords.first, path[i]->coords.second, path[i]->gCost - 1);

        if(reservationTable.find(st3) != reservationTable.end())
            throw std::runtime_error("Non unique ST3 claim"); 


        reservationTable.insert(st3);
        reservationTable.insert(st32);
        //reservationTable.emplace(std::make_tuple(path[i]->coords.first, path[i]->coords.second, path[i]->gCost));
        //reservationTable.emplace(std::make_tuple(path[i]->coords.first, path[i]->coords.second, path[i]->gCost - 1));



        for (int travelTime = 1; travelTime <= path[i - 1]->byTime - 1; ++travelTime)
            reservationTable.emplace(std::make_tuple(path[i]->coords.first, path[i]->coords.second, path[i]->gCost + travelTime));
    }

    reservationTable.emplace(std::make_tuple(path[0]->coords.first, path[0]->coords.second, path[0]->gCost));
    reservationTable.emplace(std::make_tuple(path[0]->coords.first, path[0]->coords.second, path[0]->gCost - 1));
}

void PathFinder::claimST3(const std::tuple<int, int, int> &st3)
{
    if(reservationTable.find(st3) != reservationTable.end())
        throw std::runtime_error("Non unique ST3 claim"); 

    reservationTable.insert(st3);
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

std::vector<std::shared_ptr<Node>> PathFinder::findPath(const Point<> &startPos, const Point<> &endPos, const DirectionVector<> &startOr,
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
            if (!safeSoftStatic(node->coords, endPos))
                continue;

            //if (!safeSemiStatic(node->coords, node->gCost))
            //continue;

            if (!safeDynamic(st3)) // TODO only expand waiting when dynamic obstacles present (further heuristic)
                continue;

            // ####################### If not in open ###############################
            if (openLookup.find(node.get()) == openLookup.end())
            {
                node->cameFrom = current;
                node->gCost = current->gCost + node->byTime;
                node->hCost = PathFinder::ManhattanHeuristic(*node, endPos);

                openLookup.insert(node.get());
                open.push(node);
            }
        }
    }
    return {};
}