#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"

int main() {
  // Define constants for screen and game settings
  const std::size_t kFramesPerSecond = 60;
  const std::size_t kMsPerFrame = 1000 / kFramesPerSecond;
  const std::size_t kScreenWidth = 640;
  const std::size_t kScreenHeight = 640;
  const std::size_t kGridWidth = 32;
  const std::size_t kGridHeight = 32;

  // Initialize game components
  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Controller controller;
  Game game(kGridWidth, kGridHeight);

  // Display the high score at the start
  std::cout << "Starting High Score: " << game.GetScore() << "\n";

  // Run the game loop
  game.Run(controller, renderer, kMsPerFrame);

  // Display game results
  std::cout << "Game Over! Terminated successfully.\n";
  std::cout << "Final Score: " << game.GetScore() << "\n";
  std::cout << "Snake Size: " << game.GetSize() << "\n";

  return 0;
}