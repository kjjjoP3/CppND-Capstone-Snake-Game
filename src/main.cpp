#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include "highscore.h"
#include "player.h"

int main() {
  // Configure constants for frame rate and screen dimensions
  constexpr std::size_t kFramesPerSecond = 60;
  constexpr std::size_t kMsPerFrame = 1000 / kFramesPerSecond;
  constexpr std::size_t kScreenWidth = 640;
  constexpr std::size_t kScreenHeight = 640;
  constexpr std::size_t kGridWidth = 32;
  constexpr std::size_t kGridHeight = 32;

  // Initialize Player and HighScore objects
  HighScore highScoreSystem;
  Player currentPlayer;
  currentPlayer.enter_name();
  std::cout << "Game has started!\n\n";

  // Game loop
  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Controller inputController;
  Game gameInstance(kGridWidth, kGridHeight);
  gameInstance.Run(inputController, renderer, kMsPerFrame);

  // Update high scores before the game closes
  highScoreSystem.Read();
  highScoreSystem.Update(currentPlayer.getName(), gameInstance.GetScore());
  highScoreSystem.Write();

  // Display final score and recent high scores
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << gameInstance.GetScore() << "\n";
  std::cout << "Size: " << gameInstance.GetSize() << "\n\n";
  highScoreSystem.Print();

  return 0;
}
