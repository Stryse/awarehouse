#include "PathFinder.h"
#include "IMoveMechanism.h"
#include "State.h"
#include <cmath>

PathFinder::PathFinder(const State &state)
    : ROWS(state.getRowCount()), COLS(state.getColCount())
{
    reservationTable.reserve(10000);

    //########################## Setup static obstacles ###########################
    // PodDocks
    for (auto &podDock : state.getPodDocks())
    {
        staticObstacles.emplace(std::make_pair(podDock->getPosition().getPosX(),
                                               podDock->getPosition().getPosY()));
    }
    // Charging Stations
    for (auto &ChargingStation : state.getChargingStations())
    {
        staticObstacles.emplace(std::make_pair(ChargingStation->getPosition().getPosX(),
                                               ChargingStation->getPosition().getPosY()));
    }
    // Delivery Stations
    for (auto &DeliveryStation : state.getDeliveryStations())
    {
        staticObstacles.emplace(std::make_pair(DeliveryStation->getPosition().getPosX(),
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

bool PathFinder::safeStatic(const std::pair<int, int> &coord) const
{
    return coord.first >= 0 && coord.first < ROWS &&
           coord.second >= 0 && coord.second < COLS &&
           staticObstacles.find(coord) == staticObstacles.end();
}

bool PathFinder::safeSemiStatic(const std::pair<int, int> &coord, int timestamp) const
{
    auto obstacle = semiStaticObstacles.find(coord);
    return obstacle == semiStaticObstacles.end() || timestamp < obstacle->second;
}

std::array<std::shared_ptr<Node>, 5> PathFinder::findNeighbours(Node &node, const IMoveMechanism &moveMechanism) const
{
    std::array<std::shared_ptr<Node>, 5> neighbours{

        // Up movement with MoveMechanism Delta time and energy
        std::make_shared<Node>(std::make_pair(node.coords.first, node.coords.second - 1),
                 DirectionVector<>::UP(),
                 (moveMechanism.getTimeCost(node.arriveOrientation, DirectionVector<>::UP())),
                 (node.byEnergy + moveMechanism.getEnergyCost(node.arriveOrientation, DirectionVector<>::UP()))),

        // Down movement with MoveMechanism Delta time and energy
         std::make_shared<Node>(std::make_pair(node.coords.first, node.coords.second + 1),
                 DirectionVector<>::DOWN(),
                 (moveMechanism.getTimeCost(node.arriveOrientation, DirectionVector<>::DOWN())),
                 (node.byEnergy + moveMechanism.getEnergyCost(node.arriveOrientation, DirectionVector<>::DOWN()))),

        // Left movement with MoveMechanism Delta time and energy
         std::make_shared<Node>(std::make_pair(node.coords.first - 1, node.coords.second),
                 DirectionVector<>::LEFT(),
                 (moveMechanism.getTimeCost(node.arriveOrientation, DirectionVector<>::LEFT())),
                 (node.byEnergy + moveMechanism.getEnergyCost(node.arriveOrientation, DirectionVector<>::LEFT()))),

        //  Right movement with Movemechanism Delta time and energy
         std::make_shared<Node>(std::make_pair(node.coords.first + 1, node.coords.second),
                 DirectionVector<>::RIGHT(),
                 (moveMechanism.getTimeCost(node.arriveOrientation, DirectionVector<>::RIGHT())),
                 (node.byEnergy + moveMechanism.getEnergyCost(node.arriveOrientation, DirectionVector<>::RIGHT()))),

        // Waiting at current position
         std::make_shared<Node>(node.coords, node.arriveOrientation, 1, node.byEnergy)
    };

    return neighbours;
}

void PathFinder::registerNode(const std::shared_ptr<Node> &node)
{
    reservationTable.emplace(std::make_tuple(node->coords.first, node->coords.second, node->gCost));
    //reservationTable.emplace(std::make_tuple(node.coords.first, node.coords.second, node.cameFrom->byTime));
    if (node->cameFrom != nullptr)
    {
        for (int travelTime = node->cameFrom->gCost; travelTime < node->gCost; ++travelTime)
            reservationTable.emplace(std::make_tuple(node->coords.first, node->coords.second, travelTime));
    }
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
    for (auto &node : path) // hátúlról kell majd
        registerNode(node);
}

std::vector<std::shared_ptr<Node>> PathFinder::tracePath(const std::shared_ptr<Node>& node) const
{
    std::vector<std::shared_ptr<Node>> path;

    std::shared_ptr<Node> n = node;
    while(n->cameFrom != nullptr)
    {
        path.push_back(n);
        n = n->cameFrom;
    }
    return path;
}

std::vector<std::shared_ptr<Node>> PathFinder::findPath(const Point<> &startPos, const Point<> &endPos, const DirectionVector<> &startOr,
                          int startTime, const IMoveMechanism &moveMechanism) const
{
    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, NodeComparator> open;
    std::unordered_set<const Node *> openLookup;

    std::unordered_set<std::tuple<int, int, int>, boost::hash<std::tuple<int, int, int>>> closed;

    std::shared_ptr<Node> current = std::make_shared<Node>(std::make_pair(startPos.getPosX(), startPos.getPosY()), startOr, 0, 0, startTime, 0, nullptr);
    openLookup.insert(current.get());
    open.push(current);

    while (!open.empty())
    {
        current = open.top();
        openLookup.erase(open.top().get());
        open.pop();

        closed.emplace(std::make_tuple(current->coords.first, current->coords.second, current->byTime));

        // ##################### Found path ######################
        if (PathFinder::PointToPairEqualityAdapter(endPos, current->coords))
            return tracePath(current);

        // ################ Explore STA* Neighbours ###############
        std::array<std::shared_ptr<Node>, 5> neighbours = findNeighbours(*current, moveMechanism);
        for (auto node : neighbours)
        {
            std::tuple<int, int, int> st3(std::make_tuple(node->coords.first, node->coords.second, node->byTime));

            // ################### Continue if node is in closed ####################
            if (closed.find(st3) != closed.end())
                continue;

            // ####################### Continue if not safe #########################
            if (!safeStatic(node->coords) ||
                !safeSemiStatic(node->coords, node->byTime) ||
                !safeDynamic(st3)) // TODO only expand waiting when dynamic obstacles present (further heuristic)
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
    //return {};
}