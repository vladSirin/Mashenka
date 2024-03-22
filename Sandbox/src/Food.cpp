#include "Food.h"
#include "Random.h" // Assuming you have a Random class for generating random positions

Food::Food(int grid_width, int grid_height)
    : gridWidth(grid_width), gridHeight(grid_height) {
    Respawn();
}

Food::~Food() {
}

void Food::Respawn() {
    GeneratePosition();
}

void Food::GeneratePosition() {
    // Assuming Random::Init() has been called somewhere in your initialization process
    x = Random::Float() * gridWidth; // Generate a random x position within the grid
    y = Random::Float() * gridHeight; // Generate a random y position within the grid
    // Note: Adjust the Random::Float() usage as needed based on your Random class implementation
}


