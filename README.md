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

# CPPND Capstone Project: Snake Game

This project is a submission for the Udacity C++ Nanodegree Capstone Project, building upon the starter code provided by Udacity. The base project was inspired by a StackOverflow post, and the game mechanics resemble the classic Snake game. This README outlines the added features, instructions for building and running the game, and explains how the rubric points are met in this project.

## Game Enhancements and Modifications

Several key features have been introduced or modified from the starter code to improve gameplay:

1. **Boundary Walls**  
   The game now features solid boundaries on the edges, acting as walls. The snake must turn away upon contact with a wall or its own body. If a turn is not possible, the game ends. This design was inspired by the Google Snake game.

2. **Automatic Turning**  
   When the snake encounters its own body or a wall, it automatically attempts to turn left. If a left turn is not feasible, a right turn is attempted. The player can always override this automated behavior by providing a direction input.

3. **Dynamic Speed Adjustment**  
   Speed remains constant initially but slows down slightly as the snake’s length grows beyond 25 cells. Specifically, each time the snake grows, the speed decreases by 5% until the snake reaches 35 cells, after which the speed stays fixed. This adjustment was found to improve gameplay experience by balancing challenge and control.

4. **Improved Visuals**  
   The snake's body is now rendered with a clear outline to enhance visual distinction between segments.

## Building and Running the Project

### Prerequisites
- **C++ compiler** (e.g., g++)
- **SDL2 library** for handling rendering and input (cross-platform installation instructions are provided below)

### Cross-Platform Installation Instructions

- **Ubuntu**: 
  ```bash
  sudo apt-get update
  sudo apt-get install libsdl2-dev


Mac:

bash
Copy code
brew install sdl2
Windows:
Download and install SDL2 development libraries from SDL's official website. Follow platform-specific instructions for setup.

Building and Running the Game
Clone the repository:

bash
Copy code
git clone https://github.com/kjjjoP3/CppND-Capstone-Snake-Game
Build the project:

bash
Copy code
mkdir build
cd build
cmake ..
make
Run the game:

bash
Copy code
./SnakeGame
New Features and Expected Behavior
Boundary Walls: The game does not wrap around; instead, walls act as barriers. Colliding with a wall requires a turn or ends the game if a turn is not possible.
Automatic Turning: The snake will turn left or right as needed to avoid obstacles when possible.
Dynamic Speed: The snake's speed decreases slightly after reaching a length of 25 cells, giving the player better control as the snake grows longer.
Enhanced Visuals: Clear outlining improves segment visibility, helping players track the snake's path more easily.
Rubric Points Addressed
The following rubric items are implemented in this project. Each item is linked to specific code sections, including file names and line numbers where relevant.

1. Functions and Control Structures
Requirement: Demonstrate an understanding of C++ functions and control structures.
Code Reference: See renderer.cpp at lines 78, 94, 110, and 150. Functions were added, including RenderBody, which iterates backward over a std::vector.
2. User Input Processing
Requirement: Accept and process user input.
Code Reference: snake.cpp, line 195 in the new ChangeDirection function. Input handling has been expanded beyond simple directional changes.
3. Object-Oriented Programming (OOP)
Requirement: Use OOP techniques.
Code Reference: New Point class introduced. In snake.h, lines 30-40, data members in the Snake class are now private with public getters/setters.
4. Appropriate Access Specifiers
Requirement: Use appropriate access specifiers.
Code Reference: snake.h, lines 30-40. All data members in Snake are private, aligning with encapsulation principles.
5. Member Initialization Lists
Requirement: Utilize member initialization lists.
Code Reference: Used in class constructors throughout the project.
6. Encapsulation
Requirement: Encapsulate behavior within classes.
Code Reference: See Snake class in snake.h. All data members are private, providing encapsulation.
7. Function Overloading
Requirement: Overload functions to handle different parameters.
Code Reference: renderer.cpp, lines 78-100, where Renderer::Direction is overloaded.
8. References in Function Declarations
Requirement: Use references in function declarations.
Code Reference: Functions Renderer::RenderBlock and Renderer::RenderBody take parameters by reference (renderer.h, lines 16-21). The getter for Snake::body also returns a const reference.
