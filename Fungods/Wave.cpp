#include "Wave.h"


Wave::Wave(Uint32 event) : m_userEvent(event) {
    
}

void Wave::addDelay(int delay) {
    m_tempDelay = delay;
}
void Wave::addBacteria(std::string name, Path* path) {
    float x = float(rand() % WINDOW_WIDTH);
    float y = float(rand() % WINDOW_HEIGHT);
    int edge = rand() % 4;
    float m_w = 0;
    float m_h = 0;
    switch (edge) {
    case 0:
        x = -m_w;
        break;
    case 1:
        x = WINDOW_WIDTH + m_w;
        break;
    case 2:
        y = -m_h;
        break;
    case 3:
        y = WINDOW_HEIGHT + m_h;
        break;
    }
    Bacterium* b = new Bacterium(name, path);
    m_bacteria.push_back(b);
    m_delays.push_back(m_tempDelay);
    m_tempDelay = 0;
}

void Wave::live() {
    if (m_index >= m_bacteria.size())
        return;

    if (++m_tick >= m_delays.at(m_index)) {
        transmitBacterium(m_bacteria.at(m_index));
        m_tick = 0;
        m_index++;
    }
}

void Wave::transmitBacterium(Bacterium* b) {
	SDL_Event event;
	SDL_zero(event);
	event.type = m_userEvent;
	event.user.code = S_BACTERIA;
	event.user.data1 = b;
	SDL_PushEvent(&event);
}

bool Wave::isDead() {
    bool dead = true;
    for (Bacterium* b : m_bacteria)
        if (!b->isDead())
            dead = false;
    return dead;
}