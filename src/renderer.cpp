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
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
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
  if (snake.IsAlive()) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer, &block);

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}

Renderer::Direction Renderer::Oriented(int x1, int y1, int x2, int y2)
{
    // Simplified logic using a ternary approach to determine direction
    if (y1 != y2) {
        return (y1 < y2) ? Direction::kUp : Direction::kDown;
    }
    return (x1 < x2) ? Direction::kLeft : Direction::kRight;
}

Renderer::Direction Renderer::Oriented(SDL_Point p1, SDL_Point p2)
{
    // Directly using simplified logic to determine orientation
    return Oriented(p1.x, p1.y, p2.x, p2.y);
}

void Renderer::RenderBlock(Direction dir, int x, int y, SDL_Rect& block)
{
    // Consolidated logic for adjusting block position based on direction
    int offset_x = 0, offset_y = 0, adjustment = 0;
    switch (dir) {
    case Direction::kUp:
        offset_y = -1;
        adjustment = -2;
        break;
    case Direction::kDown:
        offset_y = 1;
        adjustment = 2;
        break;
    case Direction::kLeft:
        offset_x = -2;
        adjustment = 1;
        break;
    case Direction::kRight:
        offset_x = 1;
        adjustment = 1;
        break;
    }
    
    block.x = x * block.w + offset_x;
    block.y = y * block.h + offset_y;
    block.w += (dir == Direction::kLeft || dir == Direction::kRight) ? adjustment : 0;
    block.h += (dir == Direction::kUp || dir == Direction::kDown) ? adjustment : 0;
    SDL_RenderFillRect(sdl_renderer, &block);
}

void Renderer::RenderBody(Snake const snake, SDL_Rect &block)
{
    Direction orientation;
    const std::vector<SDL_Point>& body = snake.GetBody();
    int x = static_cast<int>(snake.GetHead().x);
    int y = static_cast<int>(snake.GetHead().y);
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    if (!body.empty()) {
        orientation = Oriented(x, y, body.back().x, body.back().y);
        RenderBlock(orientation, body.back().x, body.back().y, block);
    }

    for (auto point = body.rbegin() + 1; point != body.rend(); ++point) {
        orientation = Oriented(*(point - 1), *point);
        RenderBlock(orientation, point->x, point->y, block);
    }
}
