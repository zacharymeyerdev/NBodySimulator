// body2d class = each body, mass+pos+velocity+force

#ifndef BODY2D_HPP
#define BODY2D_HPP

#include "real_type.hpp"
#include "vec2.hpp"

/**
 * @brief represents a body on a 2D plane
 * Stores:
 *      m = mass
 *      r = current position (x, y)
 *      v = current velocity (vx, vy)
 *      f = accumulated force for time step (Fx, Fy)
 * 
 * force accumulator is cleared once per time step
 * added by addForce()
 * After accumulating forces, uses m, r, v, and f to update the body's state
 * 
 */
class Body2D{
public:
    Real m; // mass of body
    Vec2 r; // current position vector (x, y)
    Vec2 v; // current velocity vector (vx, vy)
    Vec2 f; // accumulated force vector (Fx, Fy)

    /**
     * @brief default constructor
     * Initializes:
     *      mass = 0
     *      position = (0, 0)
     *      velocity = (0, 0)
     *      force = (0, 0)
     * 
     */
    Body2D() : m(static_cast<Real>(0)), r(), v(), f(){}
    /**
     * @brief Construct body given mass, position, and velocity
     * Initializes:
     *      force = (0, 0)
     * @param mass mass of body
     * @param position initial position (x, y)
     * @param velocity initial velocity (vx, vy)
     */
    Body2D(Real mass, const Vec2 &position, const Vec2 &velocity) : m(mass), r(position), v(velocity), f(){}
    /**
     * @brief clear accumulated force on body
     * called once per each force accumulation step before additions
     */
    void clearForce(){
        f.x = static_cast<Real>(0);
        f.y = static_cast<Real>(0);
    }
    /**
     * @brief force addition for the body's accumulator
     * called once for each interaction
     * 
     * @param F force vector to add to accumulator
     */
    void addForce(const Vec2 &F){
        f.x += F.x;
        f.y += F.y;
    }
};

#endif