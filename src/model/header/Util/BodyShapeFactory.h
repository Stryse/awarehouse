#ifndef BODY_SHAPE_FACTORY__H
#define BODY_SHAPE_FACTORY__H

#include "Point.h"
#include <stdexcept>
#include <vector>

template <typename TPoint = Point<>>
class BodyShapeFactory
{
public:
    using Point = TPoint;

    static std::vector<Point> onlyOrigin() { return std::vector<Point>{}; }
    static std::vector<Point> twoBlockHeigh() { return {Point(0, 0, 1)}; }
    static std::vector<Point> twoByTwoCube()
    {
        std::vector<Point> buffer(7);
        buffer.emplace_back(1, 0, 0);
        buffer.emplace_back(0, 0, 1);
        buffer.emplace_back(1, 0, 1);
        buffer.emplace_back(0, -1, 0);
        buffer.emplace_back(0, -1, 1);
        buffer.emplace_back(1, -1, 0);
        buffer.emplace_back(1, -1, 1);
        return buffer;
    }
};

#endif /* BODY_SHAPE_FACTORY__H */