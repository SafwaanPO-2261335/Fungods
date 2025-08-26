#pragma once

#include "../../PARAMS.h"
#include <vector>
#include <random>
#include <iostream>
#include "../../Location.h"

class Hypha
{
private:
	int m_x;
	int m_y;
	Location m_l;
	Hypha* m_root;
	Hypha* m_pre;
	std::vector<Hypha*> m_post;
public:
	Hypha(int x, int y, Hypha* root = nullptr, Hypha* pre = nullptr)
		: m_x(x), m_y(y), m_root(root), m_pre(pre) { m_l = Location(float(m_x), float(m_y)); if (root == nullptr) m_root = this;
	}
	int getX() { return m_x; }
	int getY() { return m_y; }
	std::vector<Hypha*> grow(int branch, int turn);
	Hypha* getPre() { return m_pre; }
	void addHypha(int x, int y);
	int count();
	std::vector<Hypha*> getAll();
	std::vector<Hypha*> getRandom(int factor);
	float getDistance2(Location l) { return m_l.distance2(l); }
};
