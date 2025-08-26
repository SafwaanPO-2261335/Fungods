#pragma once

#include "../Fungus.h"

class Hericium : public Fungus
{
public:
	Hericium(float x, float y) : Fungus("Hericium", x, y) {}
	void live() override;
};
