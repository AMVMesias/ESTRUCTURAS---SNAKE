#include "TextureManager.h"

#include <stdexcept>

TextureManager::TextureManager(SDL_Renderer* renderer) 
    : renderer_(renderer) {}

TextureManager::~TextureManager() {
    ClearAllTextures();
}

SDL_Texture* TextureManager::LoadTexture(const std::string& file_path) {
    if (textures_.count(file_path) == 0) {
        SDL_Texture* texture = IMG_LoadTexture(renderer_, file_path.c_str());
        if (!texture) {
            throw std::runtime_error("Failed to load texture: " + file_path);
        }
        textures_[file_path] = texture;
    }
    return textures_[file_path];
}

void TextureManager::RemoveTexture(const std::string& file_path) {
    auto it = textures_.find(file_path);
    if (it != textures_.end()) {
        SDL_DestroyTexture(it->second);
        textures_.erase(it);
    }
}

void TextureManager::ClearAllTextures() {
    for (auto& texture_pair : textures_) {
        SDL_DestroyTexture(texture_pair.second);
    }
    textures_.clear();
}