#ifndef FOOD_H
#define FOOD_H

#include <SDL2/SDL.h>
#include "Position.h"

struct Position;

class Food {
public:
    Food(SDL_Renderer* renderer);
    ~Food();

    Position getPosition();
    void respawn();
    void render();

private:
    SDL_Renderer* renderer;
    Position position;
    SDL_Texture* foodTexture;

    void loadResources();
    void cleanupResources();
};

#endif // FOOD_H

