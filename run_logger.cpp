// runlogger class, writes to csv

#include <fstream>
#include <string>
#include <vector>

#include "body2d.hpp"
#include "nbody_system2d.h"
#include "run_logger.h"

RunLogger::RunLogger() : m_trajOfs(), m_wroteHeader(false){}

bool RunLogger::open(const std::string &path){
    m_trajOfs.open(path);
    m_wroteHeader = false;
    return static_cast<bool>(m_trajOfs);
}

void RunLogger::writeHeader(const NBodySystem2D &system, bool includeEnergy){
    // if file is not open or header is written, do nothing
    if(!m_trajOfs || m_wroteHeader){
        return;
    }

    const std::size_t nBodies = system.bodyCount();

    // first column is time
    m_trajOfs << "t";
    // for each body, add x, y, vx, vy columns
    for(std::size_t i = 0; i < nBodies; ++i){
        const std::size_t index = i + 1;
        m_trajOfs << ",x" << index << ",y" << index << ",vx" << index << ",vy" << index;
    }
    // optional total energy column
    if(includeEnergy){
        m_trajOfs << ",E_total";
    }

    m_trajOfs << "\n";
    m_wroteHeader = true;
}

void RunLogger::logState(Real t, const NBodySystem2D &system, bool includeEnergy){
    // if file not open, do nothing
    if(!m_trajOfs){
        return;
    }
    // write time
    m_trajOfs << t;

    // wrute each body's position and velocity
    const std::vector<Body2D> &bodies = system.bodies();
    const std::size_t n = bodies.size();

    for(std::size_t i = 0; i < n; ++i){
        const Body2D &b = bodies[i];
        m_trajOfs << "," << b.r.x << "," << b.r.y << "," << b.v.x << "," << b.v.y;;
    }

    // optional totalEnergy
    if(includeEnergy){
        const Real energy = system.totalEnergy();
        m_trajOfs << "," << energy;
    }
    m_trajOfs << "\n";
}

void RunLogger::close(){
    if(m_trajOfs.is_open()){
        m_trajOfs.close();
    }
    m_wroteHeader = false;
}
