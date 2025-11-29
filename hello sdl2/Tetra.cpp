//
//  Tetra.cpp
//  hello sdl2
//
//  Created by Chow Jia Ying on 24/11/25.
//

#include "Tetra.h"
#include "Colors.h"

// I-piece (Cyan) - Straight line
// Rotations: 0 = horizontal, 1 = vertical, 2 = horizontal, 3 = vertical
const std::vector<std::vector<std::vector<int>>> Tetra::I_SHAPES = {
    {   // Rotation 0: Horizontal
        {0, 0, 0, 0},
        {1, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {   // Rotation 1: Vertical
        {0, 0, 1, 0},
        {0, 0, 1, 0},
        {0, 0, 1, 0},
        {0, 0, 1, 0}
    },
    {   // Rotation 2: Horizontal
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {1, 1, 1, 1},
        {0, 0, 0, 0}
    },
    {   // Rotation 3: Vertical
        {0, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 1, 0, 0}
    }
};

// O-piece (Yellow) - Square (doesn't rotate)
const std::vector<std::vector<std::vector<int>>> Tetra::O_SHAPES = {
    {
        {0, 1, 1, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {
        {0, 1, 1, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {
        {0, 1, 1, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {
        {0, 1, 1, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    }
};

// T-piece (Purple)
const std::vector<std::vector<std::vector<int>>> Tetra::T_SHAPES = {
    {   // Rotation 0: T pointing up
        {0, 1, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {   // Rotation 1: T pointing right
        {0, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 0}
    },
    {   // Rotation 2: T pointing down
        {0, 0, 0, 0},
        {1, 1, 1, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 0}
    },
    {   // Rotation 3: T pointing left
        {0, 1, 0, 0},
        {1, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 0}
    }
};

// S-piece (Green)
const std::vector<std::vector<std::vector<int>>> Tetra::S_SHAPES = {
    {   // Rotation 0
        {0, 1, 1, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {   // Rotation 1
        {0, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 0}
    },
    {   // Rotation 2
        {0, 0, 0, 0},
        {0, 1, 1, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0}
    },
    {   // Rotation 3
        {1, 0, 0, 0},
        {1, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 0}
    }
};

// Z-piece (Red)
const std::vector<std::vector<std::vector<int>>> Tetra::Z_SHAPES = {
    {   // Rotation 0
        {1, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {   // Rotation 1
        {0, 0, 1, 0},
        {0, 1, 1, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 0}
    },
    {   // Rotation 2
        {0, 0, 0, 0},
        {1, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0}
    },
    {   // Rotation 3
        {0, 1, 0, 0},
        {1, 1, 0, 0},
        {1, 0, 0, 0},
        {0, 0, 0, 0}
    }
};

// J-piece (Blue)
const std::vector<std::vector<std::vector<int>>> Tetra::J_SHAPES = {
    {   // Rotation 0
        {1, 0, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {   // Rotation 1
        {0, 1, 1, 0},
        {0, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 0}
    },
    {   // Rotation 2
        {0, 0, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 0}
    },
    {   // Rotation 3
        {0, 1, 0, 0},
        {0, 1, 0, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0}
    }
};

// L-piece (Orange)
const std::vector<std::vector<std::vector<int>>> Tetra::L_SHAPES = {
    {   // Rotation 0
        {0, 0, 1, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {   // Rotation 1
        {0, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0}
    },
    {   // Rotation 2
        {0, 0, 0, 0},
        {1, 1, 1, 0},
        {1, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {   // Rotation 3
        {1, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 0}
    }
};

Tetra::Tetra(TetraType type, int gridX, int gridY) 
    : type(type), gridX(gridX), gridY(gridY), rotation(0) {
    setColorForType();
}

void Tetra::setColorForType() {
    switch (type) {
        case TetraType::I:
            color = Colors::cyan;
            break;
        case TetraType::O:
            color = Colors::yellow;
            break;
        case TetraType::T:
            color = Colors::purple;
            break;
        case TetraType::S:
            color = Colors::green;
            break;
        case TetraType::Z:
            color = Colors::red;
            break;
        case TetraType::J:
            color = Colors::blue;
            break;
        case TetraType::L:
            color = Colors::orange;
            break;
    }
}

const std::vector<std::vector<std::vector<int>>>& Tetra::getShapesForType() const {
    switch (type) {
        case TetraType::I: return I_SHAPES;
        case TetraType::O: return O_SHAPES;
        case TetraType::T: return T_SHAPES;
        case TetraType::S: return S_SHAPES;
        case TetraType::Z: return Z_SHAPES;
        case TetraType::J: return J_SHAPES;
        case TetraType::L: return L_SHAPES;
    }
    return I_SHAPES; // Fallback
}

const std::vector<std::vector<int>>& Tetra::getShape() const {
    return getShapesForType()[rotation];
}

void Tetra::render(SDL_Renderer* renderer, float cellSize, float gridStartX, float gridStartY) const {
    const auto& shape = getShape();
    
    // Set color for this piece
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    
    // Render each filled cell
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (shape[row][col] == 1) {
                // Calculate screen position
                float x = gridStartX + (gridX + col) * cellSize;
                float y = gridStartY + (gridY + row) * cellSize;
                
                SDL_FRect rect = {x, y, cellSize, cellSize};
                SDL_RenderFillRect(renderer, &rect);
                
                // Draw border for better visibility
                SDL_SetRenderDrawColor(renderer, Colors::black.r, Colors::black.g, Colors::black.b, Colors::black.a);
                SDL_RenderRect(renderer, &rect);
                SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            }
        }
    }
}

void Tetra::rotateClockwise() {
    rotation = (rotation + 1) % 4;
}

void Tetra::rotateCounterClockwise() {
    rotation = (rotation - 1 + 4) % 4;
}

void Tetra::moveLeft() {
    gridX--;
}

void Tetra::moveRight() {
    gridX++;
}

void Tetra::moveDown() {
    gridY++;
}

std::vector<std::pair<int, int>> Tetra::getOccupiedCells() const {
    std::vector<std::pair<int, int>> cells;
    const auto& shape = getShape();
    
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (shape[row][col] == 1) {
                cells.push_back({gridX + col, gridY + row});
            }
        }
    }
    
    return cells;
}

