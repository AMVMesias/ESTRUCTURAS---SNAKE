#ifndef SCORE_H
#define SCORE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>
#include "nlohmann/json.hpp"



struct ScoreEntry {
    std::string name;
    int score;
    int time;
};

class Score {
public:
    Score(SDL_Renderer* renderer);
    ~Score();
    void display();
    void addScore(const ScoreEntry &entry);
    void loadScoresFromJson();
    void renderText(const char* text, int x, int y, SDL_Color color);
    void saveScoresToJson();

private:
    SDL_Renderer* renderer;
    SDL_Texture* backgroundTexture;
    TTF_Font* font;
    std::vector<ScoreEntry> scores;
    void loadResources();
    void cleanupResources();
};

#endif // SCORE_H
