// runlogger class, writes to csv

#ifndef RUN_LOGGER_HPP
#define RUN_LOGGER_HPP

#include <fstream>
#include <string>
#include <vector>

#include "real_type.hpp"

class NBodySystem2D;

/**
 * @brief declares and defines RunLogger, helper class for CSV output
 *        writes time evolution of NBodySystem2D to a CSV file
 *        writes header row, then appends one row each time logState() is created
 * 
 * Columns:
 *  t,
 *  x1,y1,vx1,vy1
 *  x2,y2,vx2,vy2
 *  ...
 *  x3,y3,vx3,vy3
 * [E_total] (optional)
 */
class RunLogger{
public:
    /**
     * @brief construct a closed logger
     * 
     */
    RunLogger();
    /**
     * @brief open csv file for writing
     * 
     * @param path file path to open
     * @return true if file stream is valid and available for writing
     * @return false otherwise
     */
    bool open(const std::string &path);
    /**
     * @brief write csv header row
     *        writes once per file
     * 
     * @param system the NBodySystem2D with bodies to define columns
     * @param includeEnergy if true, append E_total column at the end
     */
    void writeHeader(const NBodySystem2D &system, bool includeEnergy);
    /**
     * @brief append a single simulation state row to csv file
     *        Writes:
     *          t,
     *          x1,y1,vx1,vy1,
     *          x2,y2,vx2,vy2,
     *          ...
     *          x3,y3,vx3,vy3,
     *          [E_total]
     * @param t current simulation time
     * @param system current N-body system state
     * @param includeEnergy if true, append totalEnergy() to last column
     */
    void logState(Real t, const NBodySystem2D &system, bool includeEnergy);
    /**
     * @brief close output file and reset state
     * 
     */
    void close();
private:
    std::ofstream m_trajOfs; // output file stream
    bool m_wroteHeader; // tracks whether header row has been written
};

#endif