#include "player.h"
#include <iostream>

// Assigns the player's name
void Player::setName(std::string playerName) {
    _playerName = playerName;
}

// Assigns the player's score
void Player::setScore(int score) {
    _score = score;
}

// Assigns the player's date
void Player::setDate(std::string date) {
    _date = date;
}

// Retrieves the player's name
std::string Player::getName() {
    return _playerName;
}

// Retrieves the player's score
int Player::getScore() {
    return _score;
}

// Retrieves the player's date
std::string Player::getDate() {
    return _date;
}

// Prompts for the player's name input
void Player::enter_name() {
    std::cout << "Please enter the player's name: ";
    std::string name;
    std::cin >> name;
    setName(name);
}
