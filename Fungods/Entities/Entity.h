#pragma once

#include "../Thing.h"
#include <SDL3/SDL.h>
#include <random>
#include <iostream>
#include <fstream>



class Entity : public Thing {
private:
    bool collidecorners(SDL_FRect* r0, SDL_FRect* r1);

protected:
    std::string const m_group;
    std::string m_name;
    float m_x = 0;
    float m_y = 0;
    float m_w = -1;
    float m_h = -1;
    int m_team = -1;

    std::vector<Entity*>* m_colleagues = new std::vector<Entity*>;

    int m_healthMax = 1;
    int m_health = m_healthMax;
    bool m_dead = false;

    float m_a = 3;
    float m_da = 0.1f;
    float m_s = 10;

    float m_sFactor = 1;
    int m_sTick = 0;

    bool m_enlightened = false;
    int m_eTick = 0;

    float m_shadowX = m_x;
    float m_shadowY = m_y;
    float m_dx = 0;
    float m_dy = 0;

    int m_damage = 1;
    int m_delay = 0;
    int m_aTick = 0;

    Location m_loc;
    SDL_FRect m_rect = SDL_FRect(0, 0, 1, 1);
    float m_r = 0;
    float m_r2 = 0;

    Uint32 m_event = NULL;
    int m_spriteIndex = 0;

    void move();
    
    void track(Location target);
    bool reloaded();
    Entity* getTarget();

    void configStats();
    virtual void setStat(std::string key, std::string val);

public:
    Entity(std::string group, std::string name, float x, float y, int team=0);
    virtual void init(int w, int h, int spriteIndex, Uint32 event = NULL);
    void setX(float val) { m_x = val; };
    void setY(float val) { m_y = val; };
    void setW(float val) { m_w = val; };
    void setH(float val) { m_h = val; };
    void setR(float val) { m_r = val; m_r2 = val * val;};
    void setColleagues(std::vector<Entity*>* colleagues) { m_colleagues = colleagues; };
    float getX() { return m_x; };
    float getY() { return m_y; };
    float getW() { return m_w; };
    float getH() { return m_h; };
    float getR() { return m_r; };
    float getR2() { return m_r2; };
    bool isDead() { return m_dead; };
    int getTeam() { return m_team; };
    int getHealth() { return m_health; };
    std::string getgroup() { return m_group; };
    std::string getName() { return m_name; };
    Location getLocation() { return m_loc; };
    SDL_FRect* getRect() { return &m_rect; };
    bool collideRadius(Entity* to_check);
    bool collideRect(Entity* to_check);
    void hurt(int damage);
    virtual void die();
    void update();
    void tick() { m_aTick--; m_sTick--; m_eTick--; };
    float getSpeedFactor();
    void slow(float factor, int ticks);
    float getEnlightened();
    void enlighten(int ticks);
};
