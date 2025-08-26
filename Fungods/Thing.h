#pragma once

#include <vector>
#include <string>
#include <filesystem>
#include <vector>

#include "Location.h"

class Thing
{
private:
	int m_spriteIndex = 0;
public:
	void setSpriteIndex(int val) { m_spriteIndex = val; };
	int getSpriteIndex() { return m_spriteIndex; };
	virtual void live() = 0;
};
