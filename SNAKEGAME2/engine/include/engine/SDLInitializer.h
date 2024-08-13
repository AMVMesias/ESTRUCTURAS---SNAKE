#pragma once

#include <SDL2/SDL.h>

class SDLInitializer {
public:
    SDLInitializer(Uint32 flags);
    ~SDLInitializer();
};
