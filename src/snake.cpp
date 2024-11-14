#include "snake.h"
#include <cmath>
#include <algorithm>

void Snake::Update()
{
    SDL_Point previous_position = {static_cast<int>(head_x), static_cast<int>(head_y)};
    UpdateHead();
    SDL_Point current_position = {static_cast<int>(head_x), static_cast<int>(head_y)};

    if (current_position.x != previous_position.x || current_position.y != previous_position.y)
    {
        UpdateBody(current_position, previous_position);
    }
}

void Snake::UpdateHead()
{
    float delta_x = 0.0f;
    float delta_y = 0.0f;

    switch (direction)
    {
    case Direction::kUp:
        delta_y = -speed;
        break;
    case Direction::kDown:
        delta_y = speed;
        break;
    case Direction::kLeft:
        delta_x = -speed;
        break;
    case Direction::kRight:
        delta_x = speed;
        break;
    }

    head_x = fmod(head_x + delta_x + grid_width, grid_width);
    head_y = fmod(head_y + delta_y + grid_height, grid_height);
}

void Snake::Reset()
{
    body.clear();
    speed = 0.1f;
    size = 1;
    head_x = static_cast<float>(grid_width) / 2;
    head_y = static_cast<float>(grid_height) / 2;
    direction = Direction::kUp;
    alive = true;
}

void Snake::Reset(int initial_size, float initial_speed)
{
    Reset();
    size = initial_size;
    speed = initial_speed;
}

void Snake::Reset(int initial_size, float initial_speed, float initial_head_x, float initial_head_y)
{
    Reset(initial_size, initial_speed);
    head_x = initial_head_x;
    head_y = initial_head_y;
}

void Snake::UpdateBody(SDL_Point &current_position, SDL_Point &previous_position)
{
    body.push_back(previous_position);

    if (!growing)
    {
        body.erase(body.begin());
    }
    else
    {
        growing = false;
        ++size;
    }

    for (const auto &segment : body)
    {
        if (current_position.x == segment.x && current_position.y == segment.y)
        {
            alive = false;
            break;
        }
    }
}

void Snake::GrowBody() 
{ 
    growing = true; 
}

bool Snake::SnakeCell(int x, int y) const
{
    if (x == static_cast<int>(head_x) && y == static_cast<int>(head_y))
    {
        return true;
    }

    return std::any_of(body.begin(), body.end(), [x, y](const SDL_Point &segment) {
        return segment.x == x && segment.y == y;
    });
}
