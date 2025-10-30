//
//  GameScene.h
//  hello sdl2
//
//  Created by Chow Jia Ying on 31/10/25.
//

#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "Scene.h"

class GameScene : public Scene {
public:
    GameScene();
    void initialise() override;
    void render() override;
};

#endif // GAMESCENE_H
