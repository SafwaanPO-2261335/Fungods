#include "Tuber.h"

void Tuber::live() {
	bool r = reloaded();
	m_mycelium->live();
	for (Entity* e : *m_colleagues)
	{
		if (e->getTeam() == this->m_team)
			continue;
		if (m_mycelium->checkSensors(e->getLocation(), e->getR2())) {
			if (r) {
				e->hurt(1);
			}

		}
	}
	postLive();
}