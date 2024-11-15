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
  // Khởi tạo thư viện SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "Không thể khởi tạo SDL.\n";
    std::cerr << "Lỗi: " << SDL_GetError() << "\n";
    return;
  }

  // Tạo cửa sổ
  sdl_window = SDL_CreateWindow("Trò chơi Rắn", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (!sdl_window) {
    std::cerr << "Không thể tạo cửa sổ.\n";
    std::cerr << "Lỗi: " << SDL_GetError() << "\n";
    return;
  }

  // Tạo bộ dựng
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (!sdl_renderer) {
    std::cerr << "Không thể tạo bộ dựng.\n";
    std::cerr << "Lỗi: " << SDL_GetError() << "\n";
  }
}

Renderer::~Renderer() {
  if (sdl_renderer) {
    SDL_DestroyRenderer(sdl_renderer);
  }
  if (sdl_window) {
    SDL_DestroyWindow(sdl_window);
  }
  SDL_Quit();
}

void Renderer::Render(Snake const &snake, SDL_Point const &food) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Làm sạch màn hình
  SDL_SetRenderDrawColor(sdl_renderer, 30, 30, 30, 255);
  SDL_RenderClear(sdl_renderer);

  // Hiển thị thức ăn
  SDL_SetRenderDrawColor(sdl_renderer, 255, 204, 0, 255);
  block.x = food.x * block.w;
  block.y = food.y * block.h;
  SDL_RenderFillRect(sdl_renderer, &block);

  // Hiển thị thân rắn
  SDL_SetRenderDrawColor(sdl_renderer, 255, 255, 255, 255);
  for (SDL_Point const &segment : snake.body) {
    block.x = segment.x * block.w;
    block.y = segment.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }

  // Hiển thị đầu rắn
  block.x = static_cast<int>(snake.head_x) * block.w;
  block.y = static_cast<int>(snake.head_y) * block.h;
  SDL_SetRenderDrawColor(sdl_renderer, snake.alive ? 0 : 255, snake.alive ? 122 : 0, snake.alive ? 204 : 0, 255);
  SDL_RenderFillRect(sdl_renderer, &block);

  // Cập nhật màn hình
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title = "Điểm: " + std::to_string(score) + " FPS: " + std::to_string(fps);
  SDL_SetWindowTitle(sdl_window, title.c_str());
}
