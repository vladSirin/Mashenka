#pragma once
#include <vector>
#include <utility> // For std::pair
#include <Mashenka.h>

// Direction enumeration to represent the snake's movement direction
enum class Direction {
    None,
    Up,
    Down,
    Left,
    Right
};

class Snake {
public:
    Snake(int grid_width, int grid_height);
    virtual ~Snake() = default;

    void LoadAsset();
    
    // TimeStep Update
    void OnUpdate(Mashenka::TimeStep ts);
    void OnRender();
    
    // Updates the snake's position based on its current direction
    void Update();

    // Changes the snake's direction
    void ChangeDirection(Direction newDirection);

    // Grows the snake's length by one segment
    void Grow();

    // Checks if the snake has collided with itself or the boundaries
    bool CheckCollision() const;

    // Resets the snake to its initial state
    void Reset();

    // Getters
    const std::vector<std::pair<int, int>>& GetBody() const { return body; }
    const std::pair<int, int>& GetHead() const {return GetBody().front();}
    Direction GetDirection() const { return direction; }

private:
    std::vector<std::pair<int, int>> body; // Stores the positions of the snake's body segments
    Direction direction; // Current movement direction of the snake
    int gridWidth, gridHeight; // Dimensions of the game grid
    bool growing; // Flag to indicate if the snake is currently growing

    // Helper method to update the position of the snake's head based on the current direction
    std::pair<int, int> NextHeadPosition() const;

    // Head Texture for the snake
    Mashenka::Ref<Mashenka::Texture2D> m_SnakeHeadTexture;
};
