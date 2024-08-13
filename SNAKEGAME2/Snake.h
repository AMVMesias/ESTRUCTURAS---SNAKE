#ifndef SNAKE_H
#define SNAKE_H

#include <SDL2/SDL.h>
#include <vector>
#include "Position.h"



class Snake {
public:
    Snake(SDL_Renderer* renderer, int difficulty);
    ~Snake();

    void handleInput(SDL_Event &event);
    void move();
    void grow();
    void render();
    bool checkCollision(Position foodPosition);

private:
    SDL_Renderer* renderer;
    std::vector<Position> body;
    Position direction;
    int speed;
    SDL_Texture* snakeTexture;

    void loadResources();
    void cleanupResources();
};

#endif // SNAKE_H

