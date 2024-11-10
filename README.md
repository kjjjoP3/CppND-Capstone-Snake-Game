# CPPND: Capstone Snake Game Example

This is a starter repo for the Capstone project in the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). The code for this repo was inspired by [this](https://codereview.stackexchange.com/questions/212296/snake-game-in-c-with-sdl) excellent StackOverflow post and set of responses.

<img src="snake_game.gif"/>

The Capstone Project gives you a chance to integrate what you've learned throughout this program. This project will become an important part of your portfolio to share with current and future colleagues and employers.

In this project, you can build your own C++ application or extend this Snake game, following the principles you have learned throughout this Nanodegree Program. This project will demonstrate that you can independently create applications using a wide range of C++ features.

## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  >Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source. 
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./SnakeGame`.


## CC Attribution-ShareAlike 4.0 International


Shield: [![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

This work is licensed under a
[Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].

[![CC BY-SA 4.0][cc-by-sa-image]][cc-by-sa]

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-image]: https://licensebuttons.net/l/by-sa/4.0/88x31.png
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg

# Snake Game

## Introduction

This is a simple Snake game developed in C++ using SDL2 for graphics rendering. The game allows the player to control a snake, eat food, grow longer, and avoid collisions. Additionally, the game features a variety of gameplay mechanics, including adjustable difficulty levels, high score tracking, and more.

## Features

### Game Mechanics

- **Movement**: Use the arrow keys (`Up`, `Down`, `Left`, `Right`) to control the snake's movement.
- **Eating Food**: The snake grows longer by eating food, represented by yellow squares.
- **Game Over**: The game ends if the snake hits the wall or collides with its own body.
- **High Scores**: The high scores are saved to `highScore.txt` and displayed at the end of the game.

### Difficulty Levels

- **Easy**: No walls or poison food.
- **Normal**: Includes walls; the snake will die if it hits the wall.
- **Hard**: Adds poison food with a 20% chance, and toxic food that poisons the snake for 5 seconds. While poisoned, the snake moves in the opposite direction of the player's input.

### Pause Feature

- The game can be paused and resumed by pressing the `ESC` key.

### Toxic Food

- Toxic food appears at random and has a 20% chance to appear when the snake eats food.
- When the snake eats toxic food, it becomes poisoned for 5 seconds.
- During the poisoned state, the snake will move in the opposite direction (e.g., pressing the `UP` key will make the snake go `DOWN`).

### Modal Box

- When the game ends, a dialog box appears showing the player's score and snake size information.

## Requirements

- **C++ (C++11 or newer)**
- **SDL2 library**

### Installing SDL2

#### On Ubuntu (or similar Linux OS):

```bash
sudo apt-get update
sudo apt-get install libsdl2-dev
