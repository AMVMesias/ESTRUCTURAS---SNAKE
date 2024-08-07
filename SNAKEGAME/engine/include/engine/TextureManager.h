#pragma once

#include <map>
#include <string>

#include <SDL2/SDL_image.h>

class TextureManager {
public:
    TextureManager(SDL_Renderer* renderer);
    ~TextureManager();

    SDL_Texture* LoadTexture(const std::string& file_path);
    void RemoveTexture(const std::string& file_path);

private:
    SDL_Renderer* renderer_;
    std::map<std::string, SDL_Texture*> textures_;

    void ClearAllTextures();
};
