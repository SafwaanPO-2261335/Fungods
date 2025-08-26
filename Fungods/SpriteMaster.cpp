#include "SpriteMaster.h"


bool SpriteMaster::init() { 
    bool init_succes = SDL_CreateWindowAndRenderer("Fungods: Defence", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &m_window, &m_renderer);
    m_sprites.push_back(new Sprite(m_renderer, "Entities", "Entities", "default"));
    return init_succes;
}

int SpriteMaster::addEntity(std::string group, std::string name, int* w, int* h) {
    int match = 0;
    for (int i = 0; i < m_sprites.size(); i++)
        if (m_sprites.at(i)->equals(group, name))
            match = i;
    if (!match) {
        Sprite* s = new Sprite(m_renderer, "Entities", group, name);
        if (s->getValid()) {
            match = (int)m_sprites.size();
            m_sprites.push_back(s);
        }
    }
    m_sprites.at(match)->getDimentions(w, h);
    return match;
}

int SpriteMaster::addLevel(std::string group, std::string name) {
    int match = 0;
    for (int i = 0; i < m_sprites.size(); i++)
        if (m_sprites.at(i)->equals(group, name))
            match = i;
    if (!match) {
        Sprite* s = new Sprite(m_renderer, "Levels", group, name);
        if (s->getValid()) {
            match = (int)m_sprites.size();
            m_sprites.push_back(s);
        }
    }
    return match;
}

int SpriteMaster::addGui(std::string group, std::string name) {
    int match = 0;
    for (int i = 0; i < m_sprites.size(); i++)
        if (m_sprites.at(i)->equals(group, name))
            match = i;
    if (!match) {
        Sprite* s = new Sprite(m_renderer, "GUI", group, name);
        if (s->getValid()) {
            match = (int)m_sprites.size();
            m_sprites.push_back(s);
        }
    }
    return match;
}

void SpriteMaster::cls() {
    SDL_RenderClear(m_renderer);
}

void SpriteMaster::renderLevel(Level* l) {
    SDL_FRect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = WINDOW_WIDTH;
    rect.h = WINDOW_HEIGHT;
    m_sprites.at(l->getSpriteIndex())->render(m_renderer, &rect);
}

void SpriteMaster::renderEntity(Entity* e) {
    SDL_FRect r = *(e->getRect());
    r.x *= GW_WIDTH_FACTOR;
    r.y *= GW_HEIGHT_FACTOR;
    r.w *= GW_WIDTH_FACTOR;
    r.h *= GW_HEIGHT_FACTOR;

    Sprite* s = m_sprites.at(e->getSpriteIndex());

    if (e->getEnlightened()) {
        s->renderEnlightened(m_renderer, &r);
    }
    else {
        s->render(m_renderer, &r);
    }
}

void SpriteMaster::renderGui(int index, int x, int y) {
    int w, h;
    m_sprites.at(index)->getDimentions(&w, &h);
    x *= GW_WIDTH_FACTOR;
    y *= GW_HEIGHT_FACTOR;
    w *= GW_WIDTH_FACTOR;
    h *= GW_HEIGHT_FACTOR;

    SDL_FRect rect;
    rect.x = x - w/2;
    rect.y = y - h/2;
    rect.w = w;
    rect.h = h;

    m_sprites.at(index)->render(m_renderer, &rect);
}

void SpriteMaster::renderMycelium(Mycelium* m) {
    SDL_Texture* t = m->getTexture(m_renderer);
    SDL_RenderTexture(m_renderer, t, NULL, m->getRect());
}

void SpriteMaster::renderScreen() {
    SDL_RenderPresent(m_renderer);
}