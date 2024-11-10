#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
#include "snake.h"

class Renderer {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };

  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  void Render(Snake const& snake, SDL_Point const& food);
  void RenderBody(Snake const& snake, SDL_Rect& block);
  void UpdateWindowTitle(int score, int fps);
  Direction GetDirectionFromPoints(int x1, int y1, int x2, int y2);
  Direction GetDirectionFromPoints(SDL_Point p1, SDL_Point p2);
  void RenderSingleBlock(Direction dir, int x, int y, SDL_Rect& block);

 private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
};

#endif
