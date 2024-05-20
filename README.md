# Simulated-Annealing Cell Placement Tool

This repository contains a simple cell placement tool that places digital cells in a grid to minimize the total wire length of the nets. The tool uses the simulated annealing algorithm to achieve this.

## Table of Contents
1. [Overview](#overview)
2. [Command Line Input and Dependencies](#command-line-input-and-dependencies)
3. [Running the Program](#running-the-program)

## Overview
In this project, we implement a simple cell placement tool that places the digital cells in a grid in order to minimize the total wire length of the nets. The cells and their net connections are given. To do that, we use the simulated annealing algorithm.

## Command Line Input and Dependencies
To run the program, you need the following:
* C++ Compiler (we recommend GCC)
* Python 3.x
* Python Libraries: PIL (Python Imaging Library); it is needed for the gif generation.

## Running the Program
To run the program, follow these steps:
1. Compile the program using the following command:
```
g++ -O3 -o program main.cpp
```
2. Run the program with the input file and cooling rate as arguments. For example, to run the program on input file "d1.txt" with cooling rate "0.95", use the following command:
```
./program d1.txt 0.95
```
3. To see the process of swapping through an animation gif, add the flag -s to the command line. For example:
```
./program d1.txt 0.95 -s
```
This will generate a gif along with the initial random grid, its cost, the final grid, its cost, and the final temperature.
