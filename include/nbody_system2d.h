// nbodysystem2d class, vector+G+eps2, physics

#ifndef NBODY_SYSTEM2D_H
#define NBODY_SYSTEM2D_H

#include "real_type.hpp"
#include "body2d.hpp"

#include <vector>
#include <cstddef>
/**
 * @brief 2d newtonian n-body system
 * Stores:
 *      list of Body2D objects with masses, positions, velocities, and forces
 *      graviational constant G
 *      softening parameter eps2 for when bodies get close
 * Responsbile for:
 *      managing list of bodies: add, query
 *      computing pairwise gravitational forces O(n^2)
 *      computing total energy = kinetic + potential
 *      advancing the system with either euler, semieuler, or verlet
 */
class NBodySystem2D{
public:
    /**
     * @brief default constructor
     * Initializes:
     *      G = 1
     *      eps2 = 0
     *      bodies list empty
     * 
     */
    NBodySystem2D();
    /**
     * @brief Construct with specified G and softening parameter
     * 
     * @param GValue gravitational constant
     * @param eps2Value softening value added to r^2
     */
    NBodySystem2D(Real GValue, Real eps2Value);

    /**
     * @brief set gravitational constant
     * 
     * @param GValue gravitational constant
     */
    void setG(Real GValue);

    /**
     * @brief Set softening parameter
     *        prevents division by very small numbers
     * 
     * @param eps2Value softening parameter
     */
    void setEps2(Real eps2Value);

    /**
     * @brief Get gravitational constant
     * 
     * @return Real 
     */
    Real getG() const;

    /**
     * @brief Get softening parameter
     * 
     * @return Real 
     */
    Real getEps2() const;

    /**
     * @brief add new body to system
     * 
     * @param body instance of Body2D to append to vector
     */
    void addBody(const Body2D &body);

    /**
     * @brief returns number of bodies in system
     * 
     * @return std::size_t number of bodies in system
     */
    std::size_t bodyCount() const;

    /**
     * @brief non-const access to body list
     * 
     * @return std::vector<Body2D>& 
     */
    std::vector<Body2D> &bodies();

    /**
     * @brief const access to body list
     * 
     * @return const std::vector<Body2D>& 
     */
    const std::vector<Body2D> &bodies() const;

    /**
     * @brief compute gravitational forces on all bodies
     * 
     * Steps:
     *      clear all force accumulators
     *      for each pair (i, j) compute gravitational force
     *          F = G * m_i * m_j * r_hat / (|r|^2 + eps2)^(3/2) and add +F to body i and -F to body j
     *          Complexity = O(n^2) for n bodies
     * 
     */
    void computeForces();
    /**
     * @brief compute total energy of the system
     * 
     * Energy = kinetic + potential
     *      kinetic = sum(0.5 * m * v^2) over all bodies
     *      potential = sum over i<j of (-G * m_i * m_j / |r_ij|)
     *      uses eps2 for softening
     * 
     * @return Real Total Energy
     */
    Real totalEnergy() const;
    /**
     * @brief advance system by one time step using euler
     * Algo:
     *      computeForces()
     *      a = F / m
     *      r_{n+1} = r_n + v_n * dt
     *      v_{n+1} = v_n + a * dt
     */
    void stepEuler(Real dt);
    /**
     * @brief advance system by one time step using semieuler
     *      * Algo:
     *      computeForces()
     *      a = F / m
     *      v_{n+1} = v_n + a * dt
     *      r_{n+1} = r_n + v_{n+1} * dt
     */
    void stepSemiEuler(Real dt);
    /**
     * @brief advance system by one time step using verlet
     * Algo:
     *      computeForces() to get acceleration a_old = F/m
     *      update positions using current velocity abd a_old:
     *          r_{n+1} = r_n + v_n * dt + 0.5 * a_old * dt^2
     *      computeForces() again to get acceleration a_new
     *      update velocities: 
     *          v_{n+1} = v_n + 0.5 * (a_old + a_new) * dt
     */
    void stepVerlet(Real dt);
    
private:
    std::vector<Body2D> m_bodies; // list of all simulated bodies
    Real m_G; // gravitation constant
    Real m_eps2; // softening parameter
};

#endif