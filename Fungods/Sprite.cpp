#include "Sprite.h"
#include <iostream>

Sprite::Sprite(SDL_Renderer* renderer, std::string type, std::string group, std::string name) : m_group(group), m_name(name) {
    std::string s = (std::filesystem::current_path().parent_path() / type / group / name / "S0.bmp").string();
    m_surface = SDL_LoadBMP(s.c_str());
    if (!m_surface) {
        m_valid = false;
    }

    m_texture = SDL_CreateTextureFromSurface(renderer, m_surface);
    if (!m_texture) {
        m_valid = false;
    }


    SDL_SetTextureScaleMode(m_texture, SDL_SCALEMODE_NEAREST);
}

void Sprite::render(SDL_Renderer* renderer, SDL_FRect* rect) {
    SDL_RenderTexture(renderer, m_texture, NULL, rect);
}

void Sprite::renderEnlightened(SDL_Renderer* renderer, SDL_FRect* rect) {
    SDL_Surface* surface(m_surface);
    for (int x = 0; x < surface->w; x++) {
        for (int y = 0; y < surface->h; y++) {
            Uint32* const target_pixel = (Uint32*)((Uint8*)surface->pixels + y * surface->pitch + x * 4);
            Uint32 a = *target_pixel & 0b11111111000000000000000000000000;
            if (a > 0 && *target_pixel != a){
                Uint32 color = ((std::rand() % 256) << 16) + ((std::rand() % 256) << 8) + ((std::rand() % 256));
                *target_pixel = color + a;
            }
            
        }
    }
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

    SDL_RenderTexture(renderer, texture, NULL, rect);
}

bool Sprite::equals(std::string group, std::string name) {
    return m_group == group && m_name == name;
}

void Sprite::getDimentions(int* w, int* h) {
    *w = m_texture->w;
    *h = m_texture->h;
}