#ifndef GAME_H
#define GAME_H

#include <random>
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"

class Game
{
public:
  Game(std::size_t grid_width, std::size_t grid_height);

  void Run(Controller &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;

private:
  Snake snake;
  SDL_Point food;
  SDL_Point bonus_food{-1, -1}; 

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  std::mutex mutex;
  std::condition_variable cv;
  std::thread bonusFoodThread;
  bool is_bonus_food_active{false};
  bool already_appeared{false};
  int score{0};

  void PlaceFood();
  void PlaceBonusFood();
  void Update();
  void BonusFoodTimer();
  void Restart();
  bool IsGameOver() const;
};

#endif