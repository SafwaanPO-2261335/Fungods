#pragma once

#include "Entities/Bacteria/Bacterium.h"
#include "Path.h"
#include "PARAMS.h"
#include "EVENTS.h"
#include <vector>
#include <string>

class Wave
{
private:
	std::vector<Bacterium*> m_bacteria;
	std::vector<int> m_delays;
	int m_tick = 0;
	int m_tempDelay = 0;
	Uint32 m_userEvent;
	int m_index = 0;
	void transmitBacterium(Bacterium* b);
public:
	Wave(Uint32 event);
	void addDelay(int delay);
	void addBacteria(std::string name, Path* path);
	void live();
	bool isDead();
};
