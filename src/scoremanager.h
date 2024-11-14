#ifndef SCOREMANAGER_H
#define SCOREMANAGER_H

#include <string>

class ScoreManager {
public:
    ScoreManager(const std::string &filename);
    ~ScoreManager();
    void saveHighScore(int score);

private:
    std::string filename;
    int highScore;

    void loadHighScore();
    void writeHighScore();
};

#endif