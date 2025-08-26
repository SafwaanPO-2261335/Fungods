#include "Psilocybe.h"

void Psilocybe::live() {
	m_mycelium->live();
	for (Entity* e : *m_colleagues)
	{
		if (e->getTeam() == this->m_team)
			continue;
		if (e->getHealth() <8 && m_mycelium->checkSensors(e->getLocation(), e->getR2())) {
			bool r = reloaded();
			if (r) {
				e->enlighten(32);
			}
		}
	}
	postLive();
}