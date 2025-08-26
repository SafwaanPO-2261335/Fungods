#pragma once

#include "../Fungus.h"

class Penicillium : public Fungus
{
public:
	Penicillium(float x, float y) : Fungus("Penicillium", x, y) {}
	void live() override;
};

