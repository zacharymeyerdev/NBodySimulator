// 2d vector type, holds (x, y) values for operations using them, abstraction

#ifndef VEC2_HPP
#define VEC2_HPP

#include "real_type.hpp"
#include <cmath>

/**
 * @brief simple 2D vector class for positions, velocities, and forces
 *        math helper that represents a 2D vector (x, y) using the Real type
 * Has:
 *  vector addition
 *  vector subtraction
 *  scalar multiplication
 *  euclidian length
 * 
 */
class Vec2{
public:
    Real x; // x component of vector
    Real y; // y component of vector

    /**
     * @brief default constructor
     * Initializes:
     *      vector = (0, 0)
     * 
     */
    Vec2() : x(static_cast<Real>(0)), y(static_cast<Real>(0)){}
    /**
     * @brief Construct a vector with explicit x and y components
     * 
     * @param xVal initial x component
     * @param yVal initial y component
     */
    Vec2(Real xVal, Real yVal) : x(xVal), y(yVal){}
    /**
     * @brief return the sum of this vector and another
     * 
     * @param v vector to add
     * @return Vec2 containing the element-wise sum
     */
    Vec2 add(const Vec2 &v) const{
        return Vec2(x + v.x, y + v.y);
    }
    /**
     * @brief return difference between this vector and another
     * 
     * @param v vector to subtract
     * @return Vec2 new Vec2 containing element-wise difference
     */
    Vec2 sub(const Vec2 &v) const{
        return Vec2(x - v.x, y - v.y);
    }
    /**
     * @brief return this vector scaled by another scalar
     * 
     * @param k scalar multiplier
     * @return Vec2 new Vec2 containing the scaled components
     */
    Vec2 scale(Real k) const{
        return Vec2(k * x, k * y);
    }
    /**
     * @brief compute euclidean length of vector
     *        ||v|| = sqrt(x^2 + y^2)
     * @return Real the length of the vector
     */
    Real norm() const{
        return static_cast<Real>(std::sqrt(x * x + y * y));
    }
};

#endif