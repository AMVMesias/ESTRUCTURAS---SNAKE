#include "Engine.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

Engine::Engine() : window(nullptr), renderer(nullptr), music(nullptr), menu(nullptr), game(nullptr), score(nullptr), isRunning(true) {}

Engine::~Engine() {
    cleanup();
}

bool Engine::init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) return false;
    if (IMG_Init(IMG_INIT_PNG) == 0) return false;
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) return false;
    if (TTF_Init() == -1) return false;

    window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) return false;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) return false;

    menu = new Menu(renderer);
    game = new Game(renderer);
    score = new Score(renderer);

    music = Mix_LoadMUS("assets/music/musica.mp3");
    if (!music) return false;

    Mix_PlayMusic(music, -1);

    return true;
}

void Engine::run() {
    while (isRunning) {
        menu->handleEvents(isRunning);
        menu->render();
    }
}

void Engine::cleanup() {
    Mix_HaltMusic();
    Mix_FreeMusic(music);
    delete menu;
    delete game;
    delete score;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_Quit();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}
