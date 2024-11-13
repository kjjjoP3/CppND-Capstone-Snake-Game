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
    std::cerr << "SDL initialization failed.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (!sdl_window) {
    std::cerr << "Failed to create window.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (!sdl_renderer) {
    std::cerr << "Failed to create renderer.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

void Renderer::Render(const Snake& snake, const SDL_Point& food) {
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

  // Update screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title = "Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps);
  SDL_SetWindowTitle(sdl_window, title.c_str());
}

Renderer::Direction Renderer::Oriented(int x1, int y1, int x2, int y2) {
  // Determine the direction based on the relative positions
  if (y1 < y2) return Direction::kUp;
  if (y1 > y2) return Direction::kDown;
  return (x1 < x2) ? Direction::kLeft : Direction::kRight;
}

Renderer::Direction Renderer::Oriented(const SDL_Point& p1, const SDL_Point& p2) {
  return Oriented(p1.x, p1.y, p2.x, p2.y);
}

void Renderer::RenderBlock(Direction dir, int x, int y, SDL_Rect& block) {
  // Adjust the block's position and size based on the direction
  switch (dir) {
    case Direction::kUp:
      block.x = x * block.w + 1;
      block.y = y * block.h - 1;
      block.w -= 2;
      SDL_RenderFillRect(sdl_renderer, &block);
      block.w += 2;
      break;

    case Direction::kDown:
      block.x = x * block.w + 1;
      block.y = y * block.h + 1;
      block.w -= 2;
      SDL_RenderFillRect(sdl_renderer, &block);
      block.w += 2;
      break;

    case Direction::kLeft:
      block.x = x * block.w - 2;
      block.y = y * block.h + 1;
      block.h -= 2;
      block.w++;
      SDL_RenderFillRect(sdl_renderer, &block);
      block.h += 2;
      block.w--;
      break;

    case Direction::kRight:
      block.x = x * block.w + 1;
      block.y = y * block.h + 1;
      block.h -= 2;
      SDL_RenderFillRect(sdl_renderer, &block);
      block.h += 2;
      break;
  }
}

void Renderer::RenderBody(const Snake& snake, SDL_Rect& block) {
  const auto& body = snake.GetBody();
  int x = static_cast<int>(snake.GetHead().x);
  int y = static_cast<int>(snake.GetHead().y);
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

  // Render the last segment of the snake's body
  Direction orientation = Oriented(x, y, body.back().x, body.back().y);
  RenderBlock(orientation, body.back().x, body.back().y, block);

  // Render the rest of the body
  for (auto it = body.rbegin() + 1; it != body.rend(); ++it) {
    orientation = Oriented(*(it - 1), *it);
    RenderBlock(orientation, it->x, it->y, block);
  }
}
