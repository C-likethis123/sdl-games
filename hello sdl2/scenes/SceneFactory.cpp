//
//  SceneFactory.cpp
//  hello sdl2
//
//  Created by Chow Jia Ying on 29/11/25.
//

#include "SceneFactory.h"
#include "InitialScene.h"
#include "GameScene.h"
#include "GameOverScene.h"

std::unique_ptr<Scene> SceneFactory::createScene(const std::string& sceneKey) {
    if (sceneKey == "initial") {
        return std::make_unique<InitialScene>();
    } else if (sceneKey == "game" || sceneKey == "next") {
        // Support both "game" and "next" for backwards compatibility
        return std::make_unique<GameScene>();
    } else if (sceneKey == "gameover") {
        return std::make_unique<GameOverScene>();
    }
    
    // Default: return initial scene if unknown key
    return std::make_unique<InitialScene>();
}

