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
#include "../TetrisGrid.h"
#include "../ScoreBox.h"
#include "../NextPiecePreview.h"
#include "../GhostPieceRenderer.h"

class GameScene : public Scene {
public:
    GameScene();
    
    void initialise() override;
    void render() override;
    void handleEvent(SDL_Event& e) override;
    
private:
    // Tetris grid configuration
    TetrisGrid tetrisGrid;
    
    // UI components
    ScoreBox scoreBox;
    NextPiecePreview nextPiecePreview;
    GhostPieceRenderer ghostPieceRenderer;
    
    // Tetris state
    std::unique_ptr<Tetra> currentPiece;
    std::unique_ptr<Tetra> nextPiece;
    uint64_t lastMoveTime;
    static constexpr uint64_t MOVE_DELAY_MS = 1000;  // Auto-move down every 1s

    // Helper methods
    void reset();
    bool checkGameOver();
    void spawnNewPiece();
    TetraType getRandomTetraType();
    bool canMove(int deltaX, int deltaY);
    bool rotateIfValid(bool clockwise);
    void hardDrop();
    void updateLineClearAnimation(); // TODO: Needs refactoring
    void updateGravity();
    
    LTexture background;
};

#endif // GAMESCENE_H
