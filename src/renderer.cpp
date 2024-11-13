#include "renderer.h"
#include <iostream>
#include <string>

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

void Renderer::Render(Snake const snake, SDL_Point const &food) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  // Render food
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
  block.x = food.x * block.w;
  block.y = food.y * block.h;
  SDL_RenderFillRect(sdl_renderer, &block);

  // Render snake's body
  RenderBody(snake, block);

  // Render snake's head
  block.x = static_cast<int>(snake.GetHead().x) * block.w;
  block.y = static_cast<int>(snake.GetHead().y) * block.h;
  SDL_SetRenderDrawColor(sdl_renderer, snake.IsAlive() ? 0x00 : 0xFF,
                         snake.IsAlive() ? 0x7A : 0x00, 0xCC, 0xFF);
  SDL_RenderFillRect(sdl_renderer, &block);

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}

Renderer::Direction Renderer::Oriented(int x1, int y1, int x2, int y2) {
  if (y1 < y2)
    return Direction::kUp;
  else if (y1 > y2)
    return Direction::kDown;
  else if (x1 < x2)
    return Direction::kLeft;
  return Direction::kRight;
}

Renderer::Direction Renderer::Oriented(SDL_Point p1, SDL_Point p2) {
  if (p1.y < p2.y)
    return Direction::kUp;
  else if (p1.y > p2.y)
    return Direction::kDown;
  else if (p1.x < p2.x)
    return Direction::kLeft;
  return Direction::kRight;
}

void Renderer::RenderBlock(Direction dir, int x, int y, SDL_Rect& block) {
  int adjustedX = x * block.w;
  int adjustedY = y * block.h;

  switch (dir) {
    case Direction::kUp:
      adjustedY -= 1;
      block.w -= 2;
      break;
    case Direction::kDown:
      adjustedY += 1;
      block.w -= 2;
      break;
    case Direction::kLeft:
      adjustedX -= 2;
      block.h -= 2;
      block.w += 1;
      break;
    case Direction::kRight:
      adjustedX += 1;
      block.h -= 2;
      break;
  }

  block.x = adjustedX;
  block.y = adjustedY;
  SDL_RenderFillRect(sdl_renderer, &block);
  // Restore the block size after drawing.
  if (dir == Direction::kUp || dir == Direction::kDown) {
    block.w += 2;
  } else if (dir == Direction::kLeft || dir == Direction::kRight) {
    block.h += 2;
    block.w -= 1;
  }
}

void Renderer::RenderBody(Snake const snake, SDL_Rect &block) {
  Direction orientation;
  const std::vector<SDL_Point>& body = snake.GetBody();
  int x = static_cast<int>(snake.GetHead().x);
  int y = static_cast<int>(snake.GetHead().y);
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

  // Render head
  block.x = x * block.w;
  block.y = y * block.h;
  SDL_RenderFillRect(sdl_renderer, &block);

  // Render body
  for (size_t i = 0; i < body.size(); ++i) {
    // Ensure the body part is connected
    x = body[i].x;
    y = body[i].y;
    block.x = x * block.w;
    block.y = y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }
}
