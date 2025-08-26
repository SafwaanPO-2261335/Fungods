#include "Penicillium.h"

void Penicillium::live() {
	bool r = reloaded();
	m_mycelium->live();
	for (Entity* e : *m_colleagues)
	{
		if (e->getTeam() == this->m_team)
			continue;
		if (m_mycelium->checkSensors(e->getLocation(), e->getR2())) {
			if (r) {
				float sf = e->getSpeedFactor();
				if (sf > 0.2)
					sf /= 1.2f;
				if (sf < 0.2)
					sf = 0.2f;
				e->slow(sf, 8);
			}
				
		}
	}
	postLive();
}