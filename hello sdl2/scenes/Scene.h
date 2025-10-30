//
//  Scene.h
//  hello sdl2
//
//  Created by Chow Jia Ying on 29/10/25.
//

/**
 
 A scene is a new "page"
 Manages the widgets and elements that are rendered on the screen, and whether any of the elements needs to react to events.
 
 */

#ifndef SCENE_H
#define SCENE_H

#include "../Button.h"
#include <vector>
#include "../Texture.h"
#include <SDL3/SDL_main.h>

class Scene {
    public:
        Scene();
        virtual ~Scene();  // Defined in .cpp to anchor the vtable
        
        // Delete copy operations (Scene contains non-copyable members)
        Scene(const Scene&) = delete;
        Scene& operator=(const Scene&) = delete;
        
        // Default move operations (allow Scene to be moved)
        Scene(Scene&&) noexcept = default;
        Scene& operator=(Scene&&) noexcept = default;
        
        virtual void initialise() = 0;  // Pure virtual - must be implemented by derived classes
        virtual void render() = 0;      // Pure virtual - must be implemented by derived classes
        void handleEvent(SDL_Event& e);
        static const int SCREEN_WIDTH = 640;
        static const int SCREEN_HEIGHT = 480;
    protected:
        std::vector<Button> buttons;
        LTexture background;
};

#endif // SCENE_H
