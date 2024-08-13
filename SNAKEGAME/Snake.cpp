#include "Snake.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

Snake::Snake(SDL_Renderer* renderer, int difficulty) : renderer(renderer), direction({1, 0}), speed(difficulty * 2) {
    body.push_back({200, 200});
    loadResources();
}

Snake::~Snake() {
    cleanupResources();
}

void Snake::loadResources() {
    snakeTexture = IMG_LoadTexture(renderer, "assets/images/snake.png");
}

void Snake::cleanupResources() {
    SDL_DestroyTexture(snakeTexture);
}

void Snake::handleInput(SDL_Event &event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_UP:
                direction = {0, -1};
                break;
            case SDLK_DOWN:
                direction = {0, 1};
                break;
            case SDLK_LEFT:
                direction = {-1, 0};
                break;
            case SDLK_RIGHT:
                direction = {1, 0};
                break;
        }
    }
}

void Snake::move() {
    for (size_t i = body.size() - 1; i > 0; --i) {
        body[i] = body[i - 1];
    }
    body[0].x += direction.x * speed;
    body[0].y += direction.y * speed;

    // Collision detection with boundaries and self - TO DO
}

void Snake::grow() {
    body.push_back(body.back());
}

void Snake::render() {
    for (const auto &segment : body) {
        SDL_Rect rect = {segment.x, segment.y, 20, 20};
        SDL_RenderCopy(renderer, snakeTexture, NULL, &rect);
    }
}

bool Snake::checkCollision(Position foodPosition) {
    return (body[0].x == foodPosition.x && body[0].y == foodPosition.y);
}

