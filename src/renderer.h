#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
#include "snake.h"

class Renderer {
 public:
  // Hướng di chuyển của các khối trong trò chơi
  enum class Direction { kUp, kDown, kLeft, kRight };

  // Constructor & Destructor
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  // Hàm để render trò chơi
  void Render(const Snake& snake, const SDL_Point& food);

  // Render toàn bộ thân của rắn
  void RenderBody(const Snake& snake, SDL_Rect& block);

  // Cập nhật tiêu đề cửa sổ với điểm số và FPS
  void UpdateWindowTitle(int score, int fps);

  // Xác định hướng dựa trên tọa độ hai điểm đầu vào
  Direction GetDirectionFromPoints(int x1, int y1, int x2, int y2);
  Direction GetDirectionFromPoints(SDL_Point p1, SDL_Point p2);

  // Render một khối đơn lẻ với hướng và tọa độ cho trước
  void RenderSingleBlock(Direction dir, int x, int y, SDL_Rect& block);

 private:
  SDL_Window* sdl_window;    // Con trỏ tới cửa sổ SDL
  SDL_Renderer* sdl_renderer; // Con trỏ tới renderer của SDL

  const std::size_t screen_width;  // Chiều rộng màn hình
  const std::size_t screen_height; // Chiều cao màn hình
  const std::size_t grid_width;    // Chiều rộng của lưới
  const std::size_t grid_height;   // Chiều cao của lưới
};

#endif
