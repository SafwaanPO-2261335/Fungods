#include "Entity.h"

Entity::Entity(std::string group, std::string name, float x, float y, int team) : m_group(group), m_name(name), m_x(x), m_y(y), m_team(team)
{ 
}

void Entity::init(int w, int h, int spriteIndex, Uint32 event) { 
    m_event = event;
    m_w = float(w);
    m_h = float(h);
    configStats(); 
    setSpriteIndex(spriteIndex);
}

bool Entity::collidecorners(SDL_FRect* r0, SDL_FRect* r1)
{
    for (int i = 0; i <= 1; i++)
        for (int j = 0; j <= 1; j++)
        {
            float x = r0->x + i * r0->w;
            float y = r0->y + j * r0->h;
            if (r1->x <= x and x <= r1->x + r1->w and r1->y <= y and y <= r1->y + r1->h)
                return true;
        }
    return false;
}

void Entity::update() {
    m_rect.x = m_x - m_w / 2;
    m_rect.y = m_y - m_h / 2;
    m_rect.w = m_w;
    m_rect.h = m_h;
    m_loc.setX(m_x);
    m_loc.setY(m_y);
}

bool Entity::collideRect(Entity* to_check) {
    SDL_FRect* r = to_check->getRect();

    return collidecorners(&m_rect, r) || collidecorners(r, &m_rect);
}

bool Entity::collideRadius(Entity* to_check) {
    return to_check->getLocation().distance(m_loc) < m_r + to_check->getR();
}

void Entity::move() {
    float d = Location(0, 0).distance(Location(m_dx, m_dy));
    float f = getSpeedFactor();

    // deceleration
    if (d != 0) {
        if (d <= (m_da) && d >= -(m_da)) {
            m_dx = 0;
            m_dy = 0;
        }
        else {
            double q = (d - m_da) / d;
            m_dx = (float)(q * m_dx);
            m_dy = (float)(q * m_dy);
        }
    }

    m_shadowX = m_x;
    m_shadowY = m_y;

    m_x += m_dx * f;
    m_y += m_dy * f;

    update();
}

void Entity::track(Location target) {
    
    double d = getLocation().distance(target);
    
    if (d != 0) {
        double q = m_a / d;
        m_dx += (float)(q * (target.getX() - m_x));
        m_dy += (float)(q * (target.getY() - m_y));
    }

    // too fast
    float s = Location(0, 0).distance(Location(m_dx, m_dy));
    if (s != 0 && s > m_s) {
        double q = m_s / s;
        m_dx = (float)(q * m_dx);
        m_dy = (float)(q * m_dy);
    }
}

bool Entity::reloaded() {
    if (m_aTick < 0) {
        m_aTick = m_delay;
        return true;
    }
    return false;
}

Entity* Entity::getTarget() {
    Entity* target = this;
    float minD = -1;
    for (Entity* e : *m_colleagues)
    {
        if (e->getTeam() == this->m_team)
            continue;

        float d = this->getLocation().distance(e->getLocation());
        if (d < minD || minD < 0) {
            minD = d;
            target = e;
        }
    }
    return target;
}

void Entity::configStats() {
    std::ifstream statFile(std::filesystem::current_path().parent_path() / "Entities" / m_group / m_name / "stats.fge");

    std::string buffer;
    size_t space;
    std::string key;
    std::string val;
    while (getline(statFile, buffer)) {
        space = buffer.find(' ');
        key = buffer.substr(0, space);
        val = buffer.substr(space + 1, buffer.size() - space);

        setStat(key, val);
    }

    statFile.close();

    if (m_r == 0)
        m_r = (m_w + m_h) / 4;
    m_r2 = m_r * m_r;

    m_health = m_healthMax;

    update();
}

void Entity::setStat(std::string key, std::string val) {
    if (key == "HP")
        m_healthMax = stoi(val);
    if (key == "DMG")
        m_damage = stoi(val);
    else if (key == "W")
        m_w = stof(val);
    else if (key == "H")
        m_h = stof(val);
    else if (key == "S")
        m_s = stof(val);
    else if (key == "A")
        m_a = stof(val);
    else if (key == "DA")
        m_da = stof(val);
    else if (key == "R")
        m_r = stof(val);
    else if (key == "D")
        m_delay = stoi(val);
}

void Entity::hurt(int damage) {
    m_health -= damage;
    m_dx = 0;
    m_dy = 0;
    if (m_health <= 0)
        die();
}

void Entity::die() {
    m_dead = true;
    m_x = -666;
    m_y = -666;
    update();
}

float Entity::getSpeedFactor() {
    if (m_sTick < 0)
        m_sFactor = 1.0f;
    return m_sFactor;
}

void Entity::slow(float factor, int ticks) {
    m_sFactor = factor;
    m_sTick = ticks;
}

float Entity::getEnlightened() {
    if (m_eTick == 1)
        die();
    return m_enlightened;
}

void Entity::enlighten(int ticks) {
    m_enlightened = true;
    m_eTick = ticks;
}