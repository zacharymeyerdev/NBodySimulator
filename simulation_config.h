// simulationconfig class, reading and validating

#ifndef SIMULATION_CONFIG_H
#define SIMULATION_CONFIG_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>

#include "real_type.hpp"

/**
 * @brief declares SimulationConfig, which owns all high level run parameters
 * 
 * Holds all user configuration settings (time step, method, files, etc.)
 * loading the settings from key=value text file
 * validating that loaded settings are usable
 * 
 * config.txt example:
 *      precision = long long
 *      method = verlet
 *      dt = 0.01
 *      steps = 100000
 *      outputEvery = 100
 *      G = 1.0
 *      eps2 = 0.0001
 *      bodiesFile = bodies.csv
 *      outTrajFile = trajectories.csv
 *      includeEnergy = true
 * 
 * Lines starting with '#' or blank lines are ignored
 */
class SimulationConfig{
public:
    std::string precision; // precision
    std::string method; // method name

    Real dt; // time step size for each integration step
    long long steps; // number of time steps to run simulation
    int outputEvery; // how many steps between each csv log write

    Real G; // gravitational constant
    Real eps2; // softening term

    std::string bodiesFile; // path to csv file with initial body conditions
    std::string outTrajFile; // path to csv file to store trajectory output

    bool includeEnergy; // whether or not to include total energy in csv output

    /**
     * @brief Construct a config with defaults
     *        overwritten by loadFromFile() as necessary
     */
    SimulationConfig();
    
    /**
     * @brief load configuration values from a key=value text file
     * 
     * @param path path to config file
     * @return true if file was opened and parsed
     * @return false otherwise
     */
    bool loadFromFile(const std::string &path);
    /**
     * @brief validate configuration values are usable
     *        checks for positive dt, steps, and outputEvery
     * @param err stream to print error messages into
     * @return true if all checks pass
     * @return false otherwise
     */
    bool validate(std::ostream &err) const;
private:
    /**
     * @brief trim leading and trailing whitespace from string
     * 
     * @param input raw string
     * @return std::string = copy with whitespace removed
     */
    static std::string trim(const std::string &input);
    /**
     * @brief prase a boolean from a string
     * 
     * @param value string token to parse
     * @param out reference to where parsed bool will be stored
     * @return true if token was recognized as a boolean
     * @return false otherwise
     */
    static bool parseBool(const std::string &value, bool &out);
};

#endif