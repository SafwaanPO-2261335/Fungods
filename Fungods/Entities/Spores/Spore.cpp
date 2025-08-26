#include "Spore.h"

void Spore::aim(Location loc) {
    double d = getLocation().distance(loc);

    if (d != 0) {
        double q = m_s / d;
        m_dx += (float)(q * (loc.getX() - m_x));
        m_dy += (float)(q * (loc.getY() - m_y));
    }
}

void Spore::live() {
    if (m_dead)
        return;
    if (--m_tick < 0 && m_maxTicks != -1) {
        die();
        return;
    }
    if (m_tracking)
    {
        Entity* target = getTarget();
        if (target != this) {
            if (collideRadius(target)) {
                target->hurt(m_damage);
                die();
            }

            track(target->getLocation());
        }
        move();
    }
    else {
        for (Entity* e : *m_colleagues)
        {
            if (e->getTeam() == this->m_team)
                continue;
            if (collideRadius(e)) {
                e->hurt(m_damage);
                die();
            }
        } 
        move();
    }
}

void Spore::setStat(std::string key, std::string val) {
    __super::setStat(key, val);
    if (key == "T")
        m_tracking = stoi(val);
    else if (key == "L") {
        m_maxTicks = stoi(val);
        m_tick = m_maxTicks;
    }
}

void Spore::suck(Location loc) {
    float minD = GRID_WIDTH;
    float dxLoc = m_loc.getX() - loc.getX();
    float dyLoc = m_loc.getY() - loc.getY();
    
    float dLoc = m_loc.distance(loc);

    if (dLoc >= minD)
        return;

    float factor = (minD - dLoc)/minD;

    if (dLoc != 0) {
        double q = (6 / (dLoc)) * factor;
        m_dx -= (float)(q * dxLoc);
        m_dy -= (float)(q * dyLoc);
    }
}

void Spore::blow(Location loc, Location shadowloc) {
    float minD = 64;
    float dxLoc = m_loc.getX() - loc.getX();
    float dyLoc = m_loc.getY() - loc.getY();

    float dxCross = shadowloc.getX() - loc.getX();
    float dyCross = shadowloc.getY() - loc.getY();

    float dLoc = m_loc.distance(loc);
    float dShLoc = m_loc.distance(shadowloc);
    float dCross = loc.distance(shadowloc);


    /*
    if (!dxLoc)
        dxLoc = 0.01;

    if (!dxCross)
        dxCross = 0.01;

    if (!dyLoc)
        dyLoc = 0.01;

    if (!dyCross)
        dyCross = 0.01;


    float aLoc = atan(dyLoc / dxLoc);
    float aCross = atan(dyCross / dxCross);

    float a = aCross - aLoc;

    float d = abs(sin(a) * dLoc);

    if (d >= minD)
        return;

    if (dLoc > dCross)
        if (dShLoc > minD)
            return;

    if (dShLoc > dCross)
        return;

    */
    float d = dShLoc;

    if (d >= minD)
        return;



    //m_dx += dx;
    //m_dy += dy;

    float factor = dCross / 32;
    if (factor > 1)
        factor = 1;

    factor *= (minD - d) / minD;

    if (dCross != 0) {
        double q = (20 / (dLoc)) * factor;
        m_dx -= (float)(q * dxLoc);
        m_dy -= (float)(q * dyLoc);
    }
}