# Tetris Implementation Guide

## Overview
The Tetris system consists of:
- **Tetra class**: Represents a single Tetris piece (tetromino)
- **TetrisScene**: Example scene showing how to use Tetra with keyboard controls

## Tetra Class

### All 7 Tetromino Types
Each piece has its own color and rotation behavior:

| Type | Shape | Color | Special Notes |
|------|-------|-------|---------------|
| `I` | Straight line | Cyan | 4 blocks, 2 unique rotations |
| `O` | Square | Yellow | 4 blocks, no rotation needed |
| `T` | T-shape | Purple | 4 blocks, 4 rotations |
| `S` | S-shape | Green | 4 blocks, 2 unique rotations |
| `Z` | Z-shape | Red | 4 blocks, 2 unique rotations |
| `J` | J-shape | Blue | 4 blocks, 4 rotations |
| `L` | L-shape | Orange | 4 blocks, 4 rotations |

### Creating a Tetris Piece

```cpp
// Create a T-piece at grid position (5, 0)
auto piece = std::make_unique<Tetra>(TetraType::T, 5, 0);

// Or create any random piece
TetraType type = static_cast<TetraType>(rand() % 7);
auto randomPiece = std::make_unique<Tetra>(type, 5, 0);
```

### Rendering

```cpp
void render() {
    SDL_Renderer* renderer = Globals::getRenderer();
    float cellSize = 30.0f;
    float gridStartX = 100.0f;
    float gridStartY = 50.0f;
    
    piece->render(renderer, cellSize, gridStartX, gridStartY);
}
```

### Rotation

```cpp
// Rotate clockwise (like pressing UP arrow)
piece->rotateClockwise();

// Rotate counter-clockwise (like pressing Z)
piece->rotateCounterClockwise();

// Current rotation state (0, 1, 2, or 3)
int rotation = piece->getRotation();
```

### Movement

```cpp
// Move left/right/down
piece->moveLeft();
piece->moveRight();
piece->moveDown();

// Get current position
int x = piece->getX();
int y = piece->getY();
```

### Collision Detection

```cpp
// Get all cells occupied by the piece
std::vector<std::pair<int, int>> cells = piece->getOccupiedCells();

// Example: Check if piece can move down
bool canMoveDown(Tetra* piece, const Grid& grid) {
    auto cells = piece->getOccupiedCells();
    
    for (const auto& [x, y] : cells) {
        int newY = y + 1;
        
        // Check if hits bottom
        if (newY >= GRID_HEIGHT) return false;
        
        // Check if hits locked piece
        if (grid[newY][x] == 1) return false;
    }
    
    return true;
}
```

## Arrow Key Controls (in TetrisScene)

| Key | Action |
|-----|--------|
| **←** Left Arrow | Move piece left |
| **→** Right Arrow | Move piece right |
| **↓** Down Arrow | Move piece down (soft drop) |
| **↑** Up Arrow / **Space** | Rotate clockwise |
| **Z** | Rotate counter-clockwise |

## Example: Integrating into Your Game

```cpp
// In main.cpp, add TetrisScene to your scene map:
#include "scenes/TetrisScene.h"

scenes["tetris"] = std::make_unique<TetrisScene>();
Globals::setScene("tetris");
```

## Shape Data Structure

Each piece uses a 4x4 grid where:
- `1` = filled cell
- `0` = empty cell

Example: T-piece rotation 0 (pointing up)
```
0 1 0 0
1 1 1 0
0 0 0 0
0 0 0 0
```

## Customization

### Change Colors
Edit `Tetra::setColorForType()` in Tetra.cpp:
```cpp
case TetraType::I:
    color = {0, 255, 255, 255};  // Change RGB values
    break;
```

### Change Cell Size
Modify in TetrisScene.h:
```cpp
static constexpr float CELL_SIZE = 30.0f;  // Change this
```

### Add More Rotations
Edit the shape arrays in Tetra.cpp (e.g., `I_SHAPES`, `T_SHAPES`, etc.)

## Next Steps

To complete a full Tetris game, you'll need to add:
1. **Gravity** - Auto-move pieces down over time
2. **Line clearing** - Detect and remove full rows
3. **Score system** - Track points
4. **Game over detection** - When pieces stack to the top
5. **Preview** - Show next piece
6. **Hold piece** - Allow swapping current piece

Would you like help implementing any of these features?

