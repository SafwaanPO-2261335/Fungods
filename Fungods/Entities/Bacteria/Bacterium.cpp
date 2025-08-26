#include "Bacterium.h"


Bacterium::Bacterium(std::string name, Path* path)
    : Entity("Bacteria", name, 0, 0, 1), m_path(path) {
    m_x = m_path->getLoc(0).getX() - m_w + (std::rand() % ((int)m_w * 2));
    m_y = m_path->getLoc(0).getY() - m_h + (std::rand() % ((int)m_h * 2));
    update();
}


void Bacterium::live() {
    if (!m_dead) {
        if (getEnlightened()) {
            // val uw eigen vrienden aan
            Entity* target = this;
            float minD = -1;
            for (Entity* e : *m_colleagues)
            {
                if (e == this || e->getTeam() != this->m_team)
                    continue;

                float d = this->getLocation().distance(e->getLocation());
                if (d < minD || minD < 0) {
                    minD = d;
                    target = e;
                }
            }
            if (target == this)
                die();
            else {
                if (collideRadius(target)) {
                    target->hurt(m_damage);
                }

                track(target->getLocation());
            }
            move();
        }
        else if (m_pathIndex < m_path->getSize()) {
            for (Entity* e : *m_colleagues) {
                if (e->getTeam() == this->m_team || e->getgroup() == "Spores")
                    continue;
                if (collideRadius(e)) {
                    e->hurt(m_damage);
                    hurt(1);
                }
                    
            }

            Location loc = m_path->getLoc(m_pathIndex);
            track(loc);
            if (pow(loc.getX() - m_x, 2) + pow(loc.getY() - m_y, 2) < m_w * m_h + m_s * m_s)
                m_pathIndex++;
            move();
        }
        else {
            Entity* target = getTarget();
            if (target != this) {
                if (collideRadius(target)) {
                    target->hurt(m_damage);
                    hurt(1);
                }
                track(target->getLocation());
            }
            move();
        }
    }
}

void Bacterium::die() {
    __super::die();

    if (m_event == NULL)
        return;
    SDL_Event event;
    SDL_zero(event);
    event.type = m_event;
    event.user.code = ADD_SCORE;
    event.user.data1 = &m_score;
    SDL_PushEvent(&event);
}

void Bacterium::setStat(std::string key, std::string val) {
    __super::setStat(key, val);
    if (key == "P")
        m_score = stoi(val);
}