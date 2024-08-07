#pragma once

#include <SDL2/SDL.h>

struct Color {
    Color(Uint8 r_, Uint8 g_, Uint8 b_, Uint8 a_) 
        : r(r_), g(g_), b(b_), a(a_) {}

    Color() : r(255), g(255), b(255), a(255) {}
    
    Uint8 r, g, b, a;
};
