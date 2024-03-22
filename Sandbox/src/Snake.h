#pragma once
#include <vector>
#include <utility> // For std::pair

class Snake {
public:
    Snake(int grid_width, int grid_height);
    virtual ~Snake();

    enum class Direction {
        kUp,
        kDown,
        kLeft,
        kRight
    };

    void Update();
    bool EatFood(int foodX, int foodY);
    bool CheckCollision() const;

    void Grow();
    void Reset();

    // Getters for snake properties
    std::vector<std::pair<int, int>> GetBody() const { return body; }
    Direction GetDirection() const { return direction; }
    void SetDirection(Direction dir) { direction = dir; }

private:
    std::vector<std::pair<int, int>> body; // Holds the coordinates of the snake's body segments
    Direction direction;
    int gridWidth, gridHeight;
    bool growing = false;
    int growthCounter = 0;

    // Helper methods
    void MoveHead();
    void UpdateBody();
};

