#pragma once

#include "../Fungus.h"

class Psilocybe : public Fungus
{
public:
	Psilocybe(float x, float y) : Fungus("Psilocybe", x, y) {}
	void live() override;
};

