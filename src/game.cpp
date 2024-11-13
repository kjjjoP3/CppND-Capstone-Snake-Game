#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),          // Khắc phục lỗi thức ăn ngoài màn hình
      random_h(0, static_cast<int>(grid_height - 1)) {
  GenerateFood(); // Đặt thức ăn tại vị trí mới
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool is_running = true;

  while (is_running) {
    frame_start = SDL_GetTicks();

    // Vòng lặp chính của trò chơi: Xử lý đầu vào, cập nhật trạng thái, và hiển thị.
    controller.HandleInput(is_running, snake);
    Update();
    renderer.Render(snake, food);

    frame_end = SDL_GetTicks();
    frame_count++;
    frame_duration = frame_end - frame_start;

    // Cập nhật tiêu đề cửa sổ mỗi giây một lần.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // Điều chỉnh thời gian nghỉ giữa các khung hình để duy trì tốc độ khung hình cố định.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::GenerateFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Đảm bảo vị trí thức ăn không nằm trên cơ thể rắn
    if (!snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::Update() {
  if (!snake.IsAlive()) return;

  snake.Update();

  int head_x = static_cast<int>(snake.GetHead().x);
  int head_y = static_cast<int>(snake.GetHead().y);

  // Kiểm tra xem đầu rắn có ở vị trí thức ăn không
  if (food.x == head_x && food.y == head_y) {
    score++;
    GenerateFood(); // Đặt thức ăn tại vị trí mới
    snake.GrowBody();

    // Tăng tốc độ khi rắn đạt chiều dài trong khoảng nhất định
    if (snake.SizeInBounds(25, 35)) {
        snake.ScaleSpeed(0.95);
    }
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.GetSize(); }
