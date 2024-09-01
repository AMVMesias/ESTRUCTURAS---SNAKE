#include "Score.h"
#include "Menu.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <fstream>
#include <algorithm>

Score::Score(SDL_Renderer* renderer) : renderer(renderer) {
    loadResources();
    loadScoresFromJson();
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
    renderText("High Scores", SCREEN_WIDTH / 2 - 100, 50, white);

    int yPosition = 150;
    for (const auto &entry : scores) {
        std::string scoreText = entry.name + " - " + std::to_string(entry.score) + " pts - " + std::to_string(entry.time) + " sec";
        renderText(scoreText.c_str(), SCREEN_WIDTH / 2 - 200, yPosition, white);
        yPosition += 50;
    }

    SDL_RenderPresent(renderer);
}

void Score::addScore(const ScoreEntry &entry) {
    scores.push_back(entry);
    std::sort(scores.begin(), scores.end(), [](const ScoreEntry& a, const ScoreEntry& b) {
        return a.score > b.score;
    });
    if (scores.size() > 10) {
        scores.resize(10);
    }
    saveScoresToJson();
}

void Score::renderText(const char* text, int x, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect destRect = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &destRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void Score::loadScoresFromJson() {
    std::ifstream file("scores.json");
    if (file.is_open()) {
        nlohmann::json j;
        file >> j;
        file.close();

        scores.clear();
        for (const auto& score : j["scores"]) {
            scores.push_back({
                score["name"],
                score["score"],
                score["time"]
            });
        }

        std::sort(scores.begin(), scores.end(), [](const ScoreEntry& a, const ScoreEntry& b) {
            return a.score > b.score;
        });

        if (scores.size() > 10) {
            scores.resize(10);
        }
    }
}

void Score::saveScoresToJson() {
    nlohmann::json j;
    j["scores"] = nlohmann::json::array();

    for (const auto& score : scores) {
        nlohmann::json scoreJson;
        scoreJson["name"] = score.name;
        scoreJson["score"] = score.score;
        scoreJson["time"] = score.time;
        j["scores"].push_back(scoreJson);
    }

    std::ofstream file("scores.json");
    file << j.dump(4);
    file.close();
}
