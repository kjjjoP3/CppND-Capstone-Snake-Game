#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <memory>
#include <string>
#include "snake.h"

class Renderer {
public:
    Renderer(std::size_t screen_width, std::size_t screen_height, std::size_t block_size, std::size_t block_count);
    Renderer(const Renderer& other) = delete;  // Xóa copy constructor
    Renderer& operator=(const Renderer& other) = delete; // Xóa copy assignment
    Renderer(Renderer&& other) noexcept;  // Move constructor
    Renderer& operator=(Renderer&& other) noexcept; // Move assignment
    ~Renderer();

    void Render(Snake snake, const SDL_Point& food);
    void UpdateWindowTitle(int score, int high_score);
    void RenderBlock(Renderer::Direction direction, int x, int y, SDL_Rect& block);
    void RenderBody(Snake snake, SDL_Rect& block);
    
private:
    std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> sdl_window;
    std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> sdl_renderer;
    std::size_t screen_width;
    std::size_t screen_height;
    std::size_t block_size;
    std::size_t block_count;
    
    void InitSDL();
    void CleanUp();
};

#endif
