#ifndef ENGINE_H
#define ENGINE_H


#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "Menu.h"
#include "Game.h"
#include "Score.h"

class Engine {
public:
    Engine();
    ~Engine();

    bool init();
    void run();
    void cleanup();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    Mix_Music* music;

    Menu* menu;
    Game* game;
    Score* score;

    bool isRunning;
};

#endif // ENGINE_H
