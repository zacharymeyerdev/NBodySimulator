/* Course: CSCI 200
 * Lab: Final Project - N-Body Simulator
 * Name: Zachary Meyer
 * Author: zachary_meyer
 * Description: 
 *      The purpose of this program is to simulate the physical interactions between 3 or more bodies.
 *      It does this by loading pre-established bodies from the "bodies.csv" file, 
 *      loading the simulation configs from the "config.txt" file, beginning to calculate positions and movement,
 *      eolving the system using integrators, then logging the state, and visualizing the motion using SFML.
*/

// time-stepping loop

#include <iostream>
#include <string>
#include <cctype>
#include <SFML/Graphics.hpp>
#include <random>
#include <cstdint>

#include "real_type.hpp"
#include "vec2.hpp"
#include "body2d.hpp"
#include "nbody_system2d.h"
#include "simulation_config.h"
#include "body_io.h"
#include "run_logger.h"

int main(int argc, char *argv[]){   
    // determine config file path
    // allowed to override by passing a filename as the first command line argument
    std::string configPath = "config.txt";
    if(argc > 1){
        configPath = argv[1];
    }
    // load and validate simulation config
    SimulationConfig cfg;
    if(!cfg.loadFromFile(configPath)){
        std::cerr << "Unable to read config file " << configPath << ".\n";
        return 1;
    }
    if(!cfg.validate(std::cerr)){
        return 1;
    }

    // construct n-body system with G and softening
    NBodySystem2D system(cfg.G, cfg.eps2);

    // load body initial conditions from csv
    if(!loadBodiesFromCsv(cfg.bodiesFile, system)){
        return 1;
    }
    if(system.bodyCount() == 0){
        std::cerr << "Simulation has no bodies loaded.\n";
        return 1;
    }

    // set up run logger to write trajectories to CSV
    RunLogger logger;
    if(!logger.open(cfg.outTrajFile)){
        std::cerr << "Could not open output file " << cfg.outTrajFile << ".\n";
    }

    // write header line with time, positions, velocities, and energy if flagged
    logger.writeHeader(system, cfg.includeEnergy);

    // normalize method string
    std::string method = cfg.method;
    for(std::size_t i = 0; i < method.size(); ++i){
        method[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(method[i])));
    }

    // initial time and first log
    Real t = static_cast<Real>(0);
    logger.logState(t, system, cfg.includeEnergy);

    // print config summary
    std::cout << "Configuration loaded.\n";
    std::cout << "precision = " << cfg.precision << "\n";
    std::cout << "method = " << cfg.method << "\n";
    std::cout << "dt = " << static_cast<double>(cfg.dt) << "\n";
    std::cout << "steps = " << cfg.steps << "\n";
    std::cout << "bodiesFile = " << cfg.bodiesFile << "\n";
    std::cout << "outTrajFile = " << cfg.outTrajFile << "\n";
    std::cout << "includeEnergy = " << (cfg.includeEnergy ? "true" : "false");
    
    // SFML
    // window dimension in pixels
    const unsigned int windowWidth = 800U;
    const unsigned int windowHeight = 800U;

    // create VideoMode and RenderWindow
    sf::Vector2u size(windowWidth, windowHeight);
    sf::VideoMode mode(size);
    sf::RenderWindow window(mode, "N-Body Simulator");
    window.setFramerateLimit(60U);

    // scale factor from simulation units to screen pixels
    const float viewScale = 200.0f;

    // convert simulation coordinates (x, y) to screen coordinates
    // simulation origin = (0, 0) put to center of window
    // y-axis inverted so y points up
    const auto toScreen = [&](Real x, Real y) -> sf::Vector2f{
        const float sx = static_cast<float>(x) * viewScale + static_cast<float>(windowWidth) / 2.0f;
        const float sy = -static_cast<float>(y) * viewScale + static_cast<float>(windowHeight) / 2.0f;
        return sf::Vector2f(sx, sy);
    };

    // pre-create circle shapes for each body
    // colored by index
    const std::size_t bodyCount = system.bodyCount();
    std::vector<sf::CircleShape> bodyShapes;
    bodyShapes.reserve(bodyCount);

    // random color gen
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> colorDist(50, 255);

    for(std::size_t i = 0; i < bodyCount; ++i){
        sf::CircleShape circle;
        const float radius = 16.0f;
        circle.setRadius(radius);
        // set origin to center
        circle.setOrigin(sf::Vector2f(radius, radius));
        // color setting
        if(i == 0U){
            circle.setFillColor(sf::Color::Red);
        }
        else if(i == 1U){
            circle.setFillColor(sf::Color::Green);
        }
        else if(i==2U){
            circle.setFillColor(sf::Color::Blue);
        }
        else{
            // outside of the three main bodies, set to random color
            const std::uint8_t r = static_cast<std::uint8_t>(colorDist(rng));
            const std::uint8_t g = static_cast<std::uint8_t>(colorDist(rng));
            const std::uint8_t b = static_cast<std::uint8_t>(colorDist(rng));
            circle.setFillColor(sf::Color(r, g, b));
        }
        bodyShapes.push_back(circle);
    }

    // step counter for simulation loop
    long long step = 0;

    // while the window is open, simulator:
        // handle window events
        // advanced n-body system by one time step
        // log state every outputEvery steps
        // draw each body as a colored circle to its position
            
    while(window.isOpen() && step < cfg.steps){
        // pollEvent() returns pointer-like object which gets dereferenced
        while(auto event = window.pollEvent()){
            // check for window close event
            if(event->is<sf::Event::Closed>()){
                window.close();
                }
            }
        // time integration
        if(method == "euler"){
            system.stepEuler(cfg.dt);
        }
        else if(method == "semieuler"){
            system.stepSemiEuler(cfg.dt);
        }
        else{
            system.stepVerlet(cfg.dt);
        }

        t += cfg.dt;
        ++step;

        // log the state to CSV every outputEvery steps
        if(step % cfg.outputEvery == 0){
            logger.logState(t, system, cfg.includeEnergy);
        }

        // rendering
        window.clear(sf::Color::Black);
        // get const reference to the bodies for drawing
        const std::vector<Body2D> &bodies = system.bodies();
        for(std::size_t i = 0; i < bodyCount; ++i){
            const Body2D &b = bodies[i];
            const sf::Vector2f screenPos = toScreen(b.r.x, b.r.y);
            bodyShapes[i].setPosition(screenPos);
            window.draw(bodyShapes[i]);
        }
        // draw frame on screen
        window.display();
    }

    // close + summary

    logger.close();

    std::cout << "Simulation finished.\n";
    std::cout << "Steps: " << cfg.steps << ", dt: " << static_cast<double>(cfg.dt) << ", method: " << cfg.method << "\n";
    std::cout << "Output written to " << cfg.outTrajFile << ".\n";

    return 0;
}