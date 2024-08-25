#include "Menu.h"
#include <SDL2/SDL_image.h>
#include "Game.h"
Menu::Menu(SDL_Renderer* renderer)
    : renderer(renderer), selectedOption(0), currentState(MenuState::MAIN), selectedDifficulty(0) {
    backgroundTexture = IMG_LoadTexture(renderer, "assets/images/imagen.png");
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
            switch (currentState) {
                case MenuState::MAIN:
                    switch (event.key.keysym.sym) {
                        case SDLK_UP:
                            selectedOption = (selectedOption - 1 + 3) % 3;
                            break;
                        case SDLK_DOWN:
                            selectedOption = (selectedOption + 1) % 3;
                            break;
                        case SDLK_RETURN:
                            if (selectedOption == 0) {
                                currentState = MenuState::DIFFICULTY;
                            } else if (selectedOption == 1) {
                                // TODO: Show Scores
                            } else if (selectedOption == 2) {
                                isRunning = false;
                            }
                            break;
                    }
                    break;
                case MenuState::DIFFICULTY:
                    switch (event.key.keysym.sym) {
                        case SDLK_UP:
                            selectedDifficulty = (selectedDifficulty - 1 + 3) % 3;
                            break;
                        case SDLK_DOWN:
                            selectedDifficulty = (selectedDifficulty + 1) % 3;
                            break;
                        case SDLK_RETURN:
                            // Here you should create a Game instance and start it with the selected difficulty
                            {
                                Game* game = new Game(renderer);
                                game->start(selectedDifficulty + 1); // Assuming difficulty levels are 1-based (1 for Easy, 2 for Medium, 3 for Hard)

                                // Run the game loop
                                bool gameRunning = true;
                                while (gameRunning) {
game->handleEvents();
                                    game->update();
                                    game->render();
                                    SDL_Delay(1000 / (10 + selectedDifficulty * 5)); // Adjust the delay based on difficulty
                                }

                                delete game;
                            }
                            break;
                        case SDLK_ESCAPE:
                            currentState = MenuState::MAIN;
                            break;
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
    SDL_Color white = {255, 255, 255, 255};

    renderText("SNAKE GAME", SCREEN_WIDTH / 2, 100, green, true);

    if (currentState == MenuState::MAIN) {
        renderText("Iniciar Juego", SCREEN_WIDTH / 2, 250, selectedOption == 0 ? green : white, true);
        renderText("Score", SCREEN_WIDTH / 2, 350, selectedOption == 1 ? green : white, true);
        renderText("Salir", SCREEN_WIDTH / 2, 450, selectedOption == 2 ? green : white, true);
    } else if (currentState == MenuState::DIFFICULTY) {
        renderText("Selecciona la dificultad:", SCREEN_WIDTH / 2, 200, white, true);
        renderText("Fácil", SCREEN_WIDTH / 2, 300, selectedDifficulty == 0 ? green : white, true);
        renderText("Medio", SCREEN_WIDTH / 2, 400, selectedDifficulty == 1 ? green : white, true);
        renderText("Difícil", SCREEN_WIDTH / 2, 500, selectedDifficulty == 2 ? green : white, true);
    }

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
