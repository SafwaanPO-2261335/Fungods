#include "Engine.h"
#include <iostream>


bool Engine::iterate() {

	if (m_running) {
		//m_moCap->process();
		m_levels.at(m_levelIndex)->live();
		// preload entities
		std::vector<Entity*> entities;
		for (Spore* s : m_spores)
			if (!s->isDead())
				entities.push_back(s);
		for (Bacterium* b : m_bacteria)
			if (!b->isDead())
				entities.push_back(b);
		for (Fungus* f : m_fungi)
			if (!f->isDead())
				entities.push_back(f);

		// calculate entities
		for (Entity* e : entities) {
			e->setColleagues(&entities);
			e->tick();
			e->live();
		}

		// calculate wind effect
		if (m_windActive) {
			for (Spore* s : m_spores)
				if (!s->isDead())
					s->blow(m_windLoc, m_shadowWindLoc);
			m_windActive = false;
		}
		if (m_tornadoActive) {
			for (Spore* s : m_spores)
				if (!s->isDead())
					s->suck(m_tornadoLoc);
			m_tornadoActive = false;
			for (Fungus* f : m_fungi)
				if (!f->isDead())
					f->suck(m_tornadoLoc);
		}

		// render
		m_spriteMaster.cls();
		m_spriteMaster.renderLevel(m_levels.at(m_levelIndex));
		for (Fungus* f : m_fungi)
			if (!f->isDead())
				m_spriteMaster.renderMycelium(f->getMycelium());
		for (Entity* e : entities)
			m_spriteMaster.renderEntity(e);
		m_spriteMaster.renderEntity(m_fungiSelect.at(m_fungiIndex));

		if (m_score < 0)
			m_spriteMaster.renderGui(m_charge_gui.at(0), GRID_WIDTH - 9, 9);
		else if (m_score > 8)
			m_spriteMaster.renderGui(m_charge_gui.at(8), GRID_WIDTH - 9, 9);
		else
			m_spriteMaster.renderGui(m_charge_gui.at(m_score), GRID_WIDTH - 9, 9);
	}
	else {
		// render
		m_spriteMaster.cls();
		m_spriteMaster.renderGui(m_regrow_bg, int(GRID_WIDTH / 2), int(GRID_HEIGHT / 2));
		
	}

	if (m_tapping)
		m_spriteMaster.renderGui(m_cursor_tap, m_cursor_x, m_cursor_y);
	else if (m_hovering)
		m_spriteMaster.renderGui(m_cursor_hover, m_cursor_x, m_cursor_y);
	m_tapping = false;
	m_hovering = false;

	m_spriteMaster.renderScreen();

	// constant framerate
	while (m_shadowTick + 1000 / FPS > SDL_GetTicks()) {}
	// std::cout << SDL_GetTicks() - m_shadowTick << "\n";
	m_shadowTick = SDL_GetTicks();
	
    return true; 
}

void Engine::shoot(bool track) {
	float x;
	float y;
	SDL_GetMouseState(&x, &y);
	x *= WG_WIDTH_FACTOR;
	y *= WG_HEIGHT_FACTOR;
	m_fungi.at(0)->attack(x, y);
}

void Engine::spawn() {
	float x;
	float y;
	SDL_GetMouseState(&x, &y);
	x *= WG_WIDTH_FACTOR;
	y *= WG_HEIGHT_FACTOR;
	spawn(x, y);
}

void Engine::spawn(float x, float y) {
	Fungus* f = selectFungus((fungusType)m_fungiIndex, x, y);
	spawnFungi(f);
}

bool Engine::init() {
	t = std::thread(&MoCap::process, m_moCap);

	pathFile = std::fstream(std::filesystem::current_path().parent_path() / "p.txt");
	bool init_succes = m_spriteMaster.init();

	for (int i = 0; i < LVL_COUNT; i++) {
		Level* l = new Level("0", std::to_string(i), m_userEvent);
		
		int index = m_spriteMaster.addLevel("0", std::to_string(i));
		l->setSpriteIndex(index);
		m_levels.push_back(l);
	}

	spawnFungi(selectFungus((fungusType)m_levelIndex, 128, 128));
	m_fungi.at(0)->setBoss(true);

	for (int i = 0; i < FUNGI_COUNT; i++)
		addSelect(selectFungus((fungusType)i, 9, 9));

	m_regrow_bg = m_spriteMaster.addGui("start", "regrow");
	m_cursor_hover = m_spriteMaster.addGui("cursor", "hover");
	m_cursor_tap = m_spriteMaster.addGui("cursor", "tap");

	for (int i = 0; i <= 8; i++) {
		m_charge_gui.push_back(m_spriteMaster.addGui("gui", "charge" + std::to_string(i)));
	}

	return init_succes;
}

