#include "Mycelium.h"

Mycelium::Mycelium(int x, int y) {
	m_root = new Hypha(x, y);
	m_active.push_back(m_root);

	Uint32* const target_pixel = (Uint32*)((Uint8*)m_surface->pixels + m_root->getY() * m_surface->pitch + m_root->getX() * 4);
	*target_pixel = 0b11111111111111111111111111111111;
	//                RRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA
	
}

void Mycelium::grow_step() {
	m_current_size++;
	std::vector<Hypha*> new_hyphae;
	m_sensors.clear();
	for (Hypha* h : m_active) {
		// grow and save for next tick
		std::vector<Hypha*> grown = h->grow(m_branch, m_turn);
		new_hyphae.insert(new_hyphae.end(), grown.begin(), grown.end());
		// draw
		Uint32* const target_pixel = (Uint32*)((Uint8*)m_surface->pixels + h->getY() * m_surface->pitch + h->getX() * 4);
		*target_pixel = *target_pixel | m_color;
		Uint32 a = *target_pixel & 0b00000000000000000000000011111111;
		if (a == 0)
			*target_pixel += 32;
		else if (a < 128)
			*target_pixel += 32;
	}
	m_active = new_hyphae;
	m_changed = true;

	m_sensors = new_hyphae;
}

void Mycelium::shrink_step() {

	m_current_size--;
	std::vector<Hypha*> old_hyphae;
	m_sensors.clear();
	for (Hypha* h : m_active) {
		// grow and save for next tick
		Hypha* pre = h->getPre();
		bool dupe = false;
		for (Hypha* o : old_hyphae)
			if (pre == o)
				dupe = true;
		if (!dupe)
			old_hyphae.push_back(pre);
		// draw
		Uint32* const target_pixel = (Uint32*)((Uint8*)m_surface->pixels + h->getY() * m_surface->pitch + h->getX() * 4);
		Uint32 a = *target_pixel & 0b00000000000000000000000011111111;
		if (a > 0)
			*target_pixel -= 32;
	}
	m_active = old_hyphae;
	m_changed = true;

	m_sensors = old_hyphae;
}

bool Mycelium::grow() {
	
	if (m_current_size < m_size) {
		grow_step();
		return true;
	}
	
	return false;
}

bool Mycelium::shrink() {
	if (m_current_size > 0) {
		if (m_life == -1)
			return false;
		m_life--;
		if (m_life <= 0) {
			m_life = 0;
			shrink_step();
		}
		return true;
	}
	return false;
}

void Mycelium::live() {
	if (m_growing)
		m_growing = grow();
	else
		shrink();
}

SDL_Texture* Mycelium::getTexture(SDL_Renderer* r) {
	// only makes new texture when needed
	if (m_changed) {
		m_texture = SDL_CreateTextureFromSurface(r, m_surface);
		SDL_SetTextureScaleMode(m_texture, SDL_SCALEMODE_NEAREST);
		m_changed = false;
	}
	/*
	// debug sensors
	else { 
		for (Hypha* h : m_sensors) {

			Uint32* const target_pixel = (Uint32*)((Uint8*)m_surface->pixels + h->getY() * m_surface->pitch + h->getX() * 4);
			*target_pixel = 0b11111111111111111111111111111111;
		}
		std::cout << m_sensors.size() << "\n";
		m_texture = SDL_CreateTextureFromSurface(r, m_surface);
		SDL_SetTextureScaleMode(m_texture, SDL_SCALEMODE_NEAREST);
	}
	*/
	return m_texture;
}

void Mycelium::setColor(int r, int g, int b) {
	m_color = (r << 24) + (g << 16) + (b << 8);
}

bool Mycelium::checkSensors(Location l, float r2) {
	for (Hypha* h : m_sensors) {
		if (h->getDistance2(l) <= r2)
			return true;
	}
		
	return false;
}