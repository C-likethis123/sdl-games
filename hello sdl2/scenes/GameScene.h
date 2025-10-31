//
//  GameScene.h
//  hello sdl2
//
//  Created by Chow Jia Ying on 31/10/25.
//

#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "Scene.h"

class GameScene : public Scene {
public:
    GameScene();
    void initialise() override;
    void render() override;
private:
    // Equal-sized grid: 300x300 total, each cell is 100x100
    // Centered on screen (640x480)
    const float GRID_SIZE = 300.0f;
    const float CELL_SIZE = 100.0f;
    const float GRID_START_X = (SCREEN_WIDTH - GRID_SIZE) / 2.0f;  // Center horizontally
    const float GRID_START_Y = 120.0f;  // Leave space for text at top
    const float BUTTON_INSET = 2.0f;  // Padding to keep grid lines visible
    bool checkWin(const std::string& player);
};

#endif // GAMESCENE_H
