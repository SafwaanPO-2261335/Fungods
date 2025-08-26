#pragma once

#include "PARAMS.h"

#include "Entities/Entity.h"
#include "Level.h"
#include <string>
#include <SDL3/SDL.h>
#include <vector>
#include "Sprite.h"
#include "Entities/Fungi/Mycelium.h"

class SpriteMaster
{
private:
	SDL_Window* m_window = NULL;
	SDL_Renderer* m_renderer = NULL;
	std::vector<Sprite*> m_sprites;
	std::vector<SDL_Texture*> m_mycelia;
public:
	bool init();
	int addEntity(std::string group, std::string name, int* w, int* h);
	int addLevel(std::string group, std::string name);
	int addGui(std::string group, std::string name);
	SDL_Renderer* getRenderer() { return m_renderer; }
	void cls();
	void renderEntity(Entity* e);
	void renderLevel(Level* l);
	void renderGui(int index, int x, int y);
	void renderMycelium(Mycelium* m);
	void renderScreen();
};

