#pragma once

#include "../Entity.h"
#include "../../PARAMS.h"

class Spore : public Entity { 
private:
    bool m_tracking = false;
    int m_maxTicks = -1;
    int m_tick = m_maxTicks;
    void setStat(std::string key, std::string val) override;
public:
    Spore(std::string name, float x = 0, float y = 0) : Entity("Spores", name, x, y) { }
    void aim(Location loc);
    void live() override;
    void suck(Location loc);
    void blow(Location loc, Location shadowloc);
};
