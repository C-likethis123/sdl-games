//
//  TetrisScene.h
//  hello sdl2
//
//  Created by Chow Jia Ying on 24/11/25.
//

#ifndef TETRISSCENE_H
#define TETRISSCENE_H

#include "Scene.h"
#include "../Tetra.h"
#include <memory>

class TetrisScene : public Scene {
public:
    TetrisScene();
    void initialise() override;
    void render() override;
    void handleEvent(SDL_Event& e) override;  // Override to handle arrow keys
    
private:
    // Grid configuration
    static constexpr int GRID_WIDTH = 10;
    static constexpr int GRID_HEIGHT = 20;
    static constexpr float CELL_SIZE = 30.0f;
    
    // Calculate centered position
    const float GRID_START_X = (SCREEN_WIDTH - (GRID_WIDTH * CELL_SIZE)) / 2.0f;
    const float GRID_START_Y = 50.0f;
    
    // Current active piece
    std::unique_ptr<Tetra> currentPiece;
    
    // Game grid (0 = empty, 1 = occupied)
    std::vector<std::vector<int>> grid;
    
    // Helper methods
    void spawnNewPiece();
    bool canMove(int deltaX, int deltaY);
    bool canRotate(bool clockwise);
    void lockPiece();
    void renderGrid();
};

#endif // TETRISSCENE_H

