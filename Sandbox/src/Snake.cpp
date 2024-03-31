#include "snake.h"


Snake::Snake(int grid_width, int grid_height) : direction(Direction::Right), gridWidth(grid_width),
                                                gridHeight(grid_height), growing(false)
{
    // Smoke & Flame, particle system
    // TODO:

    // Initialize the body
    Reset();
}

void Snake::LoadAsset()
{
    m_SnakeHeadTexture = Mashenka::Texture2D::Create("assets/textures/SnakeHead.png");
}

void Snake::OnUpdate(Mashenka::TimeStep ts)
{
}

void Snake::OnRender()
{
    Mashenka::Renderer2D::DrawRotatedQuad({body.front().first, body.front().second}, {0.05, 0.05}, glm::radians(-45.0f),
                                          m_SnakeHeadTexture);
}

void Snake::Update()
{
}

void Snake::ChangeDirection(Direction newDirection)
{
}

void Snake::Grow()
{
}

bool Snake::CheckCollision() const
{
    return false;
}

void Snake::Reset()
{
    body.clear();
    int startX = gridWidth / 2;
    int startY = gridHeight / 2;

    for (int i=0; i<3; ++i)
    {
        body.push_back({startX - i, startY});
    }
}

std::pair<int, int> Snake::NextHeadPosition() const
{
    return {1, 1};
}
