//
//  EndingScene.h
//  hello sdl2
//
//  Created by Chow Jia Ying on 31/10/25.
//

#ifndef ENDINGSCENE_H
#define ENDINGSCENE_H

#include "Scene.h"
#include <SDL3/SDL.h>

class EndingScene : public Scene {
public:
    EndingScene();
    void initialise() override;
    void render() override;
    void onEnter();  // Called when scene becomes active
private:
    uint64_t startTime;
    static constexpr uint64_t DISPLAY_DURATION_MS = 5000;  // 5 seconds
};

#endif // ENDINGSCENE_H
