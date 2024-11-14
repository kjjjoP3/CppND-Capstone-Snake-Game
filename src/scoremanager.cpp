#include "scoremanager.h"
#include <fstream>
#include <iostream>

ScoreManager::ScoreManager(const std::string &filename) : filename(filename) {
    loadHighScore();
}

ScoreManager::~ScoreManager() {
    writeHighScore();
}

void ScoreManager::saveHighScore(int score) {
    if (score > highScore) {
        highScore = score;
        writeHighScore();
    }
}

void ScoreManager::loadHighScore() {
    std::ifstream file(filename);
    if (file.is_open()) {
        file >> highScore;
        file.close();
    } else {
        highScore = 0;
    }
}

void ScoreManager::writeHighScore() {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << highScore;
        file.close();
    } else {
        std::cerr << "Unable to open file for writing high score.\n";
    }
}