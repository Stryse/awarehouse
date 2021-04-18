#ifndef PATH_FINDER__H
#define PATH_FINDER__H

#include "Point.h"
#include <unordered_map>
#include <unordered_set>
#include <boost/functional/hash.hpp>
#include <utility>
#include <vector>


class Node
{
};

class PathFinder
{
public:
    std::vector<Node> findPath(Point<> startPos, Point<> endPost, int startTime);

private:
    std::unordered_set<std::tuple<int, int>, boost::hash<std::tuple<int,int>>> staticObstacles;
    //std::unordered_multimap<> semiStaticObstacles;
    std::unordered_set<std::tuple<int, int, int>, boost::hash<std::tuple<int,int,int>>> reservationTable;
};

#endif
