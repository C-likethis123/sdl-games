//
//  InitialScene.h
//  hello sdl2
//
//  Created by Chow Jia Ying on 30/10/25.
//

#ifndef INITIALSCENE_H
#define INITIALSCENE_H

#include "Scene.h"

class InitialScene : public Scene {
public:
    InitialScene();
    void initialise() override;
    void render() override;
};

#endif // INITIALSCENE_H
