#include "Level.h"

using namespace Mashenka;

void Level::Init()
{
    m_TriangleTexture = Texture2D::Create("assets/textures/Triangle.png");
    m_Player.LoadAssets();
    
}

void Level::OnUpdate(Mashenka::TimeStep ts)
{
}

void Level::OnRender()
{
}

void Level::OnImGuiRender()
{
}

void Level::Reset()
{
}

bool Level::CollisionTest()
{
}

void Level::GenerateObstacles()
{
}

void Level::CreateTriangleObstacle()
{
}
