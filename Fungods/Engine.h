#pragma once

#include "PARAMS.h"

#include "Entities/Fungi/Fungus.h"
#include "Entities/Bacteria/Bacterium.h"
#include "Entities/Spores/Spore.h"
#include "Location.h"
#include <vector>
#include "SpriteMaster.h"
#include "Level.h"
#include "EVENTS.h"
#include <filesystem>
#include <fstream>
#include "MoCap.h"
#include <thread>
#include "Entities/Fungi/Mycelium.h"

#include "Entities/Fungi/Amanita/Amanita.h"
#include "Entities/Fungi/Calvatia/Calvatia.h"
#include "Entities/Fungi/Hericium/Hericium.h"
#include "Entities/Fungi/Penicillium/Penicillium.h"
#include "Entities/Fungi/Psilocybe/Psilocybe.h"
#include "Entities/Fungi/Tuber/Tuber.h"

class Engine
{
private:
	enum fungusType {
		AMANITA,
		CALVATIA,
		HERICIUM,
		PENICILLIUM,
		PSILOCYBE,
		TUBER,
		FUNGI_COUNT
	};
	
	std::fstream pathFile;
	Uint32 m_userEvent = SDL_RegisterEvents(1);
	SpriteMaster m_spriteMaster;

	int m_score = 16;

	bool m_running = false;
	int m_regrow_bg;

	std::vector<int> m_charge_gui;

	bool m_hovering;
	bool m_tapping;
	int m_cursor_x;
	int m_cursor_y;
	int m_cursor_hover;
	int m_cursor_tap;
	
	std::vector<Fungus*> m_fungi;
	std::vector<Bacterium*> m_bacteria;
	std::vector<Spore*> m_spores;
	std::vector<Mycelium*> m_mycelia;

	std::vector<Fungus*> m_fungiSelect;
	int m_fungiIndex = 0;

	std::vector<Level*> m_levels;
	int m_levelIndex = 2;
	
	Uint64 m_shadowTick = SDL_GetTicks();

	bool m_windActive = false;
	bool m_tornadoActive = false;
	Location m_tornadoLoc = Location(0, 0);
	Location m_windLoc = Location(0, 0);
	Location m_shadowWindLoc = Location(0, 0);

	MoCap* m_moCap = new MoCap(m_userEvent);
	std::thread t;

	void fullReset();
	

public:
	~Engine() { pathFile.close(); t.join(); }
	bool iterate();
	void shoot(bool track);
	void spawn();
	void spawn(float x, float y);
	bool init();
	void reset();
	void handleEvent(SDL_Event* event);
	void addSelect(Fungus* f);
	void spawnFungi(Fungus* f);
	void spawnBacteria(Bacterium* b);
	void spawnBacteria(std::string name, float x, float y);
	void spawnSpore(Spore* s);
	void spawnSpore(Spore* s, Location* target);
	Fungus* selectFungus(fungusType nr, float x, float y);
};
