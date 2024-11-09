#pragma once 

#include <string>

class Player {

public:
    void setName(std::string playerName);
    void setScore(int score);
    std::string getDate(); 
    void setDate(std::string date); 

    std::string getName(); 
    int getScore(); 
    
    void enter_name();

private:
    std::string _playerName;
    int _score;
    std::string _date; 
};