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
  // Khởi tạo SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "Không thể khởi tạo SDL.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Tạo cửa sổ
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (!sdl_window) {
    std::cerr << "Không thể tạo cửa sổ.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Tạo renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (!sdl_renderer) {
    std::cerr << "Không thể tạo renderer.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

void Renderer::Render(const Snake &snake, const SDL_Point &food) {
  SDL_Rect block{.w = screen_width / grid_width, .h = screen_height / grid_height};

  // Xóa màn hình
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  // Vẽ thức ăn
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
  block.x = food.x * block.w;
  block.y = food.y * block.h;
  SDL_RenderFillRect(sdl_renderer, &block);

  // Vẽ thân rắn
  RenderBody(snake, block);

  // Vẽ đầu rắn
  block.x = static_cast<int>(snake.GetHead().x) * block.w;
  block.y = static_cast<int>(snake.GetHead().y) * block.h;
  SDL_SetRenderDrawColor(sdl_renderer, snake.IsAlive() ? 0x00 : 0xFF, snake.IsAlive() ? 0x7A : 0x00, snake.IsAlive() ? 0xCC : 0x00, 0xFF);
  SDL_RenderFillRect(sdl_renderer, &block);

  // Cập nhật màn hình
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}

Renderer::Direction Renderer::Oriented(int x1, int y1, int x2, int y2) {
    if (y1 != y2) return (y1 < y2) ? Direction::kUp : Direction::kDown;
    return (x1 < x2) ? Direction::kLeft : Direction::kRight;
}

Renderer::Direction Renderer::Oriented(const SDL_Point &p1, const SDL_Point &p2) {
    return Oriented(p1.x, p1.y, p2.x, p2.y);
}

void Renderer::RenderBlock(Direction dir, int x, int y, SDL_Rect &block) {
  auto configureBlock = [&](int offsetX, int offsetY, int widthAdjustment, int heightAdjustment) {
    block.x = x * block.w + offsetX;
    block.y = y * block.h + offsetY;
    block.w += widthAdjustment;
    block.h += heightAdjustment;
    SDL_RenderFillRect(sdl_renderer, &block);
  };

  switch (dir) {
    case Direction::kUp:
      configureBlock(1, -1, -2, 0);
      break;
    case Direction::kDown:
      configureBlock(1, 1, -2, 0);
      break;
    case Direction::kLeft:
      configureBlock(-2, 1, 1, -2);
      break;
    case Direction::kRight:
      configureBlock(1, 1, 0, -2);
      break;
  }
}

void Renderer::RenderBody(Snake const snake, SDL_Rect &block) {
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  const auto &body = snake.GetBody();
  auto orientation = Oriented(snake.GetHead().x, snake.GetHead().y, body.back().x, body.back().y);
  
  RenderBlock(orientation, body.back().x, body.back().y, block);
  for (auto it = body.rbegin() + 1; it != body.rend(); ++it) {
    orientation = Oriented(*(it - 1), *it);
    RenderBlock(orientation, it->x, it->y, block);
  }
}
