#include "renderer.h"
#include <iostream>
#include <string>

Renderer::Renderer(const std::size_t screenWidth,
                   const std::size_t screenHeight,
                   const std::size_t gridWidth, const std::size_t gridHeight)
    : screen_width(screenWidth),
      screen_height(screenHeight),
      grid_width(gridWidth),
      grid_height(gridHeight) {
  // Khởi tạo SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL không thể khởi tạo.\n";
    std::cerr << "Lỗi SDL: " << SDL_GetError() << "\n";
  }

  // Tạo cửa sổ
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Cửa sổ không thể được tạo.\n";
    std::cerr << "Lỗi SDL: " << SDL_GetError() << "\n";
  }

  // Tạo renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Không thể tạo renderer.\n";
    std::cerr << "Lỗi SDL: " << SDL_GetError() << "\n";
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
  if (snake.IsAlive()) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF); // Màu xanh cho đầu rắn sống
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF); // Màu đỏ cho đầu rắn chết
  }
  SDL_RenderFillRect(sdl_renderer, &block);

  // Cập nhật màn hình
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title = "Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps);
  SDL_SetWindowTitle(sdl_window, title.c_str());
}

Renderer::Direction Renderer::Oriented(int x1, int y1, int x2, int y2) {
  // x1, y1 là điểm tiền nhiệm, x2, y2 là điểm đang nhìn
  if (y1 < y2) {
    return Direction::kUp;
  } else if (y1 > y2) {
    return Direction::kDown;
  } else if (x1 < x2) {
    return Direction::kLeft;
  } else {
    return Direction::kRight;
  }
  return Direction::kDown; // không bao giờ đến đây
}

Renderer::Direction Renderer::Oriented(SDL_Point p1, SDL_Point p2) {
  // p1 là điểm tiền nhiệm
  if (p1.y < p2.y) {
    return Direction::kUp;
  } else if (p1.y > p2.y) {
    return Direction::kDown;
  } else if (p1.x < p2.x) {
    return Direction::kLeft;
  } else {
    return Direction::kRight;
  }
  return Direction::kDown; // không bao giờ đến đây
}

void Renderer::RenderBlock(Direction dir, int x, int y, SDL_Rect& block) {
  // Vẽ các khối với hướng tương ứng
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
  Direction orientation;
  const std::vector<SDL_Point>& body = snake.GetBody();
  int x = static_cast<int>(snake.GetHead().x);
  int y = static_cast<int>(snake.GetHead().y);
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF); // Màu trắng cho thân rắn
  orientation = Oriented(x, y, body.back().x, body.back().y);
  RenderBlock(orientation, body.back().x, body.back().y, block);
  
  // Duyệt qua toàn bộ thân rắn và vẽ
  for (auto point = body.rbegin() + 1; point != body.rend(); ++point) {
    orientation = Oriented(*(point - 1), *point);
    RenderBlock(orientation, point->x, point->y, block);
  }
}
