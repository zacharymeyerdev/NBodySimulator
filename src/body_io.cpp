// load bodies from csv, lines=mass+x+y+vx+vy

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>
#include <cmath>

#include "nbody_system2d.h"
#include "body_io.h"
#include "real_type.hpp"
#include "body2d.hpp"
#include "vec2.hpp"

// reads CSV file and adds bodies to given NBodySystem2D
/**
 * @brief read body data from csv and add them to the simulation system
 *         Expected formats: 
 *              1. mass,x,y,vx,vy for velocity components
 *              2. mass,x,y,speed,direction_deg for magnitude + direction converted to (vx, vy)
 *         Skipped if empty, begins with '#', has fewer than 5 tokens, or fails parsing
 * @param path path to CSV file
 * @param system reference to NBodySystem2D to which bodies are added to
 * @return true if file could be open
 * @return false otherwise
 */
bool loadBodiesFromCsv(const std::string &path, NBodySystem2D &system){
    // try opening file for reading
    std::ifstream input(path);

    if(!input){
        std::cerr << "Could not open bodies file " << path << ".\n";
        return false;
    }

    // pi precision to 50 decimals
    const long double PI = 3.14159265358979323846264338327950288419716939937510L;

    std::string line;
    int validCount = 0; // count how many lines made a valid body

    // process file line by line
    while(std::getline(input, line)){
        // skip empty lines
        if(line.empty()){
            continue;
        }
        // skip comment starting with '#'
        if(line[0] == '#'){
            continue;
        }

        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;

        // split line on commas into tokens
        while(std::getline(ss, token, ',')){
            // remove any '\r's
            if(!token.empty() && token.back() == '\r'){
                token.pop_back();
            }
            tokens.push_back(token);
        }
        // requires at least 5 tokens
        if(tokens.size() < 5){
            continue;
        }
        try{
            // parse mass and position
            const Real mass = static_cast<Real>(std::stold(tokens[0]));
            const Real x = static_cast<Real>(std::stold(tokens[1]));
            const Real y = static_cast<Real>(std::stold(tokens[2]));

            // velocity components to be filled
            Real vx = static_cast<Real>(0);
            Real vy = static_cast<Real>(0);

            if(tokens.size() == 5){
                // for mass,x,y,vx,vy
                vx = static_cast<Real>(std::stold(tokens[3]));
                vy = static_cast<Real>(std::stold(tokens[4]));
            }
            else{
                // for mass,x,y,speed,direction_deg, etc
                // convert speed + angle to vx,vy
                const long double speed = std::stold(tokens[3]);
                const long double directionDeg = std::stold(tokens[4]);
                const long double directionRad = directionDeg * (PI / 180.0L);

                vx = static_cast<Real>(speed * std::cos(directionRad));
                vy = static_cast<Real>(speed * std::sin(directionRad));
            }
            // construct Body2D and addit to NBodySystem2D
            Body2D body(mass, Vec2(x, y), Vec2(vx, vy));
            system.addBody(body);
            ++validCount;
        }
        // if anything fails to parse, skip
        catch(const std::exception &){
            continue;
        }
    }
    // always return true if file opened even if invalid
    return true;
}
