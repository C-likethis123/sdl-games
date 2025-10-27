//
//  Dot.cpp
//  hello sdl2
//
//  Created by Chow Jia Ying on 26/10/25.
//

#include "Dot.h"
#include "CollisionUtil.h"

Dot::Dot() {
    if (!dotTexture.loadFromFile("dot.bmp", 0xFF, 0xff, 0xff)) {
    
        SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Error loading dot.bmp, %s", SDL_GetError());
    }
    
    collisionBox.w = DOT_WIDTH;
    collisionBox.h = DOT_HEIGHT;
};

void Dot::handleEvent() {
    SDL_PumpEvents();
    const bool* state = SDL_GetKeyboardState(nullptr);
    if ( state[SDL_SCANCODE_UP] ) {
        mVelY -= DOT_VEL;
    } else if ( state[SDL_SCANCODE_DOWN]) {
        mVelY += DOT_VEL;
    } else if ( state[SDL_SCANCODE_LEFT]) {
        mVelX -= DOT_VEL;
    } else if (state[SDL_SCANCODE_RIGHT]) {
        mVelX += DOT_VEL;
    }
    
}

void Dot::move(SDL_FRect& wall)
{
    //Move the dot left or right
    mPosX += mVelX;
    collisionBox.x = mPosX;
    
    //If the dot went too far to the left or right
    if( ( mPosX < 0 ) || ( mPosX + DOT_WIDTH > LEVEL_WIDTH ) || checkCollision( collisionBox, wall ) )
    {
        //Move back
        mPosX -= mVelX;
        collisionBox.x = mPosX;
    }
    //Move the dot up or down
    mPosY += mVelY;
    collisionBox.y = mPosY;

    //If the dot went too far up or down
    if( ( mPosY < 0 ) || ( mPosY + DOT_HEIGHT > LEVEL_HEIGHT )  || checkCollision( collisionBox, wall ) )
    {
        //Move back
        mPosY -= mVelY;
        collisionBox.y = mPosY;
    }
}

void Dot::render(int camX, int camY) {
    dotTexture.render(mPosX - camX, mPosY - camY);
}

int Dot::getPosX() const {
    return mPosX;
}

int Dot::getPosY() const {
    return mPosY;
}
