#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
  GenerateFood();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Vòng lặp chính gồm các bước Input, Update, Render
    controller.HandleInput(running, snake);
    Update();
    renderer.Render(snake, food, extraFood, hasExtraFood);

    frame_end = SDL_GetTicks();

    // Tính toán thời gian thực thi của mỗi vòng lặp
    frame_count++;
    frame_duration = frame_end - frame_start;

    // Cập nhật tiêu đề cửa sổ mỗi giây
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // Nếu thời gian thực thi vòng lặp nhỏ hơn thời gian khung hình mục tiêu, tạm dừng để đạt được tốc độ khung hình mong muốn.
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
    // Đảm bảo rằng vị trí thức ăn không trùng với thân rắn
    if (!snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      break;
    }
  }
  // Khởi tạo giá trị của thức ăn phụ
  hasExtraFood = false;

  // Thêm thức ăn phụ mỗi khi kích thước của rắn chia hết cho 5
  if (snake.size % 5 == 0) {
    GenerateExtraFood();
  }
}

void Game::GenerateExtraFood() {
  int x, y;
  hasExtraFood = true;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Đảm bảo rằng vị trí của thức ăn phụ không trùng với thân rắn
    if (!snake.SnakeCell(x, y)) {
      extraFood.x = x;
      extraFood.y = y;
      break;
    }
  }
}

void Game::Update() {
  if (!snake.alive) return;

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Kiểm tra xem rắn có ăn thức ăn hay không
  if (food.x == new_x && food.y == new_y) {
    score++;
    GenerateFood();
    // Tăng chiều dài và tốc độ của rắn
    snake.GrowBody();
    snake.speed += 0.02;
  }

  // Kiểm tra xem rắn có ăn thức ăn phụ hay không
  if (extraFood.x == new_x && extraFood.y == new_y && hasExtraFood) {
    score += 5;
    snake.GrowBody();
    snake.speed += 0.02;
    hasExtraFood = false;
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }
