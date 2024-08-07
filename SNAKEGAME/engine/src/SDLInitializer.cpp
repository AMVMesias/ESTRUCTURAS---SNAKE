#include "SDLInitializer.h"

#include <stdexcept>

SDLInitializer::SDLInitializer(Uint32 flags) {
    if (SDL_Init(flags) != 0) {
        throw std::runtime_error("Failed to init SDL");
    }
}

SDLInitializer::~SDLInitializer() {
    SDL_Quit();
}
