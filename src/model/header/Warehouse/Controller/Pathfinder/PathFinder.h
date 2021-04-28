#ifndef PATH_FINDER__H
#define PATH_FINDER__H

#include "DirectionVector.h"
#include "Point.h"
#include <array>
#include <boost/functional/hash.hpp>
#include <memory>
#include <queue>
#include <unordered_set>
#include <utility>
#include <vector>

// ########################### FORWARD DECLARATIONS ######################### //
class State;
class IMoveMechanism;
// ########################################################################## //

/****************************************************************************
 * @brief A Node in the space-time searchspace.
 ****************************************************************************/
struct Node
{
    std::pair<int, int> coords;
    DirectionVector<> arriveOrientation; // The orientation when we arrive (need so we can calculate time and energy cost)
    int byTime;                          //Time cost of arrival
    int byEnergy;                        //Cummulative Energy cost of arrival

    int gCost;
    int hCost;
    bool moved;

    std::shared_ptr<Node> cameFrom;      // Node we came from

    explicit Node(std::pair<int, int> coords, DirectionVector<> arriveOrientation, int byTime, int byEnergy, bool moved)

        : coords(std::move(coords)),
          arriveOrientation(std::move(arriveOrientation)),
          byTime(byTime),
          byEnergy(byEnergy),
          moved(moved),
          cameFrom(nullptr)
    {
    }

    explicit Node(std::pair<int, int> coords, DirectionVector<> arriveOrientation, int byTime, int byEnergy, bool moved,
                  int gCost, int hCost, std::shared_ptr<Node> cameFrom = nullptr)
        : coords(std::move(coords)),
          arriveOrientation(std::move(arriveOrientation)),
          byTime(byTime),
          byEnergy(byEnergy),
          moved(moved),
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
        return lhs->getScore() > rhs->getScore();
    }
};

/********************************************************************
 * @brief This class is responsible for finding collision free
 * paths in space-time.
 * It is also resposible for maintaining the ST3 reservation tables.
 * 
 * Pathfinder DOES NOT check reservations for uniqueness so
 * users must reserve ST3 coordinates with great care so the planner
 * wont plan non collision free path.
 ********************************************************************/
class PathFinder
{
public:
    explicit PathFinder(const State &state);

    /************************************************************************************************************
     * @brief Finds collision free path from start pos to endpos with starting orientation with startingtime.
     * Obstacle restriction is soft. Only soft static obstacles must be avoided.
     ************************************************************************************************************/
    std::vector<std::shared_ptr<Node>> findPathSoft(const Point<> &startPos, const Point<> &endPos, const DirectionVector<> &startOr,
                                                int startTime, const IMoveMechanism &moveMechanism) const;

    /************************************************************************************************************
     * @brief Finds collision free path from start pos to endpos with startingorientation with startingtime.
     * Obstacle restriction is hard. Soft and Hard static obstacles must be avoided.
     ************************************************************************************************************/
    std::vector<std::shared_ptr<Node>> findPathHard(const Point<> &startPos, const Point<> &endPos, const DirectionVector<> &startOr,
                                                int startTime, const IMoveMechanism &moveMechanism) const;

    /***********************************************************************
     * @brief Registers a path in the reservation table,
     * so it is marked as used.
     ***********************************************************************/
    void claimPath(const std::vector<std::shared_ptr<Node>> &path);

    /***********************************************************************
     * @brief Registers a Space-Time-3 coordinate in the reservation table.
     ***********************************************************************/
    void claimST3(const std::tuple<int,int,int> &st3);

    /*************************************************************************************
     * @brief Claim a point for a period of time [beginTime,endTime].
     *************************************************************************************/
    void claimST3Interval(const std::pair<int,int> &point, int beginTime, int endTime);

    /*************************************************************************************
     * @brief Alter the leave time of a semi-static obstacle from infinity to newEndTime.
     *************************************************************************************/
    void alterEndOfInfiniteSemiStatic(const std::pair<int,int> &point, int newEndTime);

    /*****************************************************************
     * @brief Returns whether Coordinate (X,Y) is safe at timestamp T.
     *****************************************************************/
    bool safeDynamic(const std::tuple<int, int, int> &st3) const;

    /*****************************************************************
     * @brief Returns whether Coordinate (X,Y) can be occupied according
     * to soft static obstacles.
     *****************************************************************/
    bool safeSoftStatic(const std::pair<int, int> &coord, const Point<>& destination) const;

    /*******************************************************************
     * @brief Returns whether Coordinate (X,Y) can be occupied according
     * to hard static obstacles.
     *******************************************************************/
    bool safeHardStatic(const std::pair<int, int> &coord, const Point<>& destination) const;

    /**************************************************************************
     * @brief Returns whether Coordinate (X,Y) can be occupied according
     * to semi static obstacles (eg. an agent is not currently waiting there)
     * 
     * Safe if there's no semi-static obstacle at position (x,y) or it will be
     * only there out if timeframe which is relevant.
     **************************************************************************/
    bool safeSemiStatic(const std::pair<int, int> &coord, int timestamp, const IMoveMechanism* moveMechanism) const;

private:
    /*********************************************************************
     * @brief Returns a vector of Node neighbours
     * Neighbours of node (x,y,t) are
     * (x+1,y  ,t + timeMove)
     * (x  ,y+1,t + timeMove)
     * (x-1,y  ,t + timeMove)
     * (x  ,y-1,t + timeMove)
     * (x  ,y  ,t + 1)
     *********************************************************************/
    std::array<std::shared_ptr<Node>, 5> findNeighbours(Node &node, const IMoveMechanism &moveMechanism) const;

    /**********************************************************************
     * @brief Registers a node in the reservationTable.
     **********************************************************************/
    void registerNode(const std::shared_ptr<Node> &node);

    /***********************************************************************
     * @brief Compare point to pair without conversion.
     ***********************************************************************/
    static bool PointToPairEqualityAdapter(const Point<> &point, const std::pair<int, int> &pair);

    /***********************************************************************
     * @brief Admissible and Consistent heuristic.
     ***********************************************************************/
    static int ManhattanHeuristic(const Node &node, const Point<> &point);

    /*******************************************************************************
     * @brief Returns a Node sequence of the path with ending node provided.
     *******************************************************************************/
    std::vector<std::shared_ptr<Node>> tracePath(const std::shared_ptr<Node>& node) const;

private:
    std::unordered_set<std::pair<int, int>, boost::hash<std::pair<int, int>>> softStaticObstacles;
    std::unordered_set<std::pair<int, int>, boost::hash<std::pair<int, int>>> hardStaticObstacles;

    std::unordered_multimap<std::pair<int, int>, std::tuple<int,int,IMoveMechanism*>, boost::hash<std::pair<int, int>>> semiStaticObstacles;
    std::unordered_set<std::tuple<int, int, int>, boost::hash<std::tuple<int,int,int>>> reservationTable;

    size_t ROWS;
    size_t COLS;
};

#endif
