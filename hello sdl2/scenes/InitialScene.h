//
//  InitialScene.h
//  hello sdl2
//
//  Created by Chow Jia Ying on 1/11/25.
//
/**
 
 Initial scene shown when user opens the app.
 
 */

#ifndef INITIALSCENE_H
#define INITIALSCENE_H

#include "../Button.h"
#include "Scene.h"
#include <vector>
#include "../Texture.h"
#include <SDL3/SDL_main.h>

class InitialScene : public Scene {
    public:
        InitialScene();

        void initialise() override;  // Pure virtual - must be implemented by derived classes
        void render() override;      // Pure virtual - must be implemented by derived classes
    protected:
        LTexture background;
};

#endif // INITIALSCENE_H
