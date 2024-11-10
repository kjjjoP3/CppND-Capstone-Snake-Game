#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t gridWidth, std::size_t gridHeight)
    : snake(gridWidth, gridHeight),
      engine(dev()),
      random_w(0, static_cast<int>(gridWidth - 1)),
      random_h(0, static_cast<int>(gridHeight - 1)) {
  PlaceFood();
}

void Game::Run(const Controller &controller, Renderer &renderer,
               std::size_t targetFrameDuration) {
  Uint32 lastTitleUpdateTime = SDL_GetTicks();
  Uint32 frameStartTime;
  Uint32 frameEndTime;
  Uint32 frameTime;
  int frameCount = 0;
  bool isRunning = true;

  while (isRunning) {
    frameStartTime = SDL_GetTicks();

    // Main game loop: handle input, update state, render game
    controller.HandleInput(isRunning, snake);
    Update();
    renderer.Render(snake, food);

    frameEndTime = SDL_GetTicks();

    frameCount++;
    frameTime = frameEndTime - frameStartTime;

    // Update window title every second
    if (frameEndTime - lastTitleUpdateTime >= 1000) {
      renderer.UpdateWindowTitle(score, frameCount);
      frameCount = 0;
      lastTitleUpdateTime = frameEndTime;
    }

    // Delay if the frame time is less than the target frame duration
    if (frameTime < targetFrameDuration) {
      SDL_Delay(targetFrameDuration - frameTime);
    }
  }
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);

    // Ensure the food is placed in an empty space
    if (!snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::Update() {
  if (!snake.IsAlive()) return;

  snake.Update();

  int headX = static_cast<int>(snake.GetHead().x);
  int headY = static_cast<int>(snake.GetHead().y);

  // Check if the snake's head is on top of food
  if (food.x == headX && food.y == headY) {
    score++;
    PlaceFood();
    snake.GrowBody();

    // Optionally, adjust snake speed as it grows
    if (snake.SizeInBounds(25, 35)) {
      snake.ScaleSpeed(0.95);
    }
  }
}

int Game::GetScore() const { return score; }

int Game::GetSize() const { return snake.GetSize(); }
