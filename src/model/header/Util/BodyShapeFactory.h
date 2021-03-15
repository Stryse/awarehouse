#ifndef BODY_SHAPE_FACTORY__H
#define BODY_SHAPE_FACTORY__H

#include "Point.h"
#include <stdexcept>
#include <vector>

class BodyShapeFactory
{
public:
    enum class BodyShape
    {
        ONLY_ORIGIN,
        TWO_BY_TWO_CUBE,
        TWO_BLOCKS_HEIGH
    };
    static std::vector<Point<int>> createShape(const BodyShape &shape)
    {
        switch (shape)
        {
        case BodyShape::ONLY_ORIGIN:
            return {};

        case BodyShape::TWO_BLOCKS_HEIGH:
            return {Point<int>{0, 0, 1}};

        case BodyShape::TWO_BY_TWO_CUBE:
            return {
                Point<int>{1, 0, 0},
                Point<int>{0, 0, 1},
                Point<int>{1, 0, 1},
                Point<int>{0, -1, 0},
                Point<int>{0, -1, 1},
                Point<int>{1, -1, 0},
                Point<int>{1, -1, 1}};
        default:
            throw std::runtime_error("Unhandled Body Shape");
        }
    }
};

#endif /* BODY_SHAPE_FACTORY__H */