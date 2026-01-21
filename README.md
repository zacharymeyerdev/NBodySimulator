# NBodySimulator

A Newtonian 2D N-body gravitational simulator written in C++. Given a gravitational constant `G`, a timestep `dt`, a number of steps, and a set of bodies (mass, initial position, initial velocity), the program integrates motion under mutual gravity and visualizes it in real time using **SFML**.

It supports multiple integrators to compare stability/accuracy:
- Euler
- Semi-Implicit Euler (SemiEuler)
- Verlet

Optionally, the simulator can compute and log **total system energy** to demonstrate approximate energy conservation in a closed system. Trajectories are written to a CSV file for later analysis.

---

## Features
- 2D Newtonian gravity with softening (`eps2`) for numerical stability at close distances
- Three integration methods: `euler`, `semieuler`, `verlet`
- Optional total energy tracking/logging
- Reproducible runs via `config.txt` + `bodies.csv`
- Real-time visualization with SFML
- CSV trajectory output for plotting/analysis

---

## Build

From the project directory:
`make`

---

## Run

Default config:
`./NBodySimulator`

Custom config file:
`./NBodySimulator myconfig.txt`

An SFML window will open to display the bodies’ motion. A trajectories CSV will be generated based on your config.

---

## Configuration

Edit `config.txt` (or your custom config file) to set:

`method` = `euler` | `semieuler` | `verlet`

`dt` = `timestep`

`steps` = total simulation steps

`outputEvery` = how often to log output (in steps)

`G` = gravitational constant

`eps2` = softening parameter (added to distance^2)

`bodiesFile` = input CSV of initial conditions (e.g. `bodies.csv`)

`outTrajFile` = output CSV for trajectories (e.g. `trajectories.csv`)

`includeEnergy` = `true` | `false`

---

## Bodies File Format

bodies.csv contains one body per line:

`mass,x,y,vx,vy`

Example:

`10,0,0,0,0`

`1,100,0,0,1`
