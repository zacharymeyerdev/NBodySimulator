// nbodysystem2d class, vector+G+eps2, physics

#include "nbody_system2d.h"

#include <cmath>

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
/**
 * @brief default constructor
 * Initializes:
 *      G = 1
 *      eps2 = 0
 *      bodies list empty
 * 
 */
NBodySystem2D::NBodySystem2D() : m_bodies(), m_G(static_cast<Real>(1)), m_eps2(static_cast<Real>(0)){}
/**
 * @brief Construct with specified G and softening parameter
 * 
 * @param GValue gravitational constant
 * @param eps2Value softening value added to r^2
 */
NBodySystem2D::NBodySystem2D(Real GValue, Real eps2Value) : m_bodies(), m_G(GValue), m_eps2(eps2Value){}

/**
 * @brief set gravitational constant
 * 
 * @param GValue gravitational constant
 */
void NBodySystem2D::setG(Real GValue){
    m_G = GValue;
}

/**
 * @brief Set softening parameter
 *        prevents division by very small numbers
 * 
 * @param eps2Value softening parameter
 */
void NBodySystem2D::setEps2(Real eps2Value){
    m_eps2 = eps2Value;
}

/**
 * @brief Get gravitational constant
 * 
 * @return Real 
 */
Real NBodySystem2D::getG() const{
    return m_G;
}

/**
 * @brief Get softening parameter
 * 
 * @return Real 
 */
Real NBodySystem2D::getEps2() const{
    return m_eps2;
}

/**
 * @brief add new body to system
 * 
 * @param body instance of Body2D to append to vector
 */
void NBodySystem2D::addBody(const Body2D &body){
    m_bodies.push_back(body);
}

/**
 * @brief returns number of bodies in system
 * 
 * @return std::size_t number of bodies in system
 */
std::size_t NBodySystem2D::bodyCount() const{
    return m_bodies.size();
}

/**
 * @brief non-const access to body list
 * 
 * @return std::vector<Body2D>& 
 */
std::vector<Body2D> &NBodySystem2D::bodies(){
    return m_bodies;
}

/**
 * @brief const access to body list
 * 
 * @return const std::vector<Body2D>& 
 */
const std::vector<Body2D> &NBodySystem2D::bodies() const{
    return m_bodies;
}

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
void NBodySystem2D::computeForces(){
    const std::size_t n = m_bodies.size();
    // clear existing forces
    for(std::size_t i = 0; i < n; ++i){
        m_bodies[i].clearForce();
    }
    // pairwise interaction loop, and i < j to avoid duplicates
    for(std::size_t i = 0; i < n; ++i){
        for(std::size_t j = i + 1; j < n; ++j){
            // displacement from i to j
            Vec2 dr = m_bodies[j].r.sub(m_bodies[i].r);

            // squared distance with softening
            Real dist2 = dr.x * dr.x + dr.y * dr.y + m_eps2;
            // 1 / |r| and 1 / |r|^3
            Real invDist = static_cast<Real>(1) / static_cast<Real>(std::sqrt(dist2));
            Real invDist3 = invDist * invDist * invDist;

            // Magnitude of gravitational force
            Real forceMag = m_G * m_bodies[i].m * m_bodies[j].m * invDist3;
            
            // force vector in direction of dr
            Vec2 F = dr.scale(forceMag);

            // apply equal and opposite forces
            m_bodies[i].addForce(F);
            m_bodies[j].addForce(F.scale(static_cast<Real>(-1)));
        }
    }
}
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
Real NBodySystem2D::totalEnergy() const{
    const std::size_t n = m_bodies.size();

    Real kinetic = static_cast<Real>(0);
    Real potential = static_cast<Real>(0);

    // kinetic energy = 0.5 * m * |v|^2
    for(std::size_t i = 0; i < n; ++i){
        const Body2D &b = m_bodies[i];
        Real v2 = b.v.x * b.v.x + b.v.y * b.v.y;
        kinetic += static_cast<Real>(0.5) * b.m * v2;
    }

    // potential energy = -G * m_i * m_j / |r_ij|
    for(std::size_t i = 0; i < n; ++i){
        for(std::size_t j = i + 1; j < n; ++j){
            Vec2 dr = m_bodies[j].r.sub(m_bodies[i].r);
            Real dist2 = dr.x * dr.x + dr.y * dr.y + m_eps2;
            Real dist = static_cast<Real>(std::sqrt(dist2));

            if(dist > static_cast<Real>(0)){
                potential -= m_G * m_bodies[i].m * m_bodies[j].m / dist;
            }
        }
    }
    return kinetic + potential;
}
/**
 * @brief advance system by one time step using euler
 * Algo:
 *      computeForces()
 *      a = F / m
 *      r_{n+1} = r_n + v_n * dt
 *      v_{n+1} = v_n + a * dt
 */
void NBodySystem2D::stepEuler(Real dt){
    computeForces();

    const std::size_t n = m_bodies.size();
    for(std::size_t i = 0; i < n; ++i){
        Body2D &b = m_bodies[i];

        Real ax = b.f.x / b.m;
        Real ay = b.f.y / b.m;

        // update positions using current velocity
        b.r.x += b.v.x * dt;
        b.r.y += b.v.y * dt;

        // update velocity using current acceleration
        b.v.x += ax * dt;
        b.v.y += ay * dt;
    }
}
/**
 * @brief advance system by one time step using semieuler
 *      * Algo:
 *      computeForces()
 *      a = F / m
 *      v_{n+1} = v_n + a * dt
 *      r_{n+1} = r_n + v_{n+1} * dt
 */
void NBodySystem2D::stepSemiEuler(Real dt){
    computeForces();

    const std::size_t n = m_bodies.size();
    for(std::size_t i = 0; i < n; ++i){
        Body2D &b = m_bodies[i];

        Real ax = b.f.x / b.m;
        Real ay = b.f.y / b.m;

        // update velocity
        b.v.x += ax * dt;
        b.v.y += ay * dt;

        // update position using new velocity
        b.r.x += b.v.x * dt;
        b.r.y += b.v.y * dt;
    }
}
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
void NBodySystem2D::stepVerlet(Real dt){
    const std::size_t n = m_bodies.size();
    if(n==0){
        return;
    }
    // first force evaluation to get old accelerations
    computeForces();

    // store old accelerations
    std::vector<Vec2> aOld(n);
    for(std::size_t i = 0; i < n; ++i){
        const Body2D &b = m_bodies[i];
        aOld[i].x = b.f.x / b.m;
        aOld[i].y = b.f.y / b.m;
    }

    // update positions with current velocities and aOld
    for(std::size_t i = 0; i < n; ++i){
        Body2D &b = m_bodies[i];

        b.r.x += b.v.x * dt + static_cast<Real>(0.5) * aOld[i].x * dt * dt;
        b.r.y += b.v.y * dt + static_cast<Real>(0.5) * aOld[i].y * dt * dt;
    }
    // compute forces at new positions to get new accelerations
    computeForces();
    // update velocities with average of old and new accelerations
    for(std::size_t i = 0; i < n; ++i){
        Body2D &b = m_bodies[i];
        Vec2 aNew;
        aNew.x = b.f.x / b.m;
        aNew.y = b.f.y / b.m;
        b.v.x += static_cast<Real>(0.5) * (aOld[i].x + aNew.x) * dt;
        b.v.y += static_cast<Real>(0.5) * (aOld[i].y + aNew.y) * dt;
    }
}