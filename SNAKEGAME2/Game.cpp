#include "Game.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

Game::Game(SDL_Renderer* renderer) : renderer(renderer), snake(nullptr), food(nullptr), difficulty(1) {
    loadResources();
}

Game::~Game() {
    cleanupResources();
}

void Game::loadResources() {
    backgroundTexture = IMG_LoadTexture(renderer, "assets/images/imagen.png");
    gameMusic = Mix_LoadMUS("assets/music/musica.mp3");
}

void Game::cleanupResources() {
    SDL_DestroyTexture(backgroundTexture);
    Mix_FreeMusic(gameMusic);
    delete snake;
    delete food;
}

void Game::start(int difficulty) {
    this->difficulty = difficulty;
    snake = new Snake(renderer, difficulty);
    food = new Food(renderer);

    Mix_PlayMusic(gameMusic, -1);
}

void Game::handleEvents(bool &isRunning) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        } else {
            snake->handleInput(event);
        }
    }
}

void Game::update() {
    snake->move();
    if (snake->checkCollision(food->getPosition())) {
        food->respawn();
        snake->grow();
    }
    // Check other collisions (with walls, self, etc.) - TO DO
}

void Game::render() {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    snake->render();
    food->render();
    SDL_RenderPresent(renderer);
}

