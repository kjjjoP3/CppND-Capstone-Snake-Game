#include "snake.h"
#include <cmath>
#include <iostream>
#include <memory>
#include <future>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>

class Snake {
public:
    enum class Direction { kUp, kDown, kLeft, kRight };

    Snake(int grid_width, int grid_height);
    ~Snake();  // Rule of 5 - Destructor
    Snake(const Snake& other);  // Rule of 5 - Copy constructor
    Snake& operator=(const Snake& other);  // Rule of 5 - Copy assignment
    Snake(Snake&& other) noexcept;  // Rule of 5 - Move constructor
    Snake& operator=(Snake&& other) noexcept;  // Rule of 5 - Move assignment

    void Update();
    void GrowBody();
    bool SnakeCell(int x, int y) const;
    void ChangeDirection(Direction new_direction);

private:
    struct Point {
        float x;
        float y;
        Point(float x, float y) : x(x), y(y) {}
    };

    std::mutex mtx;  // Mutex for thread safety
    std::condition_variable cv;  // Condition variable

    int grid_width;
    int grid_height;
    float head_x;
    float head_y;
    Direction direction;
    bool growing{false};
    bool alive{true};
    int size{1};
    float speed{0.1f};

    std::vector<SDL_Point> body;

    void UpdateHead();
    void UpdateBody(SDL_Point& current_head_cell, SDL_Point& prev_head_cell);
    Point IncrHead(float x, float y) const;
    std::vector<int> NextCell() const;

    std::promise<void> exit_signal;  // For stopping threads
    std::shared_future<void> exit_future{exit_signal.get_future()};
    std::unique_ptr<std::thread> update_thread;  // Smart pointer for thread

    void RunUpdateLoop();  // Method to run update loop in a thread
};

Snake::Snake(int grid_width, int grid_height)
    : grid_width(grid_width), grid_height(grid_height), head_x(grid_width / 2), head_y(grid_height / 2),
      direction(Direction::kUp) {
    update_thread = std::make_unique<std::thread>(&Snake::RunUpdateLoop, this);
}

Snake::~Snake() {
    exit_signal.set_value();  // Signal thread to stop
    if (update_thread && update_thread->joinable()) {
        update_thread->join();
    }
}

// Rule of 5 - Copy constructor
Snake::Snake(const Snake& other)
    : grid_width(other.grid_width), grid_height(other.grid_height), head_x(other.head_x), head_y(other.head_y),
      direction(other.direction), growing(other.growing), alive(other.alive), size(other.size), speed(other.speed),
      body(other.body) {}

// Rule of 5 - Copy assignment
Snake& Snake::operator=(const Snake& other) {
    if (this == &other) return *this;
    std::lock_guard<std::mutex> lock(mtx);  // Lock for thread safety
    grid_width = other.grid_width;
    grid_height = other.grid_height;
    head_x = other.head_x;
    head_y = other.head_y;
    direction = other.direction;
    growing = other.growing;
    alive = other.alive;
    size = other.size;
    speed = other.speed;
    body = other.body;
    return *this;
}

// Rule of 5 - Move constructor
Snake::Snake(Snake&& other) noexcept
    : grid_width(other.grid_width), grid_height(other.grid_height), head_x(other.head_x), head_y(other.head_y),
      direction(other.direction), growing(other.growing), alive(other.alive), size(other.size), speed(other.speed),
      body(std::move(other.body)), update_thread(std::move(other.update_thread)) {}

// Rule of 5 - Move assignment
Snake& Snake::operator=(Snake&& other) noexcept {
    if (this == &other) return *this;
    std::lock_guard<std::mutex> lock(mtx);
    grid_width = other.grid_width;
    grid_height = other.grid_height;
    head_x = other.head_x;
    head_y = other.head_y;
    direction = other.direction;
    growing = other.growing;
    alive = other.alive;
    size = other.size;
    speed = other.speed;
    body = std::move(other.body);
    update_thread = std::move(other.update_thread);
    return *this;
}

void Snake::RunUpdateLoop() {
    while (exit_future.wait_for(std::chrono::milliseconds(100)) == std::future_status::timeout) {
        std::unique_lock<std::mutex> lock(mtx);  // Locking to avoid race conditions
        Update();
        cv.notify_all();  // Notify any waiting threads after update
    }
}

void Snake::Update() {
    SDL_Point prev_cell{
        static_cast<int>(head_x),
        static_cast<int>(head_y)};
    UpdateHead();
    SDL_Point current_cell{
        static_cast<int>(head_x),
        static_cast<int>(head_y)};
    if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
        UpdateBody(current_cell, prev_cell);
    }
}

// Implement other methods similarly, ensuring thread-safe access where necessary...

void Snake::ChangeDirection(Direction new_direction) {
    std::lock_guard<std::mutex> lock(mtx);  // Lock for thread-safe direction change
    direction = new_direction;
}