void Engine::reset() {
	m_fungi.clear();
	m_bacteria.clear();
	m_spores.clear();

	m_score += 8;
	spawnFungi(selectFungus((fungusType)m_levelIndex, 128, 128));
	m_fungi.at(0)->setBoss(true);
}

void Engine::fullReset() {
	m_fungi.clear();
	m_bacteria.clear();
	m_spores.clear();
	m_levels.clear();
	m_levelIndex = 0;
	m_score = 16;

	m_windActive = false;
	m_tornadoActive = false;

	for (int i = 0; i < LVL_COUNT; i++) {
		Level* l = new Level("0", std::to_string(i), m_userEvent);

		int index = m_spriteMaster.addLevel("0", std::to_string(i));
		l->setSpriteIndex(index);
		m_levels.push_back(l);
	}

	spawnFungi(selectFungus((fungusType)m_levelIndex, 128, 128));
	m_fungi.at(0)->setBoss(true);

	m_running = false;
}

void Engine::handleEvent(SDL_Event* event) {
	if (m_running) {

		if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
			if (event->button.button == SDL_BUTTON_LEFT) {
				float x;
				float y;
				SDL_GetMouseState(&x, &y);
				x *= WG_WIDTH_FACTOR;
				y *= WG_HEIGHT_FACTOR;

				m_shadowWindLoc = Location(x, y);
			}
			else if (event->button.button == SDL_BUTTON_RIGHT)
				m_tornadoActive = true;
			else if (event->button.button == SDL_BUTTON_MIDDLE) {
				if (!m_levels.at(m_levelIndex)->relive()) {
					m_levelIndex++;
					if (m_levelIndex >= LVL_COUNT)
						fullReset();
					else
						reset();
				}
			}
		}
		else if (event->type == SDL_EVENT_MOUSE_BUTTON_UP) {
			if (event->button.button == SDL_BUTTON_LEFT) {
				float x;
				float y;
				SDL_GetMouseState(&x, &y);
				x *= WG_WIDTH_FACTOR;
				y *= WG_HEIGHT_FACTOR;
				m_windLoc = Location(x, y);
				m_windActive = true;
			}
			else if (event->button.button == SDL_BUTTON_RIGHT)
				m_tornadoActive = false;
		}
		else if (event->type == SDL_EVENT_KEY_DOWN) {
			if (event->key.key == SDLK_P) {
				float x;
				float y;
				SDL_GetMouseState(&x, &y);
				x *= WG_WIDTH_FACTOR;
				y *= WG_HEIGHT_FACTOR;
				pathFile << "L " << (int)x << ":" << (int)y << "\n";
			}
			else if (event->key.key == SDLK_LEFT) {
				m_fungiIndex++;
				if (m_fungiIndex >= FUNGI_COUNT)
					m_fungiIndex = 0;
			}
			else if (event->key.key == SDLK_RIGHT) {
				m_fungiIndex--;
				if (m_fungiIndex < 0)
					m_fungiIndex = int(FUNGI_COUNT) - 1;
			}
			else if (event->key.key == SDLK_DOWN) {
				spawn();
			}
		}
		else if (event->type == SDL_EVENT_USER) {
			if (event->user.code == S_SPORE) {
				Spore* s = (Spore*)event->user.data1;
				Location* l = (Location*)event->user.data2;
				spawnSpore(s, l);
			}
			else if (event->user.code == S_BACTERIA) {
				Bacterium* b = (Bacterium*)event->user.data1;
				spawnBacteria(b);
			}
			else if (event->user.code == ADD_SCORE) {
				m_score += *((int*)event->user.data1);
				if (m_score > 8)
					m_score = 8;
			}
			else if (event->user.code == MC_TAP) {
				int x = *(int*)event->user.data1;
				int y = *(int*)event->user.data2;
				m_tapping = true;
				m_cursor_x = x;
				m_cursor_y = y;

				if (x < 24 && y < 24) {
					m_fungiIndex--;
					if (m_fungiIndex < 0)
						m_fungiIndex = int(FUNGI_COUNT) - 1;
				}
				else if (Location(x, y).distance2(Location(GRID_WIDTH / 2, GRID_HEIGHT / 2)) < 64) {
					if (!m_levels.at(m_levelIndex)->relive()) {
						m_levelIndex++;
						if (m_levelIndex >= LVL_COUNT)
							fullReset();
						else
							reset();
					}
				}
				else if (x > 0 && x < GRID_WIDTH && y > 0 && y < GRID_HEIGHT)
					spawn(float(x), float(y));
			}
			else if (event->user.code == MC_HOVER) {
				int x = *(int*)event->user.data1;
				int y = *(int*)event->user.data2;
				m_hovering = true;
				m_cursor_x = x;
				m_cursor_y = y;

				m_tornadoActive = true;
				m_tornadoLoc.setX(x);
				m_tornadoLoc.setY(y);
				
			}
			else if (event->user.code == GAME_OVER) {
				fullReset();
			}
		}
	}
	else {
		if (event->type == SDL_EVENT_USER) {
			if (event->user.code == MC_TAP) {
				int x = *(int*)event->user.data1;
				int y = *(int*)event->user.data2;
				m_tapping = true;
				m_cursor_x = x;
				m_cursor_y = y;

				if (Location(x, y).distance2(Location(GRID_WIDTH / 2, GRID_HEIGHT / 2)) < 64) {
					m_running = true;
				}
			}
			else if (event->user.code == MC_HOVER) {
				int x = *(int*)event->user.data1;
				int y = *(int*)event->user.data2;
				m_hovering = true;
				m_cursor_x = x;
				m_cursor_y = y;
			}
		}
		else if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
			if (event->button.button == SDL_BUTTON_LEFT) {
				m_running = true;
			}
		}
	}
}

