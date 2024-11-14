#include <iostream>
#include <future>
#include <fstream>
#include "controller.h"
#include "game.h"
#include "renderer.h"

struct GameResult {
    int score;
    int size;
};

// Overloaded operator<< for GameResult
std::ostream& operator<<(std::ostream& os, const GameResult& result) {
    os << "Game has terminated successfully!\n";
    os << "Score: " << result.score << "\n";
    os << "Size: " << result.size << "\n";
    return os;
}

// Template function for writing to file
template <typename T>
void WriteToFile(const std::string& file_name, const T& data) {
    std::ofstream file(file_name, std::ios::app);  // Open in append mode
    if (file.is_open()) {
        file << data;
        file << "--------------------------------\n";  // Separator line
        file.close();
    } else {
        std::cerr << "Failed to open file: " << file_name << std::endl;
    }
}

void RunGame(std::promise<GameResult> &&game_result_promise, Game &game, Controller &controller, Renderer &renderer, std::size_t kMsPerFrame) {
    game.Run(controller, renderer, kMsPerFrame);
    
    GameResult result;
    result.score = game.GetScore();
    result.size = game.GetSize();
    
    game_result_promise.set_value(result);
}

int main() {
    constexpr std::size_t kFramesPerSecond{60};
    constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
    constexpr std::size_t kScreenWidth{640};
    constexpr std::size_t kScreenHeight{640};
    constexpr std::size_t kGridWidth{32};
    constexpr std::size_t kGridHeight{32};

    Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
    Controller controller;
    Game game(kGridWidth, kGridHeight);
    
    std::promise<GameResult> game_result_promise;
    std::future<GameResult> game_result_future = game_result_promise.get_future();

    std::thread gameThread(RunGame, std::move(game_result_promise), std::ref(game), std::ref(controller), std::ref(renderer), kMsPerFrame);

    gameThread.join();  // Wait for the game to finish
    
    GameResult result = game_result_future.get();  // Get the game results
    
    std::cout << result << std::endl;
    
    // Write the results to a file using the template function
    WriteToFile("../game_results.txt", result);
    
    return 0;
}