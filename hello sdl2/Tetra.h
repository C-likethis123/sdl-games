//
//  Tetra.h
//  hello sdl2
//
//  Created by Chow Jia Ying on 24/11/25.
//

#ifndef TETRA_H
#define TETRA_H

#include <SDL3/SDL.h>
#include <array>
#include <vector>

enum class TetraType {
    I,  // Straight piece (cyan)
    O,  // Square piece (yellow)
    T,  // T-shaped piece (purple)
    S,  // S-shaped piece (green)
    Z,  // Z-shaped piece (red)
    J,  // J-shaped piece (blue)
    L   // L-shaped piece (orange)
};

class Tetra {
public:
    Tetra(TetraType type, int gridX, int gridY);
    
    // Rendering
    void render(SDL_Renderer* renderer, float cellSize, float gridStartX, float gridStartY);
    
    // Rotation
    void rotateClockwise();
    void rotateCounterClockwise();
    
    // Movement
    void moveLeft();
    void moveRight();
    void moveDown();
    
    // Setters
    void setPosition(int x, int y) {
        gridX = x;
        gridY = y;
    }
    void setX(int x) { gridX = x; }
    void setY(int y) { gridY = y; }
    
    // Getters
    TetraType getType() const { return type; }
    int getRotation() const { return rotation; }
    int getX() const { return gridX; }
    int getY() const { return gridY; }
    const std::vector<std::vector<int>>& getShape() const;
    SDL_Color getColor() const { return color; }
    
    // Get occupied cells in grid coordinates
    std::vector<std::pair<int, int>> getOccupiedCells() const;
    
private:
    TetraType type;
    int gridX;  // Position in grid
    int gridY;
    int rotation;  // 0, 1, 2, 3 (clockwise rotations)
    SDL_Color color;
    
    // Shape definitions for each type and rotation
    // 1 = filled, 0 = empty
    static const std::vector<std::vector<std::vector<int>>> I_SHAPES;
    static const std::vector<std::vector<std::vector<int>>> O_SHAPES;
    static const std::vector<std::vector<std::vector<int>>> T_SHAPES;
    static const std::vector<std::vector<std::vector<int>>> S_SHAPES;
    static const std::vector<std::vector<std::vector<int>>> Z_SHAPES;
    static const std::vector<std::vector<std::vector<int>>> J_SHAPES;
    static const std::vector<std::vector<std::vector<int>>> L_SHAPES;
    
    void setColorForType();
    const std::vector<std::vector<std::vector<int>>>& getShapesForType() const;
};

#endif // TETRA_H
