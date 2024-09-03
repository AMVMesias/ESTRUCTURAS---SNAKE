#include "Menu.h"
#include <SDL2/SDL_image.h>
#include "Game.h"
#include "Score.h"

Menu::Menu(SDL_Renderer* renderer)
    : renderer(renderer), selectedOption(0), currentState(MenuState::MAIN), selectedDifficulty(0) {
    backgroundTexture = IMG_LoadTexture(renderer, "assets/images/imagen.png");
    font = TTF_OpenFont("assets/fonts/fuente.ttf", 30);
    menuMusic = Mix_LoadMUS("assets/music/musica.mp3");  // Asegúrate de tener esta música
}

Menu::~Menu() {
    SDL_DestroyTexture(backgroundTexture);
    TTF_CloseFont(font);
    Mix_FreeMusic(menuMusic);
}

void Menu::playMusic() {
    if (Mix_PlayingMusic() == 0) {
        Mix_PlayMusic(menuMusic, -1);  // -1 para reproducir en loop
    } else if (Mix_PausedMusic() == 1) {
        Mix_ResumeMusic();
    }
}

void Menu::stopMusic() {
    Mix_HaltMusic();
}

void Menu::show() {
    currentState = MenuState::MAIN;
    playMusic();  // Reproducir música del menú
    bool menuRunning = true;
    while (menuRunning) {
        handleEvents(menuRunning);
        render();
        SDL_Delay(1000 / 60); // 60 FPS
    }
}

void Menu::handleEvents(bool &isRunning) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
            exit(0);
        } else if (event.type == SDL_KEYDOWN) {
            switch (currentState) {
                case MenuState::MAIN:
                    switch (event.key.keysym.sym) {
                        case SDLK_UP:
                            selectedOption = (selectedOption - 1 + 4) % 4;
                            break;
                        case SDLK_DOWN:
                            selectedOption = (selectedOption + 1) % 4;
                            break;
                        case SDLK_RETURN:
                            if (selectedOption == 0) {
                                currentState = MenuState::DIFFICULTY;
                            } else if (selectedOption == 1) {
                                showInstructions();
                            } else if (selectedOption == 2) {
                                showScores();
                            } else if (selectedOption == 3) {
                                isRunning = false;
                                exit(0);
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
                            {
                                stopMusic();  // Detener la música del menú
                                Game* game = new Game(renderer, this);
                                game->iniciar(selectedDifficulty + 1);
                                delete game;
                                playMusic();  // Reiniciar la música del menú
                            }
                            break;
                        case SDLK_ESCAPE:
                            currentState = MenuState::MAIN;
                            break;
                    }
                    break;
                case MenuState::INSTRUCTIONS:
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        currentState = MenuState::MAIN;
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
        renderText("Cómo Jugar", SCREEN_WIDTH / 2, 300, selectedOption == 1 ? green : white, true);
        renderText("Score", SCREEN_WIDTH / 2, 350, selectedOption == 2 ? green : white, true);
        renderText("Salir", SCREEN_WIDTH / 2, 400, selectedOption == 3 ? green : white, true);
    } else if (currentState == MenuState::DIFFICULTY) {
        renderText("Selecciona la dificultad:", SCREEN_WIDTH / 2, 200, white, true);
        renderText("Fácil", SCREEN_WIDTH / 2, 300, selectedDifficulty == 0 ? green : white, true);
        renderText("Medio", SCREEN_WIDTH / 2, 400, selectedDifficulty == 1 ? green : white, true);
        renderText("Difícil", SCREEN_WIDTH / 2, 500, selectedDifficulty == 2 ? green : white, true);
    } else if (currentState == MenuState::INSTRUCTIONS) {
        TTF_CloseFont(font);
        font = TTF_OpenFont("assets/fonts/fuente.ttf", 20);
        renderText("Instrucciones:", SCREEN_WIDTH / 2, 150, white, true);
        renderText("Flechas de dirección: Mover la serpiente", SCREEN_WIDTH / 2, 250, white, true);
        renderText("WASD: Mover la serpiente", SCREEN_WIDTH / 2, 300, white, true);
        renderText("Enter: Seleccionar opción", SCREEN_WIDTH / 2, 350, white, true);
        renderText("Esc: Retroceder / Pausar", SCREEN_WIDTH / 2, 400, white, true);
        renderText("Presiona Esc para volver al menú", SCREEN_WIDTH / 2, 500, white, true);
        TTF_CloseFont(font);
        font = TTF_OpenFont("assets/fonts/fuente.ttf", 30);
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

void Menu::showScores() {
    Score scoreBoard(renderer);
    scoreBoard.display();

    SDL_Event event;
    bool backToMenu = false;
    while (!backToMenu) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                exit(0);
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                backToMenu = true;
            }
        }
    }
}

void Menu::showInstructions() {
    currentState = MenuState::INSTRUCTIONS;
}
