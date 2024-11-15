#include "game.h"
#include <iostream>
#include <thread>
#include <future>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)),
      scoreManager("highscore.txt"),
      exit_future(exit_signal.get_future().share()) {
    PlaceFood();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
    Uint32 title_timestamp = SDL_GetTicks();
    Uint32 frame_start;
    Uint32 frame_end;
    Uint32 frame_duration;
    int frame_count = 0;
    bool running = true;

    // Thread quản lý việc cập nhật trạng thái của trò chơi
    update_thread = std::thread([&]() {
        while (exit_future.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout) {
            Update();
            std::this_thread::sleep_for(std::chrono::milliseconds(target_frame_duration));
        }
    });

    while (running) {
        frame_start = SDL_GetTicks();

        // Xử lý input và hiển thị khung hình
        controller.HandleInput(running, snake);
        renderer.Render(snake, food);

        frame_end = SDL_GetTicks();

        // Tính thời gian xử lý khung hình
        frame_count++;
        frame_duration = frame_end - frame_start;

        // Cập nhật tiêu đề cửa sổ mỗi giây
        if (frame_end - title_timestamp >= 1000) {
            renderer.UpdateWindowTitle(score, frame_count);
            frame_count = 0;
            title_timestamp = frame_end;
        }

        // Điều chỉnh tốc độ để giữ tần số khung hình ổn định
        if (frame_duration < target_frame_duration) {
            SDL_Delay(target_frame_duration - frame_duration);
        }
    }

    // Dừng luồng cập nhật
    exit_signal.set_value();
    if (update_thread.joinable()) {
        update_thread.join();
    }

    // Lưu điểm cao khi kết thúc trò chơi
    scoreManager.saveHighScore(score);
}

void Game::PlaceFood() {
    int x, y;
    while (true) {
        x = random_w(engine);
        y = random_h(engine);

        // Đảm bảo thức ăn không xuất hiện trùng vị trí với thân rắn
        if (!snake.SnakeCell(x, y)) {
            food.x = x;
            food.y = y;
            return;
        }
    }
}

void Game::Update() {
    if (!snake.alive) return;

    // Khóa trạng thái rắn để đảm bảo tính an toàn luồng
    std::lock_guard<std::mutex> lock(mtx);

    // Lưu lại kích thước và điểm số hiện tại
    int previous_size = snake.size;
    int previous_score = score;

    snake.Update();

    int current_x = static_cast<int>(snake.head_x);
    int current_y = static_cast<int>(snake.head_y);

    // Kiểm tra nếu rắn ăn thức ăn
    if (food.x == current_x && food.y == current_y) {
        score++;
        PlaceFood();
        snake.GrowBody();
        snake.speed += 0.02;
    }

    // Chỉ mở khóa khi có thay đổi trạng thái rắn
    if (snake.size != previous_size || score != previous_score) {
        mtx.unlock();
    }
}

int Game::GetScore() const {
    return score;
}

int Game::GetSize() const {
    return snake.size;
}
