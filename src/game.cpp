#include "game.h"
#include <iostream>
#include <algorithm>
#include <thread>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1))
{
  PlaceFood();
}

bool Game::IsGameOver() const
{
  return !snake.alive;
}

void Game::Run(Controller &controller, Renderer &renderer, std::size_t target_frame_duration)
{
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running)
  {

    // Restart the game if requested

    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);

    if (IsGameOver())
    {
      renderer.RenderGameOverMessage();
      if (renderer.IsRestartRequested())
      {
        // Restart the game
        Restart();
        renderer.ResetGameOverMessage();
        // Reset the game-over flag after the restart
      }
      else
      {
        running = false;
        break;
      }
    }
    else
    {
      // Update the game state only if the game is not over and not going to restart
      Update();
      renderer.Render(snake, food, bonus_food);
    }

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000)
    {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to achieve the correct frame rate.
    if (frame_duration < target_frame_duration)
    {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood()
{
  int x, y;
  while (true)
  {
    x = random_w(engine);
    y = random_h(engine);
    if (!snake.SnakeCell(x, y))
    {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::PlaceBonusFood()
{
  int x, y;
  while (true)
  {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item or an obstacle
    // before placing food.
    if (!snake.SnakeCell(x, y) && !(x == food.x && y == food.y))
    {
      bonus_food.x = x;
      bonus_food.y = y;
      return;
    }
  }
}

// Function to restart the game
void Game::Restart()
{
  // Reset the snake to its initial state
  snake.Reset();
  // Reset the score to zero
  score = 0;
  // Reset the bonus food state
  is_bonus_food_active = false;
  // Clear any existing bonus food
  bonus_food.x = -1;
  bonus_food.y = -1;
}

void Game::Update()
{
  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y)
  {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
    already_appeared = false;
  }

  // Check for bonus food
  if (score > 0 && score % 10 == 0 && !already_appeared)
  {
    if (!is_bonus_food_active)
    { // Check if bonus food is already active
      PlaceBonusFood();
      is_bonus_food_active = true;
      bonusFoodThread = std::thread(&Game::BonusFoodTimer, this);
      bonusFoodThread.detach();
      already_appeared = true;
    }
  }

  if (bonus_food.x == new_x && bonus_food.y == new_y)
  {
    score += 3;
    snake.speed -= 0.05;
    bonus_food.x = -1;
    bonus_food.y = -1;
    {
      std::lock_guard<std::mutex> lock(mutex);
      is_bonus_food_active = false;
    }
    cv.notify_one();
  }
}

void Game::BonusFoodTimer()
{
  const int bonusSeconds = 10;
  auto startTime = std::chrono::high_resolution_clock::now();
  std::unique_lock<std::mutex> lock(mutex);
  while (is_bonus_food_active)
  {
    lock.unlock();
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();

    if (elapsedSeconds >= bonusSeconds)
    {
      // Bonus food time is up
      is_bonus_food_active = false;
      bonus_food.x = -1;
      bonus_food.y = -1;
      break;
    }
    lock.lock();
    // Wait for a short interval or until the condition_variable is notified
    cv.wait_for(lock, std::chrono::milliseconds(1000));
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }