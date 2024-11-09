#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <ctime> // cho ngày tháng
#include "player.h"

constexpr int kMaxEntries = 500;
const std::string kHighScoreFile = "../highscore.txt";
constexpr int kDisplayTopScores = 20;

class HighScore {
public:
    void Read();
    void Write();
    void Print();
    void Update(const std::string &playerName, int score);

private:
    std::vector<Player> playerList_; // Lưu danh sách các người chơi
};

// Hàm so sánh để sắp xếp người chơi theo điểm số giảm dần
bool comparePlayerByScore(const Player &player1, const Player &player2);
