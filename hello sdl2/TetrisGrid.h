//
//  TetrisGrid.h
//  hello sdl2
//
//  Created by Chow Jia Ying on 29/11/25.
//


#ifndef TETRISGRID_H
#define TETRISGRID_H

#include <vector>
#include "Tetra.h"

class TetrisGrid {
public:
    TetrisGrid();
    ~TetrisGrid();
    bool isCellOccupied(int x, int y) const;
    bool isCellFree(int x, int y) const;
    void lockPiece(const Tetra& piece);
    std::vector<int> findCompleteLines();
    void removeLines(const std::vector<int>& lines);
    void render();
    void clear();
    
    // Getters for grid dimensions
    int getWidth() const { return TETRIS_GRID_WIDTH; }
    int getHeight() const { return TETRIS_GRID_HEIGHT; }
    float getCellSize() const { return TETRIS_CELL_SIZE; }
    float getStartX() const { return TETRIS_GRID_START_X; }
    float getStartY() const { return TETRIS_GRID_START_Y; }
    
    // Helper method for spawning pieces at the correct position
    std::pair<int, int> getSpawnPosition() const {
        return {TETRIS_GRID_WIDTH / 2 - 2, 0};
    }
private:
    static constexpr int TETRIS_GRID_WIDTH = 15;
    static constexpr int TETRIS_GRID_HEIGHT = 20;
    static constexpr float TETRIS_CELL_SIZE = 20.0f;
    static constexpr float TETRIS_GRID_START_X = 300.0f;
    static constexpr float TETRIS_GRID_START_Y = 50.0f;
    std::vector<std::vector<int>> tetrisGrid;
    std::vector<int> linesToClear;
    bool isClearing;
    uint64_t clearStartTime;
    int blinkCount;
    static constexpr uint64_t BLINK_INTERVAL_MS = 150;  // Blink every 150ms
    static constexpr int TOTAL_BLINKS = 8;  // Blink 8 times before clearing
    

};

#endif
