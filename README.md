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

This is a classic Snake game developed using C++ and SDL2. The game allows the player to control a snake, eat food, grow longer, and avoid obstacles. New features such as difficulty levels, high score tracking, and poison food have been added to enhance the gameplay experience.

## Features

### New Features Added:
1. **Difficulty Levels**:
   - The player can choose from three difficulty levels during the game:
     - **Easy**: No walls or poison food.
     - **Normal**: Includes walls. The snake dies if it hits the walls.
     - **Hard**: Adds poison food with a 20% chance of spawning. Toxic food will poison the snake, making it move in the opposite direction for 5 seconds.
   
2. **Pause and Resume**:
   - Press `ESC` to pause or resume the game during play.

3. **High Score Tracking**:
   - The game reads and writes high scores to `highScore.txt`.

4. **Poison Food (Hard Level)**:
   - At the hard level, toxic food has a 20% chance of appearing. If eaten, the snake becomes poisoned for 5 seconds, during which the snake will move in the opposite direction of the player's input.

5. **Modal Dialog**:
   - A modal dialog appears when the game ends, displaying the score and snake size.

### Expected Behavior

- **Movement**: The snake moves according to the arrow key inputs. The snake grows longer as it eats food.
- **Game Over**: The game ends if the snake collides with the walls or its own body.
- **Difficulty Levels**: The difficulty affects game speed and obstacles. Higher difficulty includes walls and poison food.
- **High Score**: The highest score is saved in the `highScore.txt` file and displayed when the game ends.
- **Pause**: Press `ESC` to pause the game and resume it later.
- **Poison Food**: On hard difficulty, a 20% chance exists for a toxic food to appear. If eaten, the snake moves in the opposite direction for 5 seconds.

## Requirements

- **C++ (C++11 or newer)**
- **SDL2** (Simple DirectMedia Layer)
  
### Installing SDL2

#### On Ubuntu (or similar Linux OS):

```bash
sudo apt-get update
sudo apt-get install libsdl2-dev
On Windows:
Download SDL2 from SDL2 Download Page.
Extract and set up SDL2 in your project as per the SDL2 documentation.
Installation and Compilation
To compile and run the game, follow the steps below:

Clone the repository:

bash
Copy code
git clone [https://github.com/yourusername/snake-game.git](https://github.com/kjjjoP3/CppND-Capstone-Snake-Game)
cd snake-game
Compile the Game:

Run the following command to compile the game:

bash
Copy code
g++ -o snake_game game.cpp snake.cpp renderer.cpp highscore.cpp player.cpp controller.cpp -lSDL2 -std=c++11
Run the Game:

After compilation, run the game with the command:

bash
Copy code
./snake_game
The game window should appear, and you can begin playing!

Code Structure
game.cpp: Manages the overall game flow, including score tracking, difficulty levels, and game-over conditions.
snake.cpp: Contains logic for controlling the snake’s movement, growth, and collision detection.
renderer.cpp: Handles the rendering of the game, including drawing the snake, food, and updating the game window.
highscore.cpp: Responsible for saving and loading the high score to/from highScore.txt.
player.cpp: Stores information about the player, including score and size.
controller.cpp: Handles user input and controls the snake's movement.
Rubric Points Addressed
Functions and Control Structures:

The project demonstrates an understanding of C++ functions and control structures.
Files and Line Numbers:
game.cpp, line 82: PlaceFood() function.
snake.cpp, line 5: SetSpeed() function.
User Input and Processing:

The project accepts user input for controlling the snake and setting the snake’s speed.
Files and Line Numbers:
snake.cpp, line 5: SetSpeed() function allows the user to input their desired snake speed.
Object-Oriented Programming:

Classes are used to organize the game logic with appropriate access specifiers for class members.
Files and Line Numbers:
snake.cpp, snake.h, game.cpp, game.h: All these files use proper object-oriented principles.
Reading and Writing of High Score:

The game reads and writes the high score to highScore.txt.
Files and Line Numbers:
highscore.cpp, lines 10-50: Handles the reading and writing of the high score.
Pause and Resume:

The game allows the user to pause and resume the game by pressing ESC.
Files and Line Numbers:
game.cpp, lines 120-140: Handles pausing and resuming the game.
Toxic Food (Hard Difficulty):

In Hard mode, toxic food appears with a 20% chance, and the snake is poisoned if eaten.
Files and Line Numbers:
snake.cpp, lines 90-120: Logic for handling poison food and the poisoned state.
How to Play
Control the Snake: Use the arrow keys (Up, Down, Left, Right) to control the snake’s movement.
Eat Food: Eat the yellow food to grow longer and increase your score.
Avoid Collisions: The snake dies if it collides with the walls or its own body.
Pause/Resume: Press ESC to pause the game and resume it.
Select Difficulty: Press 1, 2, or 3 to select Easy, Normal, or Hard difficulty.
Game Over: When the snake dies, you can view your score and the high score in the modal dialog.
