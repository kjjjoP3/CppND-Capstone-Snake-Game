#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "SDL.h"

class GameObject {
public:
    GameObject(int grid_width, int grid_height)
        : grid_width(grid_width),
          grid_height(grid_height) {}

    int grid_width;
    int grid_height;

    virtual ~GameObject() {} // Virtual destructor for polymorphic behavior

    // Pure virtual function, making this class abstract
    virtual void Reset() = 0;
};

class Snake : public GameObject
{
public:
  enum class Direction
  {
    kUp,
    kDown,
    kLeft,
    kRight
  };

  Snake(int grid_width, int grid_height)
      : GameObject(grid_width, grid_height),
        head_x(grid_width / 2),
        head_y(grid_height / 2) {}

  void Update();

  void GrowBody();
  bool SnakeCell(int x, int y);

  // Overloaded Reset functions
  void Reset() override;
  void Reset(int initial_size, float initial_speed);
  void Reset(int initial_size, float initial_speed, float initial_head_x, float initial_head_y);

  Direction direction = Direction::kUp;

  float speed{0.1f};
  int size{1};
  bool alive{true};
  float head_x;
  float head_y;
  std::vector<SDL_Point> body;

private:
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

  bool growing{false};
};

#endif