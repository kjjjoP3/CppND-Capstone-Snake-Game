#include "snake.h"
#include <cmath>
#include <iostream>

void Snake::Update() {
  // Lưu vị trí đầu trước khi cập nhật
  SDL_Point prev_head_cell{static_cast<int>(head_x), static_cast<int>(head_y)};
  UpdateHead();
  SDL_Point current_head_cell{static_cast<int>(head_x), static_cast<int>(head_y)};

  // Nếu đầu rắn đã di chuyển sang ô mới, cập nhật các phần tử thân rắn
  if (current_head_cell.x != prev_head_cell.x || current_head_cell.y != prev_head_cell.y) {
    UpdateBody(current_head_cell, prev_head_cell);
  }
}

Point Snake::IncrHead(float x, float y) {
  // Tính toán vị trí mới của đầu rắn dựa trên hướng di chuyển hiện tại
  switch (direction) {
    case Direction::kUp:
      y -= speed;
      break;
    case Direction::kDown:
      y += speed;
      break;
    case Direction::kLeft:
      x -= speed;
      break;
    case Direction::kRight:
      x += speed;
      break;
  }
  return Point(x, y);
}

void Snake::UpdateHead() {
  bool redirected = false;
  Point new_head_position = IncrHead(head_x, head_y);
  int x = static_cast<int>(head_x);
  int y = static_cast<int>(head_y);
  int new_x = static_cast<int>(new_head_position.x);
  int new_y = static_cast<int>(new_head_position.y);

  // Kiểm tra va chạm với tường và thân rắn, thay đổi hướng nếu cần
  switch (direction) {
    case Direction::kUp:
      if (new_head_position.y < 0 || (y != new_y && SnakeCell(new_x, new_y))) {
        redirected = true;
        direction = (x <= 0 || SnakeCell(x - 1, y)) ? Direction::kRight : Direction::kLeft;
      }
      break;
    case Direction::kDown:
      if (new_head_position.y >= grid_height || (y != new_y && SnakeCell(new_x, new_y))) {
        redirected = true;
        direction = (x >= grid_width - 1 || SnakeCell(x + 1, y)) ? Direction::kLeft : Direction::kRight;
      }
      break;
    case Direction::kRight:
      if (new_head_position.x >= grid_width || (x != new_x && SnakeCell(new_x, new_y))) {
        redirected = true;
        direction = (y <= 0 || SnakeCell(x, y - 1)) ? Direction::kDown : Direction::kUp;
      }
      break;
    case Direction::kLeft:
      if (new_head_position.x <= 0 || (x != new_x && SnakeCell(new_x, new_y))) {
        redirected = true;
        direction = (y >= grid_height - 1 || SnakeCell(x, y + 1)) ? Direction::kUp : Direction::kDown;
      }
      break;
  }

  if (redirected) {
    new_head_position = IncrHead(head_x, head_y);
  }

  head_x = new_head_position.x;
  head_y = new_head_position.y;
}

void Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell) {
  // Thêm vị trí trước đó của đầu vào thân rắn
  body.push_back(prev_head_cell);

  // Nếu không đang trong trạng thái phát triển, loại bỏ phần cuối thân rắn
  if (!growing) {
    body.erase(body.begin());
  } else {
    growing = false;
    size++;
  }

  // Kiểm tra xem rắn có tự đâm vào mình không
  for (auto const &segment : body) {
    if (current_head_cell.x == segment.x && current_head_cell.y == segment.y) {
      alive = false;
      break;
    }
  }
}

void Snake::GrowBody() {
  growing = true;
}

// Phương thức kiểm tra xem ô có bị chiếm bởi rắn không
bool Snake::SnakeCell(int x, int y) const {
  if (x == static_cast<int>(head_x) && y == static_cast<int>(head_y)) {
    return true;
  }
  for (auto const &segment : body) {
    if (x == segment.x && y == segment.y) {
      return true;
    }
  }
  return false;
}

Snake::Snake(int grid_width, int grid_height)
    : grid_width(grid_width),
      grid_height(grid_height),
      head_x(grid_width / 2),
      head_y(grid_height / 2) {
  // Khởi tạo thân rắn dựa trên hướng ban đầu
  int x_offset, y_offset;
  switch (direction) {
    case Direction::kUp:
      x_offset = 0;
      y_offset = -1;
      break;
    case Direction::kDown:
      x_offset = 0;
      y_offset = 1;
      break;
    case Direction::kLeft:
      x_offset = -1;
      y_offset = 0;
      break;
    case Direction::kRight:
      x_offset = 1;
      y_offset = 0;
      break;
  }
  for (int i = 1; i < size; ++i) {
    SDL_Point segment{static_cast<int>(head_x - (size - i) * x_offset),
                      static_cast<int>(head_y - (size - i) * y_offset)};
    body.push_back(segment);
  }
}

std::vector<int> Snake::NextCell() const {
  // Xác định ô tiếp theo dựa trên hướng hiện tại của rắn
  std::vector<int> next_position{static_cast<int>(head_x), static_cast<int>(head_y)};
  switch (direction) {
    case Direction::kUp:
      next_position[1] -= 1;
      break;
    case Direction::kDown:
      next_position[1] += 1;
      break;
    case Direction::kLeft:
      next_position[0] -= 1;
      break;
    case Direction::kRight:
      next_position[0] += 1;
      break;
  }
  return next_position;
}

void Snake::ChangeDirection(Direction new_direction) {
  // Cập nhật hướng của rắn nếu ô tiếp theo không có vật cản
  Direction current_direction = direction;
  direction = new_direction;
  std::vector<int> next_position = NextCell();
  if (SnakeCell(next_position[0], next_position[1]) ||
      next_position[0] < 0 || next_position[0] >= grid_width ||
      next_position[1] < 0 || next_position[1] >= grid_height) {
    direction = current_direction;  // Khôi phục hướng nếu va chạm
  }
}
