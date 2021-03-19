#ifndef MATRIX__H
#define MATRIX__H

#include "DirectionVector.h"
#include <type_traits>
#include <vector>

template <typename TNumeric>
class Matrix
{
public:
    using Numeric = TNumeric;

    //////[ASSERTION] Template parameter CoordinateT must be numeric type
    static_assert(std::is_arithmetic<Numeric>::value, "Type CoordinateT must be arithmetic value eg. int,float");

private:
    std::vector<Numeric> elements;
    size_t rowSize;
    size_t colSize;

public:
    explicit Matrix(size_t rowSize, size_t colSize)
        : elements(rowSize * rowSize), rowSize(rowSize), colSize(colSize)
    {
    }

    explicit Matrix(size_t rowSize, size_t colSize, std::vector<Numeric> &&elements)
        : elements(std::move(elements)), rowSize(rowSize), colSize(colSize)
    {
    }

public:
    const Numeric &getElement(size_t rowCoord, size_t colCoord) const
    {
        return elements[rowCoord * rowSize + colCoord];
    }

    Numeric &getElement(size_t rowCoord, size_t colCoord)
    {
        return elements[rowCoord * rowSize + colCoord];
    }

    void setElement(size_t rowCoord, size_t colCoord, const Numeric &value)
    {
        elements[rowCoord * rowSize + colCoord] = value;
    }

    const std::vector<Numeric> &getAsVector() const
    {
        return elements;
    }

    std::vector<Numeric> &getAsVector()
    {
        return elements;
    }

    DirectionVector<Numeric> transformed(const DirectionVector<Numeric> &other)
    {
        if (other.getSize() != colSize)
            return {0, 0, 0};

        std::vector<Numeric> buffer;
        buffer.reserve(other.getSize());

        for (size_t i = 0; i < rowSize; ++i)
        {
            buffer[i] = 0;
            for (size_t j = 0; j < colSize; ++j)
                buffer[i] += getElement(i, j) * buffer[j];
        }

        return DirectionVector<Numeric>(std::move(buffer));
    }
};

#endif