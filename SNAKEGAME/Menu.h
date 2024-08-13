#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Menu {
public:
    Menu(SDL_Renderer* renderer);
    ~Menu();

    void handleEvents(bool &isRunning);
    void render();

private:
    SDL_Renderer* renderer;
    SDL_Texture* backgroundTexture;
    TTF_Font* font;
    int selectedOption;


    void renderText(const char* text, int x, int y, SDL_Color color, bool center = false);

};

#endif // MENU_H

