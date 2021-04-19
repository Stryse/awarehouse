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

std::array<Node *, 5> PathFinder::findNeighbours(Node &node, const IMoveMechanism &moveMechanism) const
{
    std::array<Node *, 5> neighbours{

        // Up movement with MoveMechanism Delta time and energy
        new Node(std::make_pair(node.coords.first, node.coords.second - 1),
                 DirectionVector<>::UP(),
                 (moveMechanism.getTimeCost(node.arriveOrientation, DirectionVector<>::UP())),
                 (node.byEnergy + moveMechanism.getEnergyCost(node.arriveOrientation, DirectionVector<>::UP()))),

        // Down movement with MoveMechanism Delta time and energy
        new Node(std::make_pair(node.coords.first, node.coords.second + 1),
                 DirectionVector<>::DOWN(),
                 (moveMechanism.getTimeCost(node.arriveOrientation, DirectionVector<>::DOWN())),
                 (node.byEnergy + moveMechanism.getEnergyCost(node.arriveOrientation, DirectionVector<>::DOWN()))),

        // Left movement with MoveMechanism Delta time and energy
        new Node(std::make_pair(node.coords.first - 1, node.coords.second),
                 DirectionVector<>::LEFT(),
                 (moveMechanism.getTimeCost(node.arriveOrientation, DirectionVector<>::LEFT())),
                 (node.byEnergy + moveMechanism.getEnergyCost(node.arriveOrientation, DirectionVector<>::LEFT()))),

        //  Right movement with Movemechanism Delta time and energy
        new Node(std::make_pair(node.coords.first + 1, node.coords.second),
                 DirectionVector<>::RIGHT(),
                 (moveMechanism.getTimeCost(node.arriveOrientation, DirectionVector<>::RIGHT())),
                 (node.byEnergy + moveMechanism.getEnergyCost(node.arriveOrientation, DirectionVector<>::RIGHT()))),

        // Waiting at current position
        new Node(node.coords, node.arriveOrientation, 1, node.byEnergy)
    };

    return neighbours;
}

void PathFinder::registerNode(const Node &node)
{
    reservationTable.emplace(std::make_tuple(node.coords.first, node.coords.second, node.byTime));
    //reservationTable.emplace(std::make_tuple(node.coords.first, node.coords.second, node.cameFrom->byTime));
    if (node.cameFrom != nullptr)
    {
        for (int travelTime = node.cameFrom->byTime; travelTime < node.byTime; ++travelTime)
            reservationTable.emplace(std::make_tuple(node.coords.first, node.coords.second, travelTime));
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

void PathFinder::claimPath(const std::vector<Node> &path)
{
    for (auto &node : path) // hátúlról kell majd
        registerNode(node);
}

void PathFinder::findPath(const Point<> &startPos, const Point<> &endPos, const DirectionVector<> &startOr,
                          int startTime, const IMoveMechanism &moveMechanism) const
{
    std::priority_queue<Node *, std::vector<Node *>, NodeComparator> open;
    std::unordered_set<const Node *> openLookup;

    std::unordered_set<std::tuple<int, int, int>, boost::hash<std::tuple<int, int, int>>> closed;

    Node *current = new Node(std::make_pair(startPos.getPosX(), startPos.getPosY()), startOr, startTime, 0, 0, 0, nullptr);
    openLookup.insert(current);
    open.push(current);

    while (!open.empty())
    {
        current = open.top();
        openLookup.erase(open.top());
        open.pop();

        closed.emplace(std::make_tuple(current->coords.first, current->coords.second, current->byTime));

        // ##################### Found path ######################
        if (PathFinder::PointToPairEqualityAdapter(endPos, current->coords))
            break;

        // ################ Explore STA neighbours ###############
        std::array<Node *, 5> neighbours = findNeighbours(*current, moveMechanism);
        for (auto node : neighbours)
        {
            std::tuple<int, int, int> st3(std::make_tuple(node->coords.first, node->coords.second, node->byTime));

            // ################### Continue if node is in closed ####################
            if (closed.find(st3) != closed.end())
            {
                delete node;
                continue;
            }

            // ####################### Continue if not safe #########################
            if (!safeStatic(node->coords) ||
                !safeSemiStatic(node->coords, node->byTime) ||
                !safeDynamic(st3)) // TODO only expand waiting when dynamic obstacles present (further heuristic)
            {
                delete node;
                continue;
            }

            // ####################### If not in open ###############################
            if (openLookup.find(node) == openLookup.end())
            {
                if (node->cameFrom != nullptr)
                    delete node->cameFrom;

                node->cameFrom = current;
                node->gCost = current->gCost + node->byTime;
                node->hCost = PathFinder::ManhattanHeuristic(*node, endPos);

                openLookup.insert(node);
                open.push(node);
            }
            else
                delete node;
        }
    }
}