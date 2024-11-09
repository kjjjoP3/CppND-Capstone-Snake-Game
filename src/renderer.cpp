#include "renderer.h"
#include <iostream>
#include <string>

Renderer::Renderer(const std::size_t window_width, const std::size_t window_height,
                   const std::size_t grid_columns, const std::size_t grid_rows)
    : window_width(window_width), window_height(window_height),
      grid_columns(grid_columns), grid_rows(grid_rows) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "Failed to initialize SDL.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, window_width,
                            window_height, SDL_WINDOW_SHOWN);

  if (window == nullptr) {
    std::cerr << "Unable to create window.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == nullptr) {
    std::cerr << "Failed to create renderer.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

Renderer::~Renderer() {
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void Renderer::Render(const Snake& snake, const SDL_Point& food, const SDL_Point& extraFood, bool hasExtraFood) {
  SDL_Rect block;
  block.w = window_width / grid_columns;
  block.h = window_height / grid_rows;

  // Clear the screen
  SDL_SetRenderDrawColor(renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(renderer);

  // Render food
  SDL_SetRenderDrawColor(renderer, 0xFF, 0xCC, 0x00, 0xFF);
  block.x = food.x * block.w;
  block.y = food.y * block.h;
  SDL_RenderFillRect(renderer, &block);

  // Render extra food if available
  if (hasExtraFood) {
    SDL_SetRenderDrawColor(renderer, 0xAA, 0xBB, 0xCC, 0xEE);
    block.x = extraFood.x * block.w;
    block.y = extraFood.y * block.h;
    SDL_RenderFillRect(renderer, &block);
  }

  // Render the snake's body
  SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (const SDL_Point& point : snake.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(renderer, &block);
  }

  // Render the snake's head
  block.x = static_cast<int>(snake.head_x) * block.w;
  block.y = static_cast<int>(snake.head_y) * block.h;
  if (snake.alive) {
    SDL_SetRenderDrawColor(renderer, 0x00, 0x7A, 0xCC, 0xFF);  // Alive snake color
  } else {
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);  // Dead snake color
  }
  SDL_RenderFillRect(renderer, &block);

  // Update the screen
  SDL_RenderPresent(renderer);
}

void Renderer::SetWindowTitle(int score, int fps) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(window, title.c_str());
}
