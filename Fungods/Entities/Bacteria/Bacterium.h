#pragma once

#include "../Entity.h"
#include "../../EVENTS.h"
#include "../../PARAMS.h"
#include "../../Path.h"
#include <iostream>

class Bacterium : public Entity {
private:
    int m_score = 0;
    Path* m_path = new Path();
    int m_pathIndex = 1;
    void setStat(std::string key, std::string val) override;
public:
    Bacterium(std::string name, float x, float y) 
        : Entity("Bacteria", name, x, y, 1) {}
    Bacterium(std::string name, Path* path);
    void live() override;
    void die() override;
};
