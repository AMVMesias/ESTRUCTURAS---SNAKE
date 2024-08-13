#include "SDLImageInitializer.h"

#include <SDL2/SDL_image.h>

#include <stdexcept>
#include <string>

SDLImageInitializer::SDLImageInitializer() {
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        throw std::runtime_error("Failed to initialize SDL_image: " + std::string(IMG_GetError()));
    }
}

SDLImageInitializer::~SDLImageInitializer() {
    IMG_Quit();
}