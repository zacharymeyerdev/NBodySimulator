# NBodySimulator

A Newtonian 2D N-body gravitational simulator written in C++. Given a gravitational constant `G`, a timestep `dt`, a number of steps, and a set of bodies (mass, initial position, initial velocity), the program integrates motion under mutual gravity and visualizes it in real time using **SFML**.

It supports multiple integrators to compare stability/accuracy:
- Euler
- Semi-Implicit Euler (SemiEuler)
- Verlet

Optionally, the simulator can compute and log **total system energy** to demonstrate approximate energy conservation in a closed system. Trajectories are written to a CSV file for later analysis.
