#include "renderer.h"
#include <iostream>

Renderer::Renderer(std::size_t screen_width,
                   std::size_t screen_height,
                   std::size_t grid_width, std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height),
      sdl_window(nullptr),
      sdl_renderer(nullptr, SDL_DestroyRenderer)
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize.\n";
        std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
    }

    // Create Window
    sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED, screen_width,
                                  screen_height, SDL_WINDOW_SHOWN);

    if (!sdl_window)
    {
        std::cerr << "Window could not be created.\n";
        std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
    }

    // Create renderer
    sdl_renderer.reset(SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED));
    if (!sdl_renderer)
    {
        std::cerr << "Renderer could not be created.\n";
        std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
    }
}

Renderer::Renderer(Renderer &&other) noexcept
    : screen_width(other.screen_width),
      screen_height(other.screen_height),
      grid_width(other.grid_width),
      grid_height(other.grid_height),
      is_game_over(other.is_game_over),
      restart_requested(other.restart_requested),
      sdl_window(other.sdl_window),
      sdl_renderer(std::move(other.sdl_renderer))
{
    // Reset other's members
    other.screen_width = 0;
    other.screen_height = 0;
    other.grid_width = 0;
    other.grid_height = 0;
    other.is_game_over = false;
    other.restart_requested = false;
}

Renderer &Renderer::operator=(Renderer &&other) noexcept
{
    if (this != &other)
    {
        screen_width = other.screen_width;
        screen_height = other.screen_height;
        grid_width = other.grid_width;
        grid_height = other.grid_height;
        is_game_over = other.is_game_over;
        restart_requested = other.restart_requested;
        sdl_window = other.sdl_window;
        sdl_renderer = std::move(other.sdl_renderer);

        // Reset other's members
        other.screen_width = 0;
        other.screen_height = 0;
        other.grid_width = 0;
        other.grid_height = 0;
        other.is_game_over = false;
        other.restart_requested = false;
    }

    return *this;
}

Renderer::~Renderer()
{
    // Destroy the SDL_Window and set the raw pointer to nullptr
    if (sdl_window)
    {
        SDL_DestroyWindow(sdl_window);
        sdl_window = nullptr;
    }

    // Quit SDL
    SDL_Quit();
}


void Renderer::Render(Snake const &snake, SDL_Point const &food, SDL_Point const &bonus_food)
{
    SDL_Rect block;
    block.w = screen_width / grid_width;
    block.h = screen_height / grid_height;

    // Clear screen
    SDL_SetRenderDrawColor(sdl_renderer.get(), 0x1E, 0x1E, 0x1E, 0xFF);
    SDL_RenderClear(sdl_renderer.get());

    // Render food
    SDL_SetRenderDrawColor(sdl_renderer.get(), 0xFF, 0xCC, 0x00, 0xFF);
    block.x = food.x * block.w;
    block.y = food.y * block.h;
    SDL_RenderFillRect(sdl_renderer.get(), &block);

    // Render Bonus Food
    if (bonus_food.x != -1 && bonus_food.y != -1)
    {
        SDL_SetRenderDrawColor(sdl_renderer.get(), 0x80, 0x00, 0x80, 0xFF);
        block.x = bonus_food.x * block.w;
        block.y = bonus_food.y * block.h;
        SDL_RenderFillRect(sdl_renderer.get(), &block);
    }

    if (!is_game_over)
    {
        // Render snake's body
        SDL_SetRenderDrawColor(sdl_renderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
        for (SDL_Point const &point : snake.body)
        {
            block.x = point.x * block.w;
            block.y = point.y * block.h;
            SDL_RenderFillRect(sdl_renderer.get(), &block);
        }

        // Render snake's head
        block.x = static_cast<int>(snake.head_x) * block.w;
        block.y = static_cast<int>(snake.head_y) * block.h;
        if (snake.alive)
        {
            SDL_SetRenderDrawColor(sdl_renderer.get(), 0x00, 0x7A, 0xCC, 0xFF);
        }
        else
        {
            SDL_SetRenderDrawColor(sdl_renderer.get(), 0xFF, 0x00, 0x00, 0xFF);
        }
        SDL_RenderFillRect(sdl_renderer.get(), &block);
    }
    // Update Screen
    SDL_RenderPresent(sdl_renderer.get());
}
void Renderer::RenderGameOverMessage()
{
    // Set the game-over flag to true
    is_game_over = true;

    // Show a pop-up message box with the game-over message
    int button_id;
    const SDL_MessageBoxButtonData buttons[] = {
        {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "OK"},
    };
    const SDL_MessageBoxData message_box_data = {
        SDL_MESSAGEBOX_INFORMATION,
        NULL,
        "Game Over",
        "Press OK to Restart the Game", 
        SDL_arraysize(buttons),
        buttons,                        
        NULL                           
    };

    // Show the message box and handle the button click
    if (SDL_ShowMessageBox(&message_box_data, &button_id) == 0 && button_id == 0)
    {
        // Set the restart_requested flag to true
        restart_requested = true; 
    }

    // Reset the game-over flag to false
    is_game_over = false;
}

void Renderer::ResetGameOverMessage()
{
    is_game_over = false;
}

void Renderer::UpdateWindowTitle(int score, int fps)
{
    std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
    SDL_SetWindowTitle(sdl_window, title.c_str());
}