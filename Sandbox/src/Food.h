#pragma once

class Food {
public:
    Food(int grid_width, int grid_height);
    virtual ~Food();

    void Respawn();
    int GetX() const { return x; }
    int GetY() const { return y; }

private:
    int x, y; // Position of the food on the grid
    int gridWidth, gridHeight;

    // Generates a new position for the food
    void GeneratePosition();
};
