#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"

int main(int argc, char* argv[]) {
  // Cấu hình khung hình và màn hình
  constexpr std::size_t kFramesPerSecond = 60;
  constexpr std::size_t kMillisecondsPerFrame = 1000 / kFramesPerSecond;
  constexpr std::size_t kScreenWidth = 240;
  constexpr std::size_t kScreenHeight = 240;
  constexpr std::size_t kGridColumns = 10;
  constexpr std::size_t kGridRows = 10;

  // Khởi tạo các đối tượng cho renderer, controller và game
  Renderer gameRenderer(kScreenWidth, kScreenHeight, kGridColumns, kGridRows);
  Controller gameController;
  Game snakeGame(kGridColumns, kGridRows);

  // Chạy trò chơi
  snakeGame.Run(gameController, gameRenderer, kMillisecondsPerFrame);

  // Hiển thị kết quả sau khi trò chơi kết thúc
  std::cout << "Game finished successfully!\n";
  std::cout << "Final Score: " << snakeGame.GetScore() << "\n";
  std::cout << "Final Size: " << snakeGame.GetSize() << "\n";

  return 0;
}
