#pragma once

#include "Hypha.h"
#include "../../Thing.h"
#include <vector>
#include <SDL3/SDL.h>
#include "../../PARAMS.h"
#include <iostream>

class Mycelium : public Thing
{
private:
	Hypha* m_root = nullptr;
	std::vector<Hypha*> m_active;
	std::vector<Hypha*> m_sensors;
	SDL_Surface* m_surface = SDL_CreateSurface(GRID_WIDTH, GRID_HEIGHT, SDL_PIXELFORMAT_RGBA8888);
	SDL_Texture* m_texture = nullptr;
	SDL_FRect* m_rect = new SDL_FRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	bool m_changed = false;
	int m_current_size = 0;
	int m_size = 0;
	int m_branch = 9999;
	int m_turn = 9999;
	int m_life = 192; // leef 8 sec
	bool m_growing = true;
	Uint32 m_color = 0;
	void grow_step();
	void shrink_step();
public:
	Mycelium(int x, int y);
	bool grow();
	bool shrink();
	SDL_Surface* getSurface() { return m_surface; }
	SDL_Texture* getTexture(SDL_Renderer* r);
	SDL_FRect* getRect() { return m_rect; }
	void live() override;
	void setSize(int s) { m_size = s; }
	void setBranch(int b) { m_branch = b; }
	void setTurn(int t) { m_turn = t; }
	void setColor(int r, int g, int b);
	void setLife(int l) { m_life = l; }
	bool checkSensors(Location l, float r2);
	bool isDead() { return m_current_size <= 0 && !m_growing; }
};
