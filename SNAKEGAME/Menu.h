#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

enum class MenuState {
    MAIN,
    DIFFICULTY,
    INSTRUCTIONS
};

class Menu {
public:
    Menu(SDL_Renderer* renderer);
    ~Menu();
    void handleEvents(bool &isRunning);
    void render();
    void showScores();
    void showInstructions();
    void show();
    void playMusic();
    void stopMusic();

private:
    SDL_Renderer* renderer;
    SDL_Texture* backgroundTexture;
    TTF_Font* font;
    int selectedOption;
    MenuState currentState;
    int selectedDifficulty;
    Mix_Music* menuMusic;

    void renderText(const char* text, int x, int y, SDL_Color color, bool center);
};
