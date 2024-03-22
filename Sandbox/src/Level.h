#pragma once
#include "Mashenka.h"
#include "Player.h"

class Level {
    
public:
    Level(int width, int height);
    virtual ~Level();

    void Init(); //initialize the level
    void OnUpdate(Mashenka::TimeStep ts);
    void OnRender();
    void OnImGuiRender();
    bool IsGameOver() const {return m_GameOver;}
    
    void Reset();
    

    Player& GetPlayer() {return m_Player;}

private:
    bool IsPositionOutsideBounds(int x, int y) const;
    void GameOver();


private:
    Player m_Player;
    
    int m_Width, m_Height; // Dimensions of the level
    bool m_GameOver; // if game is over
    int m_Score;
};

