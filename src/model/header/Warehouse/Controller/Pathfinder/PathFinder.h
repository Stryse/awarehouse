#ifndef PATH_FINDER__H
#define PATH_FINDER__H

#include "DirectionVector.h"
#include "Point.h"
#include <array>
#include <boost/functional/hash.hpp>
#include <queue>
#include <unordered_set>
#include <utility>
#include <vector>
#include <memory>

// ########################### FORWARD DECLARATIONS ######################### //
class State;
class IMoveMechanism;
// ########################################################################## //

/****************************************************************************
 * @brief 
 ****************************************************************************/
struct Node
{
    std::pair<int, int> coords;
    DirectionVector<> arriveOrientation; // The orientation when we arrive (need so we can calculate time and energy cost)
    int byTime;                          //TimeStamp of arrival
    int byEnergy;                        //Energy cost of arrival

    int gCost;
    int hCost;

    std::shared_ptr<Node> cameFrom; // Node we came from

    explicit Node(std::pair<int, int> coords, DirectionVector<> arriveOrientation, int byTime, int byEnergy)

        : coords(std::move(coords)),
          arriveOrientation(std::move(arriveOrientation)),
          byTime(byTime),
          byEnergy(byEnergy),
          cameFrom(nullptr)
    {
    }

    explicit Node(std::pair<int, int> coords, DirectionVector<> arriveOrientation, int byTime, int byEnergy,
                  int gCost, int hCost, std::shared_ptr<Node> cameFrom = nullptr)
        : coords(std::move(coords)),
          arriveOrientation(std::move(arriveOrientation)),
          byTime(byTime),
          byEnergy(byEnergy),
          gCost(gCost),
          hCost(hCost),
          cameFrom(std::move(cameFrom))
    {
    }

    int getScore() const
    {
        return gCost + hCost;
    }
};

/********************************************************************
 * @brief Compares two nodes according to their F cost (G+H)
 ********************************************************************/
struct NodeComparator
{
    bool operator()(const std::shared_ptr<Node> &lhs, const std::shared_ptr<Node> &rhs)
    {
        return lhs->getScore() >= rhs->getScore();
    }
};

class PathFinder
{
public:
    explicit PathFinder(const State &state);

    /************************************************************************************************************
     * @brief Finds collision free path from start pos to endpos with starting orientation with startingtime
     ************************************************************************************************************/
    void findPath(const Point<> &startPos, const Point<> &endPos, const DirectionVector<> &startOr,
                  int startTime, const IMoveMechanism &moveMechanism) const;

    /*****************************************************************
     * @brief Registers a path in the reservation table,
     * so it is marked as used
     *****************************************************************/
    void claimPath(const std::vector<Node> &path);

    /*****************************************************************
     * @brief Returns whether Coordinate (X,Y) is safe at timestamp T
     *****************************************************************/
    bool safeDynamic(const std::tuple<int, int, int> &st3) const;

    /*****************************************************************
     * @brief Returns whether Coordinate (X,Y) can be occupied according
     * to static obstacles
     *****************************************************************/
    bool safeStatic(const std::pair<int, int> &coord) const;

    /********************************************************************
     * @brief Returns whether Coordinate (X,Y) can be occupied according
     * to semi static obstacles (eg. an agent is not currently waiting there)
     * 
     * Safe if there's no semi-static obstacle at position (x,y) or it will be
     * only there later.
     ********************************************************************/
    bool safeSemiStatic(const std::pair<int, int> &coord, int timestamp) const;

private:
    /*********************************************************************
     * @brief Returns a vector of Node neighbours
     * Neighbours of node (x,y,t) are
     * (x+1,y  ,t + timeMove)
     * (x  ,y+1,t + timeMove)
     * (x-1,y  ,t + timeMove)
     * (x  ,y-1,t + timeMove)
     * (x  ,y  ,t + timeMove)
     *********************************************************************/
    std::array<std::shared_ptr<Node>, 5> findNeighbours(Node &node, const IMoveMechanism &moveMechanism) const;

    /**********************************************************************
     * @brief Registers a node in the reservationTable
     **********************************************************************/
    void registerNode(const Node &node);

    /***********************************************************************
     * @brief Compare point to pair without conversion
     ***********************************************************************/
    static bool PointToPairEqualityAdapter(const Point<> &point, const std::pair<int, int> &pair);

    /***********************************************************************
     * @brief 
     ***********************************************************************/
    static int ManhattanHeuristic(const Node &node, const Point<> &point);

private:
    std::unordered_set<std::pair<int, int>, boost::hash<std::pair<int, int>>> staticObstacles;
    std::unordered_map<std::pair<int, int>, int, boost::hash<std::pair<int, int>>> semiStaticObstacles;
    std::unordered_set<std::tuple<int, int, int>, boost::hash<std::tuple<int, int, int>>> reservationTable;

    size_t ROWS;
    size_t COLS;
};

#endif
