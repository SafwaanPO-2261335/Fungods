#pragma once

#include "math.h"

class Location {
private:
    float m_x = 0;
    float m_y = 0;

public:
    Location() {};
    Location(float x, float y) : m_x(x), m_y(y) {};
    void setX(float val) { m_x = val; };
    void setY(float val) { m_y = val; };
    float getX() { return m_x; };
    float getY() { return m_y; };
    float distance(Location target) { return float(sqrt(pow(target.getX() - m_x, 2) + pow(target.getY() - m_y, 2))); }
    float distance2(Location target) { return float(pow(target.getX() - m_x, 2) + pow(target.getY() - m_y, 2)); }
};

