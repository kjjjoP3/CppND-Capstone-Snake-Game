#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
#include "snake.h"

class Renderer {
 public:
  Renderer(const std::size_t window_width, const std::size_t window_height,
           const std::size_t grid_columns, const std::size_t grid_rows);
  ~Renderer();

  void Render(const Snake& snake, const SDL_Point& food, const SDL_Point& extraFood, bool hasExtraFood);
  void SetWindowTitle(int score, int fps);

 private:
  SDL_Window* window;
  SDL_Renderer* renderer;

  const std::size_t window_width;
  const std::size_t window_height;
  const std::size_t grid_columns;
  const std::size_t grid_rows;
};

#endif
