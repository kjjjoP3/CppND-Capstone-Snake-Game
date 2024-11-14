#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <string>
#include <memory>
#include "SDL.h"
#include "snake.h"

class Renderer
{
public:
  Renderer(std::size_t screen_width, std::size_t screen_height,
           std::size_t grid_width, std::size_t grid_height);
  Renderer(const Renderer &other) = delete;            // No copy constructor allowed
  Renderer &operator=(const Renderer &other) = delete; // No copy assignment operator allowed
  Renderer(Renderer &&other) noexcept;                 // Move constructor
  Renderer &operator=(Renderer &&other) noexcept;      // Move assignment operator
  ~Renderer();

  void Render(Snake const &snake, SDL_Point const &food, SDL_Point const &bonus_food);
  void UpdateWindowTitle(int score, int fps);
  void RenderGameOverMessage();
  void ResetGameOverMessage();
  bool IsRestartRequested() const { return restart_requested; }

private:
  SDL_Window* sdl_window;         // Raw pointer to SDL_Window
  std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer *)> sdl_renderer;

  std::size_t screen_width;
  std::size_t screen_height;
  std::size_t grid_width;
  std::size_t grid_height;
  bool is_game_over = false;
  bool restart_requested = false;
};

#endif