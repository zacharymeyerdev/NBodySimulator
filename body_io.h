// load bodies from csv, lines=mass+x+y+vx+vy

#ifndef BODY_IO_H
#define BODY_IO_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>
#include <cmath>

#include "nbody_system2d.h"

class NBodySystem2D;
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
// reads CSV file and adds bodies to given NBodySystem2D
bool loadBodiesFromCsv(const std::string &path, NBodySystem2D &system);

#endif