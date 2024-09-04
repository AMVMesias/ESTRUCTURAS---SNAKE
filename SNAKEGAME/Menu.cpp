#include "Menu.h"
#include <SDL2/SDL_image.h>
#include "Game.h"
#include "Score.h"

Menu::Menu(SDL_Renderer* renderer)
    : renderer(renderer), selectedOption(0), currentState(MenuState::MAIN), selectedDifficulty(0) {
    backgroundTexture = IMG_LoadTexture(renderer, "assets/images/imagen.png");
    instructionsTexture = IMG_LoadTexture(renderer, "assets/images/instrucciones.png");
    font = TTF_OpenFont("assets/fonts/fuente.ttf", 30);
    menuMusic = Mix_LoadMUS("assets/music/musica.mp3");
}

Menu::~Menu() {
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(instructionsTexture);
    TTF_CloseFont(font);
    Mix_FreeMusic(menuMusic);
}

void Menu::playMusic() {
    if (Mix_PlayingMusic() == 0) {
        Mix_PlayMusic(menuMusic, -1);
    } else if (Mix_PausedMusic() == 1) {
        Mix_ResumeMusic();
    }
}

void Menu::stopMusic() {
    Mix_HaltMusic();
}

void Menu::show() {
    currentState = MenuState::MAIN;
    playMusic();
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
                                stopMusic();
                                Game game(renderer, this);
                                game.iniciar(selectedDifficulty + 1);
                                playMusic();
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
    SDL_Color yellow = {255, 255, 0, 255};  // Changed to actual yellow
    SDL_Color shadow = {0, 0, 0, 255};  // Black color for shadow

    renderTextWithShadow("Snake Game", SCREEN_WIDTH / 2, 100, green, shadow, true);

    if (currentState == MenuState::MAIN) {
        SDL_Color shadowColor = {0, 0, 0, 255};  // Black shadow
        renderTextWithShadow("Iniciar Juego", SCREEN_WIDTH / 2, 250, selectedOption == 0 ? green : white, shadowColor, true);
        renderTextWithShadow("Cómo Jugar", SCREEN_WIDTH / 2, 300, selectedOption == 1 ? green : white, shadowColor, true);
        renderTextWithShadow("Score", SCREEN_WIDTH / 2, 350, selectedOption == 2 ? green : white, shadowColor, true);
        renderTextWithShadow("Salir", SCREEN_WIDTH / 2, 400, selectedOption == 3 ? green : white, shadowColor, true);
    } else if (currentState == MenuState::DIFFICULTY) {
        SDL_Color shadowColor = {0, 0, 0, 255};  // Black shadow
        renderTextWithShadow("Selecciona la dificultad:", SCREEN_WIDTH / 2, 200, white, shadowColor, true);
        renderTextWithShadow("Fácil", SCREEN_WIDTH / 2, 300, selectedDifficulty == 0 ? green : white, shadowColor, true);
        renderTextWithShadow("Medio", SCREEN_WIDTH / 2, 400, selectedDifficulty == 1 ? green : white, shadowColor, true);
        renderTextWithShadow("Difícil", SCREEN_WIDTH / 2, 500, selectedDifficulty == 2 ? green : white, shadowColor, true);
    } else if (currentState == MenuState::INSTRUCTIONS) {
        TTF_CloseFont(font);
        SDL_Rect destRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, instructionsTexture, NULL, &destRect);
        font = TTF_OpenFont("assets/fonts/fuente.ttf", 20);

        // Render text with shadow
        renderTextWithShadow("Instrucciones:", SCREEN_WIDTH / 2, 150, yellow, shadow, true);
        renderTextWithShadow("Flechas de dirección: Mover la serpiente", SCREEN_WIDTH / 2, 250, yellow, shadow, true);
        renderTextWithShadow("WASD: Mover la serpiente", SCREEN_WIDTH / 2, 300, yellow, shadow, true);
        renderTextWithShadow("Enter: Seleccionar opción", SCREEN_WIDTH / 2, 350, yellow, shadow, true);
        renderTextWithShadow("Esc: Retroceder / Pausar", SCREEN_WIDTH / 2, 400, yellow, shadow, true);
        renderTextWithShadow("Presiona Esc para volver al menú", SCREEN_WIDTH / 2, 500, yellow, shadow, true);

        TTF_CloseFont(font);
        font = TTF_OpenFont("assets/fonts/fuente.ttf", 30);
    }

    SDL_RenderPresent(renderer);
}

void Menu::renderTextWithShadow(const char* text, int x, int y, SDL_Color color, SDL_Color shadowColor, bool center) {
    // Render shadow
    SDL_Surface* shadowSurface = TTF_RenderText_Solid(font, text, shadowColor);
    SDL_Texture* shadowTexture = SDL_CreateTextureFromSurface(renderer, shadowSurface);
    SDL_Rect shadowRect;
    if (center) {
        shadowRect = { x - (shadowSurface->w / 2) + 2, y + 2, shadowSurface->w, shadowSurface->h };
    } else {
        shadowRect = { x + 2, y + 2, shadowSurface->w, shadowSurface->h };
    }
    SDL_RenderCopy(renderer, shadowTexture, NULL, &shadowRect);
    SDL_FreeSurface(shadowSurface);
    SDL_DestroyTexture(shadowTexture);

    // Render main text
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect;
    if (center) {
        textRect = { x - (textSurface->w / 2), y, textSurface->w, textSurface->h };
    } else {
        textRect = { x, y, textSurface->w, textSurface->h };
    }
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
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
