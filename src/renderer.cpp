#include "renderer.h"
#include <iostream>
#include <string>
#include <future>
#include <thread>
#include <mutex>
#include <condition_variable>

// Constructor initializing Renderer and RAII for SDL resources
Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    throw std::runtime_error("SDL could not initialize: " + std::string(SDL_GetError()));
  }

  // Create window using a unique_ptr with a custom deleter
  sdl_window = std::unique_ptr<SDL_Window, SDLDeleter>(SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                                                       SDL_WINDOWPOS_CENTERED, screen_width,
                                                                       screen_height, SDL_WINDOW_SHOWN));
  if (!sdl_window) {
    throw std::runtime_error("Window could not be created: " + std::string(SDL_GetError()));
  }

  // Create renderer using a unique_ptr with a custom deleter
  sdl_renderer = std::unique_ptr<SDL_Renderer, SDLDeleter>(SDL_CreateRenderer(sdl_window.get(), -1, SDL_RENDERER_ACCELERATED));
  if (!sdl_renderer) {
    throw std::runtime_error("Renderer could not be created: " + std::string(SDL_GetError()));
  }

  // Start a rendering thread to handle frame rendering
  render_future = render_promise.get_future();
  std::thread(&Renderer::RenderLoop, this).detach();
}

// Rule of 5: Copy constructor, Copy assignment, Move constructor, Move assignment, and Destructor
Renderer::Renderer(const Renderer &other) = delete;             // Delete copy constructor
Renderer &Renderer::operator=(const Renderer &other) = delete;  // Delete copy assignment

Renderer::Renderer(Renderer &&other) noexcept = default;             // Default move constructor
Renderer &Renderer::operator=(Renderer &&other) noexcept = default;  // Default move assignment

Renderer::~Renderer() {
  SDL_Quit();  // Ensure SDL resources are freed on destruction
}

// RAII and custom deleter for SDL pointers
struct SDLDeleter {
  void operator()(SDL_Window *window) const { if (window) SDL_DestroyWindow(window); }
  void operator()(SDL_Renderer *renderer) const { if (renderer) SDL_DestroyRenderer(renderer); }
};

// Render function called by main game loop
void Renderer::Render(const Snake &snake, const SDL_Point &food) {
  std::lock_guard<std::mutex> lock(render_mutex);
  // Store the latest render data
  this->snake = &snake;
  this->food = food;
  render_promise.set_value(true);  // Notify RenderLoop for an update
  render_promise = std::promise<bool>();  // Reset promise for next frame
  render_future = render_promise.get_future();
}

// Background thread rendering loop with synchronization
void Renderer::RenderLoop() {
  while (render_future.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout) {
    std::unique_lock<std::mutex> lock(render_mutex);
    condition.wait(lock, [this] { return render_future.valid(); });

    // Rendering logic
    SDL_Rect block;
    block.w = screen_width / grid_width;
    block.h = screen_height / grid_height;

    SDL_SetRenderDrawColor(sdl_renderer.get(), 0x1E, 0x1E, 0x1E, 0xFF);  // Clear screen
    SDL_RenderClear(sdl_renderer.get());

    // Render food
    SDL_SetRenderDrawColor(sdl_renderer.get(), 0xFF, 0xCC, 0x00, 0xFF);
    block.x = food.x * block.w;
    block.y = food.y * block.h;
    SDL_RenderFillRect(sdl_renderer.get(), &block);

    // Render snake body and head
    RenderBody(*snake, block);

    block.x = static_cast<int>(snake->GetHead().x) * block.w;
    block.y = static_cast<int>(snake->GetHead().y) * block.h;
    SDL_SetRenderDrawColor(sdl_renderer.get(), snake->IsAlive() ? 0x00 : 0xFF, snake->IsAlive() ? 0x7A : 0x00, snake->IsAlive() ? 0xCC : 0x00, 0xFF);
    SDL_RenderFillRect(sdl_renderer.get(), &block);

    // Update screen
    SDL_RenderPresent(sdl_renderer.get());
  }
}

// Render single snake body block with direction
void Renderer::RenderSingleBlock(Direction dir, int x, int y, SDL_Rect &block) {
  block.x = x * block.w + ((dir == Direction::kRight) ? 1 : ((dir == Direction::kLeft) ? -1 : 0));
  block.y = y * block.h + ((dir == Direction::kDown) ? 1 : ((dir == Direction::kUp) ? -1 : 0));
  block.w -= 2;
  SDL_RenderFillRect(sdl_renderer.get(), &block);
  block.w += 2;
}

// Update window title
void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window.get(), title.c_str());
}
