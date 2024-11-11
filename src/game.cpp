#include "game.h"
#include <iostream>
#include <future>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "SDL.h"

Game::Game(std::size_t gridWidth, std::size_t gridHeight)
    : snake(gridWidth, gridHeight),
      engine(dev()),
      random_w(0, static_cast<int>(gridWidth - 1)),
      random_h(0, static_cast<int>(gridHeight - 1)),
      food(std::make_unique<SDL_Point>()),
      scorePromise(std::make_shared<std::promise<int>>())  // Shared promise for score
{
  PlaceFood();
  // Start a background thread for handling score updates
  scoreFuture = scorePromise->get_future();
  std::thread(&Game::ScoreUpdateThread, this).detach();  // Detach the score update thread
}

Game::~Game() = default;

// Thread for updating the score
void Game::ScoreUpdateThread() {
  std::unique_lock<std::mutex> lock(mutex);
  while (true) {
    // Wait until notified to update the score
    condition.wait(lock, [this] { return scoreUpdated; });
    scorePromise->set_value(score);  // Send score via promise
    scoreUpdated = false;  // Reset update flag
    scorePromise = std::make_shared<std::promise<int>>();  // Renew promise for future updates
    scoreFuture = scorePromise->get_future();
  }
}

void Game::Run(const Controller &controller, Renderer &renderer, std::size_t targetFrameDuration) {
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
    renderer.Render(snake, *food);  // Dereference food to pass by reference

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

// PlaceFood function for food positioning with unique_ptr
void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);

    // Ensure the food is placed in an empty space
    if (!snake.SnakeCell(x, y)) {
      food->x = x;  // Use unique_ptr to access food data
      food->y = y;
      return;
    }
  }
}

// Update function to modify game state and handle snake growth and score updates
void Game::Update() {
  if (!snake.IsAlive()) return;

  snake.Update();

  int headX = static_cast<int>(snake.GetHead().x);
  int headY = static_cast<int>(snake.GetHead().y);

  // Check if the snake's head is on top of food
  if (food->x == headX && food->y == headY) {
    {
      std::lock_guard<std::mutex> lock(mutex);  // Lock mutex for shared score data
      score++;
      scoreUpdated = true;  // Set flag to true for score update
    }
    condition.notify_one();  // Notify score update thread
    PlaceFood();
    snake.GrowBody();

    // Optionally, adjust snake speed as it grows
    if (snake.SizeInBounds(25, 35)) {
      snake.ScaleSpeed(0.95);
    }
  }
}

// Getters to retrieve game information
int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.GetSize(); }
