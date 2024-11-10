#include "controller.h"
#include <iostream>
#include "SDL.h"
#include "snake.h"

void Controller::ChangeDirection(Snake &snake, Snake::Direction newDirection,
                                 Snake::Direction oppositeDirection) const {
  // Nếu hướng hiện tại khác hướng ngược lại, thay đổi hướng của rắn
  // Hoặc nếu kích thước rắn là 1, không bị giới hạn trong hướng ngược lại
  if (snake.GetDirection() != oppositeDirection || snake.GetSize() == 1) {
    snake.SetDirection(newDirection);
  }
}

void Controller::HandleInput(bool &isRunning, Snake &snake) const {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      isRunning = false;
    } else if (event.type == SDL_KEYDOWN) {
      switch (event.key.keysym.sym) {
        case SDLK_UP:
          ChangeDirection(snake, Snake::Direction::kUp, Snake::Direction::kDown);
          break;

        case SDLK_DOWN:
          ChangeDirection(snake, Snake::Direction::kDown, Snake::Direction::kUp);
          break;

        case SDLK_LEFT:
          ChangeDirection(snake, Snake::Direction::kLeft, Snake::Direction::kRight);
          break;

        case SDLK_RIGHT:
          ChangeDirection(snake, Snake::Direction::kRight, Snake::Direction::kLeft);
          break;
      }
    }
  }
}
