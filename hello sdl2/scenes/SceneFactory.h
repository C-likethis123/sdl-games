//
//  SceneFactory.h
//  hello sdl2
//
//  Created by Chow Jia Ying on 29/11/25.
//

#ifndef SCENEFACTORY_H
#define SCENEFACTORY_H

#include <memory>
#include <string>
#include "Scene.h"

class SceneFactory {
public:
    // Create a scene based on the scene key
    static std::unique_ptr<Scene> createScene(const std::string& sceneKey);
};

#endif // SCENEFACTORY_H

