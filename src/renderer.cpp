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
    // Khởi tạo SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Không thể khởi tạo SDL.\n";
        std::cerr << "Lỗi SDL: " << SDL_GetError() << "\n";
    }

    // Tạo cửa sổ
    sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED, screen_width,
                                  screen_height, SDL_WINDOW_SHOWN);

    if (!sdl_window)
    {
        std::cerr << "Không thể tạo cửa sổ.\n";
        std::cerr << "Lỗi SDL: " << SDL_GetError() << "\n";
    }

    // Tạo trình render
    sdl_renderer.reset(SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED));
    if (!sdl_renderer)
    {
        std::cerr << "Không thể tạo renderer.\n";
        std::cerr << "Lỗi SDL: " << SDL_GetError() << "\n";
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
    // Đặt lại các thành viên của other
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

        // Đặt lại các thành viên của other
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
    // Hủy cửa sổ SDL và đặt con trỏ về nullptr
    if (sdl_window)
    {
        SDL_DestroyWindow(sdl_window);
        sdl_window = nullptr;
    }

    // Thoát SDL
    SDL_Quit();
}

void Renderer::Render(Snake const &snake, SDL_Point const &food, SDL_Point const &bonus_food)
{
    SDL_Rect block;
    block.w = screen_width / grid_width;
    block.h = screen_height / grid_height;

    // Xóa màn hình
    SDL_SetRenderDrawColor(sdl_renderer.get(), 0x1E, 0x1E, 0x1E, 0xFF);
    SDL_RenderClear(sdl_renderer.get());

    // Vẽ thức ăn
    SDL_SetRenderDrawColor(sdl_renderer.get(), 0xFF, 0xCC, 0x00, 0xFF);
    block.x = food.x * block.w;
    block.y = food.y * block.h;
    SDL_RenderFillRect(sdl_renderer.get(), &block);

    // Vẽ thức ăn phụ
    if (bonus_food.x >= 0 && bonus_food.y >= 0)
    {
        SDL_SetRenderDrawColor(sdl_renderer.get(), 0x80, 0x00, 0x80, 0xFF);
        block.x = bonus_food.x * block.w;
        block.y = bonus_food.y * block.h;
        SDL_RenderFillRect(sdl_renderer.get(), &block);
    }

    if (!is_game_over)
    {
        // Vẽ thân của rắn
        SDL_SetRenderDrawColor(sdl_renderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
        for (SDL_Point const &point : snake.body)
        {
            block.x = point.x * block.w;
            block.y = point.y * block.h;
            SDL_RenderFillRect(sdl_renderer.get(), &block);
        }

        // Vẽ đầu của rắn
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

    // Cập nhật màn hình
    SDL_RenderPresent(sdl_renderer.get());
}

void Renderer::RenderGameOverMessage()
{
    // Đặt cờ kết thúc game
    is_game_over = true;

    // Hiển thị hộp thoại thông báo kết thúc
    int button_id;
    const SDL_MessageBoxButtonData buttons[] = {
        {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "OK"},
    };
    const SDL_MessageBoxData message_box_data = {
        SDL_MESSAGEBOX_INFORMATION,
        nullptr,
        "Kết thúc trò chơi",
        "Nhấn OK để chơi lại", 
        SDL_arraysize(buttons),
        buttons,
        nullptr
    };

    // Hiển thị hộp thoại và xử lý sự kiện
    if (SDL_ShowMessageBox(&message_box_data, &button_id) == 0 && button_id == 0)
    {
        restart_requested = true;
    }

    // Đặt lại cờ game-over
    is_game_over = false;
}

void Renderer::ResetGameOverMessage()
{
    is_game_over = false;
}

void Renderer::UpdateWindowTitle(int score, int fps)
{
    std::string title = "Điểm: " + std::to_string(score) + " FPS: " + std::to_string(fps);
    SDL_SetWindowTitle(sdl_window, title.c_str());
}
