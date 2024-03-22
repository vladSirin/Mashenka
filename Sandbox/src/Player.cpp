#include "Player.h"

Player::Player() : m_Score(0), s_Lives(3) { // Assuming player starts with 3 lives
}

Player::~Player() {
}

void Player::Reset() {
    m_Score = 0;
    s_Lives = 3; // Reset to initial lives, adjust according to your game design
}

void Player::UpdateScore(int points) {
    m_Score += points;
}

void Player::LoseLife() {
    if (s_Lives > 0) {
        --s_Lives;
    }
    // You could also handle game over state here if lives reach 0, 
    // or check for game over in the GameLayer or Level class where you can manage game states
}

bool Player::IsAlive() const {
    return s_Lives > 0;
}
