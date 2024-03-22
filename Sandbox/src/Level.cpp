#include "Level.h"

Level::Level(int width, int height)
    : m_Width(width), m_Height(height), m_Score(0) {
    // Initialize level with provided dimensions and reset the score
}

Level::~Level() {
    // Cleanup resources if necessary
}

void Level::Init()
{
    // Initialize the score, player and GameOver
}

void Level::OnUpdate(Mashenka::TimeStep ts)
{
    // Check if out of bound and resolve the game
}

void Level::OnRender()
{
    // Render commands for the level
}

void Level::OnImGuiRender()
{
    // Render UI
}

void Level::GameOver()
{
    // Resolution when game is over
}

void Level::Reset() {
    // Reset the level to its initial state
    m_Score = 0;
}

bool Level::IsPositionOutsideBounds(int x, int y) const {
    // Check if the given position is outside the level boundaries
    return x < 0 || y < 0 || x >= m_Width || y >= m_Height;
}
