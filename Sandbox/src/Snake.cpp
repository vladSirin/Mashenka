#include "Snake.h"

Snake::Snake(int grid_width, int grid_height)
    : gridWidth(grid_width), gridHeight(grid_height), direction(Direction::kUp) {
    Reset();
}

Snake::~Snake() {
}

void Snake::Update() {
    if (!growing) {
        UpdateBody();
    } else {
        growing = false; // Reset the growing flag after updating
        growthCounter--;
        if (growthCounter <= 0) growing = false;
    }

    MoveHead();
    // Check for self-collision
    if (CheckCollision()) {
        Reset(); // Reset the snake if it collides with itself
    }
}

bool Snake::EatFood(int foodX, int foodY) {
    std::pair<int, int> &head = body.front();
    if (head.first == foodX && head.second == foodY) {
        Grow();
        return true;
    }
    return false;
}

bool Snake::CheckCollision() const {
    // Implement collision detection with the snake's body and boundaries
    return true;
}

void Snake::Grow() {
    growing = true;
    growthCounter += 1; // Increase the length of the snake by one segment
}

void Snake::Reset() {
    // Reset snake to initial state
    body.clear();
    body.push_back({gridWidth / 2, gridHeight / 2}); // Start from center
    direction = Direction::kUp;
    growing = false;
    growthCounter = 0;
}

void Snake::MoveHead() {
    // Update the head's position based on the current direction
}

void Snake::UpdateBody() {
    // Move the snake's body following the head
}
