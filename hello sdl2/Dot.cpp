//
//  Dot.cpp
//  hello sdl2
//
//  Created by Chow Jia Ying on 26/10/25.
//

#include "Dot.h"

Dot::Dot() {
    if (!dotTexture.loadFromFile("dot.bmp", 0xFF, 0xff, 0xff)) {
    
        SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Error loading dot.bmp, %s", SDL_GetError());
    }
};

void Dot::handleEvent(SDL_Event& e) {
    if( e.type == SDL_EVENT_KEY_DOWN && e.key.repeat == 0 )
        {
            //Adjust the velocity
            switch( e.key.key )
            {
                case SDLK_UP: mVelY -= DOT_VEL; break;
                case SDLK_DOWN: mVelY += DOT_VEL; break;
                case SDLK_LEFT: mVelX -= DOT_VEL; break;
                case SDLK_RIGHT: mVelX += DOT_VEL; break;
            }
        }
}

void Dot::move()
{
    //Move the dot left or right
    mPosX += mVelX;
    
    //If the dot went too far to the left or right
    if( ( mPosX < 0 ) || ( mPosX + DOT_WIDTH > 640 ) )
    {
        //Move back
        mPosX -= mVelX;
    }
    //Move the dot up or down
        mPosY += mVelY;

    //If the dot went too far up or down
    if( ( mPosY < 0 ) || ( mPosY + DOT_HEIGHT > 480 ) )
    {
        //Move back
        mPosY -= mVelY;
    }
}

void Dot::render() {
    dotTexture.render(mPosX, mPosY);
}

