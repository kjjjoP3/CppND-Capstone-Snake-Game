#include "renderer.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <string>

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t block_size,
                   const std::size_t block_count)
    : screen_width(screen_width),
      screen_height(screen_height),
      block_size(block_size),
      block_count(block_count) {
    InitSDL();
}

Renderer::Renderer(Renderer&& other) noexcept
    : sdl_window(std::move(other.sdl_window)),
      sdl_renderer(std::move(other.sdl_renderer)),
      screen_width(other.screen_width),
      screen_height(other.screen_height),
      block_size(other.block_size),
      block_count(other.block_count) {
    other.sdl_window = nullptr;
    other.sdl_renderer = nullptr;
}

Renderer& Renderer::operator=(Renderer&& other) noexcept {
    if (this != &other) {
        sdl_window = std::move(other.sdl_window);
        sdl_renderer = std::move(other.sdl_renderer);
        screen_width = other.screen_width;
        screen_height = other.screen_height;
        block_size = other.block_size;
        block_count = other.block_count;
        other.sdl_window = nullptr;
        other.sdl_renderer = nullptr;
    }
    return *this;
}

Renderer::~Renderer() {
    CleanUp();
}

void Renderer::InitSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        exit(1);
    }

    sdl_window.reset(SDL_CreateWindow("Snake Game", 
                                      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                      screen_width, screen_height, SDL_WINDOW_SHOWN), SDL_DestroyWindow);
    if (!sdl_window) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        exit(1);
    }

    sdl_renderer.reset(SDL_CreateRenderer(sdl_window.get(), -1, SDL_RENDERER_ACCELERATED), SDL_DestroyRenderer);
    if (!sdl_renderer) {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
        exit(1);
    }
}

void Renderer::Render(Snake& snake, const SDL_Point& food) {
    SDL_SetRenderDrawColor(sdl_renderer.get(), 0x1E, 0x1E, 0x1E, 0xFF); // Black background
    SDL_RenderClear(sdl_renderer.get());

    // Render Snake
    SDL_Rect block = { 0, 0, static_cast<int>(block_size), static_cast<int>(block_size) };
    RenderBody(snake, block);

    // Render Food
    SDL_SetRenderDrawColor(sdl_renderer.get(), 0xFF, 0x00, 0x00, 0xFF); // Red color for food
    block.x = food.x * static_cast<int>(block_size);
    block.y = food.y * static_cast<int>(block_size);
    SDL_RenderFillRect(sdl_renderer.get(), &block);

    SDL_RenderPresent(sdl_renderer.get());
}

void Renderer::UpdateWindowTitle(int score, int high_score) {
    std::string title = "Score: " + std::to_string(score) + " High Score: " + std::to_string(high_score);
    SDL_SetWindowTitle(sdl_window.get(), title.c_str());
}

void Renderer::RenderBlock(Renderer::Direction direction, int x, int y, SDL_Rect& block) {
    // Define color based on direction or block type
    switch (direction) {
        case Direction::UP:
            SDL_SetRenderDrawColor(sdl_renderer.get(), 0x00, 0xFF, 0x00, 0xFF); // Green for UP
            break;
        case Direction::DOWN:
            SDL_SetRenderDrawColor(sdl_renderer.get(), 0x00, 0x00, 0xFF, 0xFF); // Blue for DOWN
            break;
        case Direction::LEFT:
            SDL_SetRenderDrawColor(sdl_renderer.get(), 0xFF, 0xFF, 0x00, 0xFF); // Yellow for LEFT
            break;
        case Direction::RIGHT:
            SDL_SetRenderDrawColor(sdl_renderer.get(), 0xFF, 0x00, 0xFF, 0xFF); // Pink for RIGHT
            break;
    }
    block.x = x * static_cast<int>(block_size);
    block.y = y * static_cast<int>(block_size);
    SDL_RenderFillRect(sdl_renderer.get(), &block);
}

void Renderer::RenderBody(Snake& snake, SDL_Rect& block) {
    for (const auto& segment : snake.GetBody()) {
        RenderBlock(snake.GetDirection(), segment.x, segment.y, block);
    }
}

void Renderer::CleanUp() {
    SDL_Quit();
}
