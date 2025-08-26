#pragma once

#include "../Fungus.h"

class Tuber : public Fungus
{
public:
	Tuber(float x, float y) : Fungus("Tuber", x, y) {}
	void live() override;
};

