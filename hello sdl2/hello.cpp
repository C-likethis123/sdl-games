#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Key press surfaces constants
enum KeyPressSurfaces
{
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
//The images that correspond to a keypress
SDL_Texture* gKeyPressSurfaces[ KEY_PRESS_SURFACE_TOTAL ];
//Current displayed image
SDL_Texture* gCurrentTexture = NULL;
static const char* BASE_PATH;

// Initialize SDL, window, and renderer
bool initialise() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    gWindow = SDL_CreateWindow("Mai Tutorial", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!gWindow) {
        SDL_Log("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, nullptr);
    if (!gRenderer) {
        SDL_Log("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

// Helper to get full resource path
std::string getResourcePath(const std::string& filename) {
    if (!BASE_PATH) {
        BASE_PATH = SDL_GetBasePath();
    }
    // checks if initial allocation is successful
    if (!BASE_PATH) {
        return filename;
    }
    std::string path = std::string(BASE_PATH) + filename;
    return path;
}

// Load BMP image and create a texture
SDL_Texture* load_bmp_image(const std::string& file_name) {
    std::string path = getResourcePath(file_name);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Loading image at: %s\n", path.c_str());

    SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
    SDL_Texture* loadedTexture = NULL;
    if (!loadedSurface) {
        SDL_Log("Unable to load image! SDL Error: %s\n", SDL_GetError());
        return loadedTexture;
    }

    // Create texture from surface
    loadedTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    SDL_DestroySurface(loadedSurface);

    if (!loadedTexture) {
        SDL_Log("Unable to create texture! SDL Error: %s\n", SDL_GetError());
        return loadedTexture;
    }

    return loadedTexture;
}

bool load_media() {
    //Loading success flag
    bool success = true;

    //Load default surface
    gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] = load_bmp_image( "press.bmp" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] == NULL )
    {
        printf( "Failed to load default image!\n" );
        success = false;
    }

    //Load up surface
    gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ] = load_bmp_image( "up.bmp" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ] == NULL )
    {
        printf( "Failed to load up image!\n" );
        success = false;
    }

    //Load down surface
    gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ] = load_bmp_image( "down.bmp" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ] == NULL )
    {
        printf( "Failed to load down image!\n" );
        success = false;
    }

    //Load left surface
    gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ] = load_bmp_image( "left.bmp" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ] == NULL )
    {
        printf( "Failed to load left image!\n" );
        success = false;
    }

    //Load right surface
    gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ] = load_bmp_image( "right.bmp" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ] == NULL )
    {
        printf( "Failed to load right image!\n" );
        success = false;
    }

    return success;
}

// Free resources and quit SDL
void close() {
    for (int i = 0; i < KEY_PRESS_SURFACE_TOTAL; ++i) {
        if (gKeyPressSurfaces[i]) {
            SDL_DestroyTexture(gKeyPressSurfaces[i]);
            gKeyPressSurfaces[i] = nullptr;
        }
    }

    if (gRenderer) {
        SDL_DestroyRenderer(gRenderer);
        gRenderer = nullptr;
    }

    if (gWindow) {
        SDL_DestroyWindow(gWindow);
        gWindow = nullptr;
    }

    SDL_Quit();
}

int main(int argc, char* args[]) {
    if (!initialise()) return 1;
    if (!load_media()) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Failed to load media");
        close();
    }

    // Clear screen to white
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    SDL_RenderClear(gRenderer);

    // Draw the texture
    gCurrentTexture = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
    
    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
            else if( e.type == SDL_EVENT_KEY_DOWN ) {
                //Select surfaces based on key press
                switch( e.key.key )
                {
                    case SDLK_UP:
                    gCurrentTexture = gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ];
                    break;

                    case SDLK_DOWN:
                    gCurrentTexture = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ];
                    break;

                    case SDLK_LEFT:
                    gCurrentTexture = gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ];
                    break;

                    case SDLK_RIGHT:
                    gCurrentTexture = gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ];
                    break;

                    default:
                    gCurrentTexture = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];
                    break;
                }
            }
        }

     
        
        SDL_RenderTexture(gRenderer, gCurrentTexture, nullptr, nullptr);
        // Present to screen
        SDL_RenderPresent(gRenderer);
    }

    close();
    return 0;
}
