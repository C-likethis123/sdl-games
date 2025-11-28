//
//  GameScene.h
//  hello sdl2
//
//  Created by Chow Jia Ying on 24/11/25.
//


#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "../Button.h"
#include "Scene.h"
#include "../Tetra.h"
#include <vector>
#include <memory>
#include "../Texture.h"
#include <SDL3/SDL_main.h>

class GameScene : public Scene {
public:
    GameScene();
    
    void initialise() override;
    void render() override;
    void handleEvent(SDL_Event& e) override;
    
private:
    // Tetris grid configuration
    static constexpr int TETRIS_GRID_WIDTH = 15;
    static constexpr int TETRIS_GRID_HEIGHT = 20;
    static constexpr float TETRIS_CELL_SIZE = 20.0f;
    
    const float TETRIS_GRID_START_X = 300.0f;
    const float TETRIS_GRID_START_Y = 50.0f;
    
    // Tetris state
    std::unique_ptr<Tetra> currentPiece;
    std::unique_ptr<Tetra> nextPiece;
    std::vector<std::vector<int>> tetrisGrid;  // 0 = empty, 1 = occupied
    uint64_t lastMoveTime;
    static constexpr uint64_t MOVE_DELAY_MS = 500;  // Auto-move down every 500ms
    
    // Helper methods
    void spawnNewPiece();
    TetraType getRandomTetraType();
    bool canMove(int deltaX, int deltaY);
    bool canRotate(bool clockwise);
    void lockPiece();
    void renderTetrisGrid();
    void renderNextPiecePreview();
    void updateGravity();
    
    LTexture background;
};

#endif // GAMESCENE_H
