#include "Menu.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

// Define the screen width and height (Adjust these values to match your screen size)
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

Menu::Menu(SDL_Renderer* renderer) : renderer(renderer), selectedOption(0) {
    backgroundTexture = IMG_LoadTexture(renderer, "assets/images/imagen.png");
    // Increase font size to 48
    font = TTF_OpenFont("assets/fonts/fuente.ttf", 30);
}

Menu::~Menu() {
    SDL_DestroyTexture(backgroundTexture);
    TTF_CloseFont(font);
}

void Menu::handleEvents(bool &isRunning) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_UP:
                    selectedOption = (selectedOption - 1 + 3) % 3;
                    break;
                case SDLK_DOWN:
                    selectedOption = (selectedOption + 1) % 3;
                    break;
                case SDLK_RETURN:
                    if (selectedOption == 0) {
                        // TODO: Start the game
                    } else if (selectedOption == 1) {
                        // TODO: Show Scores
                    } else if (selectedOption == 2) {
                        isRunning = false;
                    }
                    break;
            }
        }
    }
}

void Menu::render() {
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

    SDL_Color green = {0, 255, 0, 255};
    renderText("SNAKE GAME", SCREEN_WIDTH / 2, 100, green, true);

    SDL_Color white = {255, 255, 255, 255};
    renderText("Iniciar Juego", SCREEN_WIDTH / 2, 250, selectedOption == 0 ? green : white, true);
    renderText("Score", SCREEN_WIDTH / 2, 350, selectedOption == 1 ? green : white, true);
    renderText("Salir", SCREEN_WIDTH / 2, 450, selectedOption == 2 ? green : white, true);

    SDL_RenderPresent(renderer);
}

void Menu::renderText(const char* text, int x, int y, SDL_Color color, bool center) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect destRect;

    if (center) {
        destRect = { x - (surface->w / 2), y, surface->w, surface->h };
    } else {
        destRect = { x, y, surface->w, surface->h };
    }

    SDL_RenderCopy(renderer, texture, NULL, &destRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
