#pragma once

#include <SDL3/SDL.h>
#include <filesystem>
#include <vector>
#include <string>
#include <random>

class Sprite
{ 
private:
	bool m_valid = true;
	SDL_Surface* m_surface = NULL;
	SDL_Texture* m_texture = NULL;
	std::string m_group;
	std::string m_name;
public:
	Sprite(SDL_Renderer* renderer, std::string type, std::string group, std::string name);
	bool getValid() { return m_valid; }
	void render(SDL_Renderer* renderer, SDL_FRect* rect);
	void renderEnlightened(SDL_Renderer* renderer, SDL_FRect* rect);
	bool equals(std::string group, std::string name);
	void getDimentions(int* w, int* h);
};
