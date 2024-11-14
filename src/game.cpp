#include "game.h"
#include <iostream>
#include <thread>
#include <future>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)),
      scoreManager("highscore.txt"),
      exit_future(exit_signal.get_future().share()) 
{
  PlaceFood();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  update_thread = std::thread([&]() {
      while (exit_future.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout) {
          Update();
          std::this_thread::sleep_for(std::chrono::milliseconds(target_frame_duration));
      }
  });

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Render - main game loop
    controller.HandleInput(running, snake);
    renderer.Render(snake, food);

    frame_end = SDL_GetTicks();

    // Keep track of frame duration
    frame_count++;
    frame_duration = frame_end - frame_start;

    // Update window title every second
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // Delay to maintain target frame rate
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }

  // Stop the update thread
  exit_signal.set_value();
  if (update_thread.joinable()) {
    update_thread.join();
  }

  // Save high score when game ends
  scoreManager.saveHighScore(score);
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing food.
    if (!snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::Update() {
  if (!snake.alive) return;

  // Lock the snake state
  std::lock_guard<std::mutex> lock(mtx);

  // Save old size and score
  int old_size = snake.size;
  int old_score = score;

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++;
    PlaceFood();
    // Grow snake and increase speed
    snake.GrowBody();
    snake.speed += 0.02;
  }

  // Check if snake state actually changed
  if (snake.size != old_size || score != old_score) {
    mtx.unlock();
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }