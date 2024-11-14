// In controller.h
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "snake.h"
#include <iostream>
class Controller
{
public:
  void HandleInput(bool &running, Snake &snake);

private:
  void ChangeDirection(Snake &snake, Snake::Direction input,
                       Snake::Direction opposite) const;
};

#endif