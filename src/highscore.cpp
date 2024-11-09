#include "highscore.h"

// Đọc các dòng từ tệp văn bản highscore
void HighScore::Read() {
    std::string line;
    std::string name;
    std::string score;
    std::string date; 
    Player player;

    std::ifstream stream(kHighScoreFile); // Sửa tên biến cho đúng
    if (stream.is_open()) {
        while (std::getline(stream, line)) {
            std::istringstream linestream(line);
            while (linestream >> name >> score >> date) {
                player.setName(name);
                player.setScore(std::stoi(score));
                player.setDate(date); 
                playerList_.emplace_back(player); // Sửa tên biến ở đây
            }
        }
    }
}

// Ghi các điểm cao mới vào tệp
void HighScore::Write() {
    std::ofstream file(kHighScoreFile); // Sửa tên biến cho đúng
    for (const auto &player : playerList_) { // Sửa tên biến ở đây
        file << player.getName() << " " << player.getScore() << " " << player.getDate() << "\n";
    }
}

// Sau khi kết thúc trò chơi, in ra các điểm cao đầu tiên
void HighScore::Print() {
    int rank = 0;
    std::cout << "Highscores:\n\n";
    
    // Lặp qua danh sách người chơi
    for (const auto &player : playerList_) { // Sửa tên biến ở đây
        if (++rank > kDisplayTopScores) break; // Sửa tên biến ở đây
        std::cout << "Rank " << rank << ":\t" << player.getName() << "\t" << player.getScore() << "\t" << player.getDate() << "\n";
    }
    std::cout << "\n";
}

// Cập nhật điểm cao mới
void HighScore::Update(const std::string &playerName, int score) {
    Player player;
    player.setName(playerName);
    player.setScore(score);

    // Lấy thời gian hiện tại
    time_t now = time(0);
    std::tm *ltm = localtime(&now);

    std::string date = std::to_string(ltm->tm_mday) + "." + std::to_string(1 + ltm->tm_mon) + "." + std::to_string(1900 + ltm->tm_year);
    player.setDate(date);

    // Thêm điểm mới và sắp xếp danh sách
    if (score > playerList_.back().getScore()) { // Sửa tên biến ở đây
        playerList_.emplace_back(player); // Sửa tên biến ở đây
        std::sort(playerList_.begin(), playerList_.end(), comparePlayerByScore); // Sửa tên hàm so sánh
    }
}

// Hàm so sánh để sử dụng trong std::sort -> trả về đối tượng có điểm cao hơn
bool comparePlayerByScore(const Player &player1, const Player &player2) {
    return player1.getScore() > player2.getScore();
}
