#ifndef MATRIX__H
#define MATRIX__H

#include "DirectionVector.h"
#include <array>
#include <cstddef>
#include <type_traits>

/******************************************************************
 * @brief Numerical matrix type that can be used for
 * eg. Vector transformation
 * @tparam CoordT type of matrix elements (must be numeric)
 * @tparam Row number of rows in the matrix
 * @tparam Col number of columns in the matrix
 ******************************************************************/
template <typename CoordT = int, std::size_t Row = 3, std::size_t Col = 3>
class Matrix
{
public:
    using CoordinateT = CoordT;

    //////[ASSERTION] Template parameter CoordinateT must be numeric type
    static_assert(std::is_arithmetic<CoordinateT>::value, "Type CoordinateT must be arithmetic value eg. int,float");

private:
    std::array<CoordinateT, Row * Col> elements;

public:
    virtual ~Matrix() = default;
    Matrix(const Matrix &other) = default;
    Matrix(Matrix &&other) = default;
    Matrix &operator=(const Matrix &other) = default;

protected:
    Matrix();

public:
    /*********************************************************************
     * @brief Linear transformation of a vector (modifies parameter)
     *********************************************************************/
    void transform(DirectionVector<CoordinateT, Col> &target) const
    {
        for (size_t i = 0; i < Row; ++i)
        {
            target[i] = 0;
            for (size_t j = 0; j < Col; ++j)
                target[i] += getElement(i, j) * target[j];
        }
    }

    /*********************************************************************
     * @brief Linear transformation of a vector (as new variable)
     *********************************************************************/
    DirectionVector<CoordinateT> transformed(const DirectionVector<CoordinateT, Col> &other) const
    {
        std::array<CoordinateT, Col> buffer;
        for (size_t i = 0; i < Row; ++i)
        {
            buffer[i] = 0;
            for (size_t j = 0; j < Col; ++j)
                buffer[i] += getElement(i, j) * buffer[j];
        }

        return DirectionVector<CoordinateT>(std::move(buffer));
    }

    /**********************************************************************
     * @brief 3 Dimensional rotation matrix.
     * It can rotate along the Z axis by 90 degrees clockwise
     **********************************************************************/
    static Matrix<int, 3, 3> ROTATE_Z_90_CLOCKWISE()
    {
        return Matrix<int, 3, 3>{0, 1, 0,
                                 -1, 0, 0,
                                 0, 0, 1};
    }

    /**********************************************************************
     * @brief 3 Dimensional rotation matrix.
     * It can rotate along the Z axis by 90 degrees counterclockwise
     **********************************************************************/
    static Matrix<int, 3, 3> ROTATE_Z_90_COUNTERCLOCKWISE()
    {
        return Matrix<int, 3, 3>{0, -1, 0,
                                 1, 0, 0,
                                 0, 0, 1};
    }

    // ######################### GETTER ##################################
    constexpr size_t getRowSize() const noexcept { return Row; }
    constexpr size_t getColSize() const noexcept { return Col; }

    const CoordinateT &getElement(size_t rowCoord, size_t colCoord) const
    {
        return elements[rowCoord * Row + colCoord];
    }
    CoordinateT &getElement(size_t rowCoord, size_t colCoord)
    {
        return elements[rowCoord * Row + colCoord];
    }
    void setElement(size_t rowCoord, size_t colCoord, const CoordinateT &value)
    {
        elements[rowCoord * Row + colCoord] = value;
    }
    const std::array<CoordinateT, Row * Col> &getBuffer() const
    {
        return elements;
    }
    std::array<CoordinateT, Row * Col> &getBuffer()
    {
        return elements;
    }
};

#endif