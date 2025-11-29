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
    static constexpr uint64_t MOVE_DELAY_MS = 1000;  // Auto-move down every 1s
    
    // Line clearing state
    bool isClearing;
    std::vector<int> linesToClear;
    uint64_t clearStartTime;
    int blinkCount;
    static constexpr uint64_t BLINK_INTERVAL_MS = 150;  // Blink every 150ms
    static constexpr int TOTAL_BLINKS = 8;  // Blink 8 times before clearing
    
    // Helper methods
    void spawnNewPiece();
    TetraType getRandomTetraType();
    bool canMove(int deltaX, int deltaY);
    bool rotateIfValid(bool clockwise);
    void lockPiece();
    void checkAndClearLines();
    std::vector<int> findCompleteLines();
    void removeLines(const std::vector<int>& lines);
    void updateLineClearAnimation();
    void renderTetrisGrid();
    void renderNextPiecePreview();
    void renderScoreBox();
    void updateGravity();
    
    LTexture background;
};

#endif // GAMESCENE_H
