#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "Snake.h"
#include "Food.h"

class Game {
public:
    Game(SDL_Renderer* renderer);
    ~Game();

    void start(int difficulty);
    void handleEvents(bool &isRunning);
    void update();
    void render();

private:
    SDL_Renderer* renderer;
    Snake* snake;
    Food* food;
    int difficulty;
    SDL_Texture* backgroundTexture;
    Mix_Music* gameMusic;

    void loadResources();
    void cleanupResources();
};

#endif // GAME_H

