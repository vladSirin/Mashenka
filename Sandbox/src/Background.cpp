#include "Background.h"
#include "Mashenka.h"

using namespace Mashenka;

BackgroundManager::BackgroundManager(float tileSize, const Mashenka::Ref<Mashenka::Texture2D>& texture) :
    m_TileSize(tileSize),
    m_Texture(texture),
    m_LastPlayerPosition({0,0})
{
}

void BackgroundManager::OnUpdate(const glm::vec2& playerPosition)
{
    // check if player has moved enough to warrant loading new tiles
    glm::vec2 delta = playerPosition - m_LastPlayerPosition;
    if(glm::length(delta) > m_TileSize / 2.0f)
        
    {
        UpdateTiles(playerPosition);
        m_LastPlayerPosition = playerPosition;
    }
}

void BackgroundManager::OnRender() const
{
    for (const auto& tile : m_Tiles)
    {
        tile.OnRender();
    }
}

void BackgroundManager::AddTiles(const glm::vec2& tilePosition)
{
    m_Tiles.emplace_back(tilePosition, m_Texture, 1.0f);
}

void BackgroundManager::UpdateTiles(const glm::vec2& playerPosition)
{
    m_Tiles.clear();

    int left = static_cast<int>(playerPosition.x / m_TileSize) - 3;
    int right = static_cast<int>(playerPosition.x / m_TileSize) + 3;
    int top = static_cast<int>(playerPosition.y / m_TileSize) - 3;
    int bottom = static_cast<int>(playerPosition.y / m_TileSize) + 3;

    for (int x=left; x<=right; ++x)
    {
        for (int y=top; y<=bottom; ++y)
        {
            glm::vec2 tilePos = glm::vec2(x * m_TileSize, y * m_TileSize);
            AddTiles(tilePos);
            MK_CORE_INFO("there are {0} tiles", m_Tiles.size());
        }
    }
}