void Engine::addSelect(Fungus* f) {
	int w = 0;
	int h = 0;
	int index = m_spriteMaster.addEntity("Fungi", f->getName(), &w, &h);
	f->init(w, h, index, m_userEvent);
	m_fungiSelect.push_back(f);
}

void Engine::spawnFungi(Fungus* f) {
	int w = 0;
	int h = 0;
	int index = m_spriteMaster.addEntity("Fungi", f->getName(), &w, &h);
	f->init(w, h, index, m_userEvent);
	for (Fungus* f2 : m_fungi)
		if (f->collideRadius(f2))
			return;
	if (m_score >= 8) {
		m_fungi.push_back(f);
		m_score += 8;
	}
}
void Engine::spawnBacteria(Bacterium* b) {
	int w = 0;
	int h = 0;
	int index = m_spriteMaster.addEntity("Bacteria", b->getName(), &w, &h);
	b->init(w, h, index, m_userEvent);
	m_bacteria.push_back(b);
}
void Engine::spawnSpore(Spore* s) {
	int w = 0;
	int h = 0;
	int index = m_spriteMaster.addEntity("Spores", s->getName(), &w, &h);
	s->init(w, h, index);
	m_spores.push_back(s);
}
void Engine::spawnSpore(Spore* s, Location* target) {
	int w = 0;
	int h = 0;
	int index = m_spriteMaster.addEntity("Spores", s->getName(), &w, &h);
	s->init(w, h, index);
	s->aim(*target);
	m_spores.push_back(s);
}

void Engine::spawnBacteria(std::string name, float x, float y) {
	Bacterium* b = new Bacterium(name, x, y);
	spawnBacteria(b);
}

Fungus* Engine::selectFungus(fungusType nr, float x, float y) {
	switch (nr)
	{
	case AMANITA:
		return new Amanita(x, y);
	case CALVATIA:
		return new Hericium(x, y);
	case HERICIUM:
		return new Penicillium(x, y);
	case PENICILLIUM:
		return new Psilocybe(x, y);
	case PSILOCYBE:
		return new Tuber(x, y);
	case TUBER:
		return new Calvatia(x, y);
	default:
		return new Fungus("ERROR", x, y);
		break;
	}
}