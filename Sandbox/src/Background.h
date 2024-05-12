#pragma once
#include <Mashenka.h>
#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <glm/glm.hpp>

class Background
{
public:
    Background(float scale = 1.0f, float baseTileSize =1.0f, float screenWidth = 1280.0f, float screenHeight = 720.0f);

    void LoadAssets();
    void OnUpdate(Mashenka::TimeStep ts);
    void OnRender(const glm::vec2& playerPosition) const;

    void OnEvent(Mashenka::Event& e);
    bool OnWindowResized(Mashenka::WindowResizeEvent& e);

private:
    Mashenka::Ref<Mashenka::Texture2D> m_BGTileTexture;
    float m_BaseTileSize;
    glm::vec2 m_Scale;
    float m_ScreenWidth;
    float m_ScreenHeight;
};

#endif // BACKGROUND_H
