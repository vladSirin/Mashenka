#pragma once
#include <Mashenka.h>
#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <glm/glm.hpp>

class BackgroundTile
{
public:
    BackgroundTile(const glm::vec2& position, const Mashenka::Ref<Mashenka::Texture2D>& texture,
                   float scale = 1.0f) : m_Position(position), m_Texture(texture), m_Scale(scale)
    {
    }

    void OnRender() const
    {
        Mashenka::Renderer2D::DrawQuad(m_Position, glm::vec2(m_Scale), m_Texture, 1.0f);
    }

    glm::vec2 GetPosition() const
    {
        return m_Position;
    }

private:
    glm::vec2 m_Position;
    Mashenka::Ref<Mashenka::Texture2D> m_Texture;
    float m_Scale;
};

class BackgroundManager
{
public:
    BackgroundManager(float tileSize, const Mashenka::Ref<Mashenka::Texture2D>& texture);
    void OnUpdate(const glm::vec2& playerPosition);
    void OnRender() const;

private:
    std::vector<BackgroundTile> m_Tiles;
    float m_TileSize;
    Mashenka::Ref<Mashenka::Texture2D> m_Texture;
    glm::vec2 m_LastPlayerPosition;

    void AddTiles(const glm::vec2& tilePosition);
    void UpdateTiles(const glm::vec2& playerPosition);
    
};

#endif // BACKGROUND_H
