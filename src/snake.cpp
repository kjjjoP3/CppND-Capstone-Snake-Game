#include "snake.h"
#include <cmath>
#include <iostream>

void Snake::Update() {
    SDL_Point previous_cell{static_cast<int>(head_x), static_cast<int>(head_y)};
    UpdateHead();
    SDL_Point current_cell{static_cast<int>(head_x), static_cast<int>(head_y)};

    // Kiểm tra nếu vị trí đầu rắn đã thay đổi
    if (current_cell != previous_cell) {
        UpdateBody(current_cell, previous_cell);
    }
}

void Snake::UpdateHead() {
    switch (direction) {
        case Direction::kUp:
            head_y -= speed;
            break;
        case Direction::kDown:
            head_y += speed;
            break;
        case Direction::kLeft:
            head_x -= speed;
            break;
        case Direction::kRight:
            head_x += speed;
            break;
    }

    // Bảo đảm đầu rắn luôn nằm trong lưới bằng cách sử dụng phép chia dư
    head_x = fmod(head_x + grid_width, grid_width);
    head_y = fmod(head_y + grid_height, grid_height);
}

void Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell) {
    // Thêm ô trước đó của đầu rắn vào thân rắn
    body.push_back(prev_head_cell);

    // Nếu rắn không đang phát triển, xóa phần đuôi
    if (!growing) {
        body.erase(body.begin());
    } else {
        growing = false; // Kết thúc quá trình phát triển
        size++;
    }

    // Kiểm tra xem đầu rắn có chạm vào thân rắn không
    for (const auto &segment : body) {
        if (current_head_cell.x == segment.x && current_head_cell.y == segment.y) {
            alive = false;
            break;
        }
    }
}

void Snake::GrowBody() {
    growing = true;
}

bool Snake::SnakeCell(int x, int y) {
    // Kiểm tra nếu tọa độ (x, y) là đầu rắn
    if (x == static_cast<int>(head_x) && y == static_cast<int>(head_y)) {
        return true;
    }
    // Kiểm tra nếu tọa độ (x, y) là một phần của thân rắn
    for (const auto &segment : body) {
        if (x == segment.x && y == segment.y) {
            return true;
        }
    }
    return false;
}

void Snake::increaseSpeed() {
    speed += 0.05f; // Tăng tốc độ di chuyển của rắn
}
