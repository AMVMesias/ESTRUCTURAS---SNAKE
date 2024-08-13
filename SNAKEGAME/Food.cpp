#include "Food.h"
#include <cstdlib>
#include <ctime>
#include <SDL2/SDL_image.h>



Food::Food(SDL_Renderer* renderer) : renderer(renderer) {
    srand(static_cast<unsigned int>(time(nullptr)));
    loadResources();
    respawn();
}

Food::~Food() {
    cleanupResources();
}

void Food::loadResources() {
    foodTexture = IMG_LoadTexture(renderer, "assets/images/food.png");
}

void Food::cleanupResources() {
    SDL_DestroyTexture(foodTexture);
}

Position Food::getPosition() {
    return position;
}

void Food::respawn() {
    position.x = (rand() % 40) * 20;
    position.y = (rand() % 30) * 20;
}

void Food::render() {
    SDL_Rect rect = {position.x, position.y, 20, 20};
    SDL_RenderCopy(renderer, foodTexture, NULL, &rect);
}

