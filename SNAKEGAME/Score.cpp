#include "Score.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

Score::Score(SDL_Renderer* renderer) : renderer(renderer) {
    loadResources();
}

Score::~Score() {
    cleanupResources();
}

void Score::loadResources() {
    backgroundTexture = IMG_LoadTexture(renderer, "assets/images/imagen.png");
    font = TTF_OpenFont("assets/fonts/AngryBirdsPixela.ttf", 24);
}

void Score::cleanupResources() {
    SDL_DestroyTexture(backgroundTexture);
    TTF_CloseFont(font);
}

void Score::display() {
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

    SDL_Color white = {255, 255, 255, 255};
    int yPosition = 150;
    for (const auto &entry : scores) {
        std::string scoreText = entry.name + " - " + std::to_string(entry.score) + " pts - " + std::to_string(entry.time) + " sec";
        renderText(scoreText.c_str(), 250, yPosition, white);
        yPosition += 50;
    }

    SDL_RenderPresent(renderer);
}

void Score::addScore(const ScoreEntry &entry) {
    scores.push_back(entry);
    // TODO: Sort and keep the top scores only
}

void Score::renderText(const char* text, int x, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect destRect = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &destRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}


