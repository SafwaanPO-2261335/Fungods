#pragma once

#include "Location.h"
#include <vector>

class Path
{
private:
	std::vector<Location> m_path;
public:
	void addLoc(Location loc) { m_path.push_back(loc); }
	Location getLoc(int index) { return m_path.at(index); }
	size_t getSize() { return m_path.size(); }
};

