#include "Background.h"

using namespace Mashenka;

Background::Background(float scale, float baseTileSize, float screenWidth, float screenHeight)
    : m_BaseTileSize(baseTileSize), m_Scale(scale), m_ScreenWidth(screenWidth), m_ScreenHeight(screenHeight)
{
    //
}


void Background::LoadAssets()
{
    m_BGTileTexture = Texture2D::Create("assets/textures/Checkerboard.png");
}

void Background::OnUpdate(Mashenka::TimeStep ts)
{
}

void Background::OnRender(const glm::vec2& playerPosition) const
{
    //Render the background based on the current player position
    //We need a 4x4 grid of tiles to cover the entire surrounding area
    glm::vec2 topLeft = {playerPosition.x - 2.0f, playerPosition.y + 2.0f}; // top left corner of the grid

    // Draw the tiles around the player
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            glm::vec2 tilePosition;
            tilePosition.x = topLeft.x + 1.0f * i;
            tilePosition.y = topLeft.y - 1.0f * j;
            if (!((abs(tilePosition.x) == 2.0f && j != 0) || (abs(tilePosition.y) == 2.0f && i != 0)))
            {
                Renderer2D::DrawQuad({topLeft.x + 1.0f * i, topLeft.y - 1.0f * j, -0.25}, m_Scale, m_BGTileTexture,
                                     1.0f);
            }
        }
    }
}

void Background::OnEvent(Mashenka::Event& e)
{
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowResizeEvent>(MK_BIND_EVENT_FN(Background::OnWindowResized));
}

// reset the screen size based on the new values
bool Background::OnWindowResized(Mashenka::WindowResizeEvent& e)
{
    m_ScreenHeight = (float)e.GetHeight();
    m_ScreenWidth = (float)e.GetWidth();
    return false;
}
