#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "snake.h"

class Controller {
 public:
  // Xử lý các sự kiện nhập liệu từ người dùng
  void HandleInput(bool &isRunning, Snake &snake) const;

 private:
  // Thay đổi hướng của rắn với các điều kiện đối nghịch
  void ChangeDirection(Snake &snake, Snake::Direction newDirection,
                       Snake::Direction oppositeDirection) const;
};

#endif