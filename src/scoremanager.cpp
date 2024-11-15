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
    std::ifstream inputFile(filename, std::ios::in);
    if (inputFile) {
        inputFile >> highScore;
    } else {
        highScore = 0;  // Đặt giá trị mặc định nếu không thể đọc tệp
    }
}

void ScoreManager::writeHighScore() {
    std::ofstream outputFile(filename, std::ios::out);
    if (outputFile) {
        outputFile << highScore;
    } else {
        std::cerr << "Không thể mở tệp để ghi điểm cao.\n";
    }
}
