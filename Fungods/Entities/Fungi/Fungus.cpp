#include "Fungus.h"

void Fungus::attack(float x, float y) {
	if (m_spore == nullptr)
		return;
	Spore* s = new Spore(*m_spore);
	// todo: maybe spawn in exact circle
	s->setX(m_x + rand() % (int)(m_w / 2) - (int)(m_w / 4));
	s->setY(m_y + rand() % (int)(m_h / 2) - (int)(m_h / 4));
	s->update();
	spawnSpore(s, new Location(x, y));
}

void Fungus::live() {
	m_mycelium->live();
	for (Entity* e : *m_colleagues)
	{
		if (e->getTeam() == this->m_team)
			continue;
		if (m_mycelium->checkSensors(e->getLocation(), e->getR2())) {
			if (reloaded())
				attack(e->getLocation());
		}	
	}
	postLive();
}

void Fungus::postLive() {
	if (m_mycelium->isDead())
		this->die();
	update();
}

void Fungus::spawnSpore(Spore* s, Location* target) {
	if (m_event == NULL)
		return;
	SDL_Event event;
	SDL_zero(event);
	event.type = m_event;
	event.user.code = S_SPORE;
	event.user.data1 = s;
	event.user.data2 = target;
	SDL_PushEvent(&event);
}

void Fungus::setStat(std::string key, std::string val) {
	__super::setStat(key, val);
	if (key == "SPORE")
		m_spore = new Spore(val);
	else if (key == "M_SIZE")
		m_mycelium->setSize(stoi(val));
	else if (key == "M_BRANCH")
		m_mycelium->setBranch(stoi(val));
	else if (key == "M_TURN")
		m_mycelium->setTurn(stoi(val));
	else if (key == "M_COLOR") {
		size_t space0 = val.find(':');
		size_t space1 = val.substr(space0 + 1, val.size() - space0).find(':');

		int r = stoi(val.substr(0, space0));
		int g = stoi(val.substr(space0 + 1, space1));
		int b = stoi(val.substr(space0 + space1 + 2, val.size() - space1 - space0));
		m_mycelium->setColor(r, g, b);
	}	
	else if (key == "LIFE")
		m_mycelium->setLife(stoi(val));

}

void Fungus::upgrade(std::string name) {
	m_name = name;
	configStats();
}

void Fungus::suck(Location loc) {
	float d = loc.distance(m_loc);
	if (d < 24)
		if (reloaded())
			attack(loc.getX(), loc.getY());
}

void Fungus::die() {
	__super::die();
	if (m_boss) {
		if (m_event == NULL)
			return;
		SDL_Event event;
		SDL_zero(event);
		event.type = m_event;
		event.user.code = GAME_OVER;
		SDL_PushEvent(&event);
	}
}