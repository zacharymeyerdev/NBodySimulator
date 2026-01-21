// simulationconfig class, reading and validating

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>

#include "simulation_config.h"

/**
 * @brief Construct a config with defaults
 *        overwritten by loadFromFile() as necessary
 */
SimulationConfig::SimulationConfig() : precision("long double"), method("verlet"), dt(static_cast<Real>(0)), steps(0), outputEvery(1), G(static_cast<Real>(1)), eps2(static_cast<Real>(0)), bodiesFile(), outTrajFile(), includeEnergy(false){}

/**
 * @brief load configuration values from a key=value text file
 * 
 * @param path path to config file
 * @return true if file was opened and parsed
 * @return false otherwise
 */
bool SimulationConfig::loadFromFile(const std::string &path){
    std::ifstream input(path);
    if(!input){
        return false;
    }
    std::string line;
    while(std::getline(input, line)){
        const std::string trimmed = trim(line);
        // skip empty lines
        if(trimmed.empty()){
            continue;
        }
        // skip comment lines
        if(trimmed[0] == '#'){
            continue;
        }
        // look for key=value
        const std::size_t eqPos = trimmed.find('=');
        if(eqPos == std::string::npos){
            continue;
        }
        const std::string key = trim(trimmed.substr(0, eqPos));
        const std::string value = trim(trimmed.substr(eqPos + 1));

        if(key == "precision"){
            precision = value;
        }
        else if(key == "method"){
            method = value;
        }
        else if(key == "dt"){
            dt = static_cast<Real>(std::stold(value));
        }
        else if(key == "steps"){
            steps = std::stoll(value);
        }
        else if(key == "outputEvery"){
            outputEvery = std::stoi(value);
        }
        else if(key == "G"){
            G = static_cast<Real>(std::stold(value));
        }
        else if(key == "eps2"){
            eps2 = static_cast<Real>(std::stold(value));
        }
        else if(key == "bodiesFile"){
            bodiesFile = value;
        }
        else if(key == "outTrajFile"){
            outTrajFile = value;
        }
        else if(key == "includeEnergy"){
            bool parsed = false;
            if(parseBool(value, parsed)){
                includeEnergy = parsed;
            }
        }
        // unknown keys ignored
    }
    return true;
}
/**
 * @brief validate configuration values are usable
 *        checks for positive dt, steps, and outputEvery
 * @param err stream to print error messages into
 * @return true if all checks pass
 * @return false otherwise
 */
bool SimulationConfig::validate(std::ostream &err) const{
    bool ok = true;
    if(method != "euler" && method != "semieuler" && method != "verlet"){
        err << "Method must be 'euler' or 'semieuler' or 'verlet'.\n";
        ok = false;
    }
    if(dt <= static_cast<Real>(0)){
        err << "dt must be greater than 0.\n";
        ok = false;
    }
    if(steps <= 0){
        err << "steps must be greater than 0.\n";
        ok = false;
    }
    if(outputEvery <= 0){
        err << "outputEvery must be greater than 0.\n";
        ok = false;
    }
    if(bodiesFile.empty()){
        err << "bodiesFile is empty.\n";
        ok = false;
    }
    if(outTrajFile.empty()){
        err << "outTrajFile is empty.\n";
        ok = false;
    }
    return ok;
}
/**
 * @brief trim leading and trailing whitespace from string
 * 
 * @param input raw string
 * @return std::string = copy with whitespace removed
 */
std::string SimulationConfig::trim(const std::string &input){
    std::size_t start = 0;
    while(start < input.size() && std::isspace(static_cast<unsigned char>(input[start])) != 0){
        ++start;
    }
    std::size_t end = input.size();
    while(end > start && std::isspace(static_cast<unsigned char>(input[end - 1])) != 0){
        --end;
    }
    return input.substr(start, end - start);
}
/**
 * @brief prase a boolean from a string
 * 
 * @param value string token to parse
 * @param out reference to where parsed bool will be stored
 * @return true if token was recognized as a boolean
 * @return false otherwise
 */
bool SimulationConfig::parseBool(const std::string &value, bool &out){
    std::string lower = value;
    for(std::size_t i = 0; i < lower.size(); ++i){
        lower[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(lower[i])));
    }
    if(lower == "true" || lower == "1" || lower == "yes"){
        out = true;
        return true;
    }
    if(lower == "false" || lower == "0" || lower == "no"){
        out = false;
        return true;
    }
    return false;
}