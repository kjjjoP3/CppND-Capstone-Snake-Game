#include "game.h"
#include <iostream>
#include <algorithm>
#include <thread>
#include "SDL.h"
#include <random>
#include <chrono>

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
    frame_start = SDL_GetTicks();

    // Xử lý đầu vào, cập nhật và vẽ game
    controller.HandleInput(running, snake);

    if (IsGameOver())
    {
      renderer.RenderGameOverMessage();
      if (renderer.IsRestartRequested())
      {
        Restart();
        renderer.ResetGameOverMessage();
      }
      else
      {
        running = false;
        break;
      }
    }
    else
    {
      Update();
      renderer.Render(snake, food, bonus_food);
    }

    frame_end = SDL_GetTicks();
    frame_count++;
    frame_duration = frame_end - frame_start;

    if (frame_end - title_timestamp >= 1000)
    {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    if (frame_duration < target_frame_duration)
    {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood()
{
  int x, y;
  do
  {
    x = random_w(engine);
    y = random_h(engine);
  } while (snake.SnakeCell(x, y));

  food.x = x;
  food.y = y;
}

void Game::PlaceBonusFood()
{
  int x, y;
  do
  {
    x = random_w(engine);
    y = random_h(engine);
  } while (snake.SnakeCell(x, y) || (x == food.x && y == food.y));

  bonus_food.x = x;
  bonus_food.y = y;
}

void Game::Restart()
{
  snake.Reset();
  score = 0;
  is_bonus_food_active = false;
  bonus_food = {-1, -1};
}

void Game::Update()
{
  snake.Update();
  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  if (food.x == new_x && food.y == new_y)
  {
    score++;
    PlaceFood();
    snake.GrowBody();
    snake.speed += 0.02;
    already_appeared = false;
  }

  if (score > 0 && score % 10 == 0 && !already_appeared)
  {
    if (!is_bonus_food_active)
    {
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
    snake.speed = std::max(0.1f, snake.speed - 0.05f);
    bonus_food = {-1, -1};
    {
      std::lock_guard<std::mutex> lock(mutex);
      is_bonus_food_active = false;
    }
    cv.notify_one();
  }
}

void Game::BonusFoodTimer()
{
  const int bonusDuration = 10;
  auto startTime = std::chrono::high_resolution_clock::now();
  std::unique_lock<std::mutex> lock(mutex);

  while (is_bonus_food_active)
  {
    lock.unlock();
    auto currentTime = std::chrono::high_resolution_clock::now();
    if (std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count() >= bonusDuration)
    {
      is_bonus_food_active = false;
      bonus_food = {-1, -1};
      break;
    }
    lock.lock();
    cv.wait_for(lock, std::chrono::milliseconds(1000));
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }
