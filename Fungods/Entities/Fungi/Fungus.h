#pragma once

#include "../Entity.h"
#include "../Spores/Spore.h"
#include "../../EVENTS.h"
#include "Mycelium.h"
#include <vector>
#include <random>

class Fungus : public Entity {
protected:
	bool m_boss = false;
	void setStat(std::string key, std::string val) override;
	Spore* m_spore = nullptr;
	Mycelium* m_mycelium;
	void postLive();
	void die() override;
public:
	Fungus(std::string name, float x, float y) : Entity("Fungi", name, x, y) { m_mycelium = new Mycelium(int(x), int(y)); }
	void setBoss(bool c) { m_boss = c; m_mycelium->setLife(-1); }
	virtual void attack(Location l) { attack(l.getX(), l.getY()); }
	virtual void attack(float x, float y);
	void live() override;
	void spawnSpore(Spore* s, Location* target);
	void upgrade(std::string name);
	Mycelium* getMycelium() { return m_mycelium; };
	void suck(Location loc);
};
