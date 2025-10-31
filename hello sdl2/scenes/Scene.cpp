//
//  Scene.cpp
//  hello sdl2
//
//  Created by Chow Jia Ying on 29/10/25.
//

#include "Scene.h"

Scene::Scene() {
    // Note: Cannot call initialise() here since it's pure virtual
    // Derived classes must call their own initialise() after construction
}

Scene::~Scene() {
    // Virtual destructor defined in .cpp to anchor vtable and generate typeinfo
}

void Scene::handleEvent(SDL_Event& e) {
    for (auto& button : buttons) {
        bool isHovered = button.isMouseOver(e);
        button.setHovered(isHovered);  // Always update hover state (true or false)
        
        if (button.isClicked(e)) {
            button.onClick();
            break;
        }
    }
}
