#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <memory>
#include "snake.h" // Chắc chắn rằng Snake đã được khai báo ở đây hoặc trước đó

class Renderer {
public:
    enum class Direction {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    Renderer(const std::size_t screen_width,
             const std::size_t screen_height,
             const std::size_t block_size,
             const std::size_t block_count);
    
    Renderer(Renderer&& other) noexcept;
    Renderer& operator=(Renderer&& other) noexcept;
    ~Renderer();

    void Render(Snake& snake, const SDL_Point& food);
    void UpdateWindowTitle(int score, int high_score);

private:
    void InitSDL();
    void RenderBlock(Direction direction, int x, int y, SDL_Rect& block);
    void RenderBody(Snake& snake, SDL_Rect& block);
    void CleanUp();

    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> sdl_window;
    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> sdl_renderer;
    std::size_t screen_width;
    std::size_t screen_height;
    std::size_t block_size;
    std::size_t block_count;
};

#endif // RENDERER_H
