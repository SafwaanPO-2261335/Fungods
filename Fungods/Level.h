#pragma once

#include "Thing.h"
#include "Wave.h"
#include "Path.h"
#include "PARAMS.h"

class Level : public Thing
{
private:
	std::vector<Path*> m_paths;
	std::vector<std::vector<Wave*>> m_path_waves;
	int m_currentWave = 0;
	bool m_living = false;
	
public:
	Level(std::string group, std::string name, Uint32 event);
	void live() override;
	bool relive();
};
