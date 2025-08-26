#include "Hypha.h"

std::vector<Hypha*> Hypha::grow(int branch, int turn) {

	if (m_pre == nullptr) {
		addHypha(m_x, m_y - 1);
		addHypha(m_x, m_y + 1);
		addHypha(m_x - 1, m_y);
		addHypha(m_x + 1, m_y);

		addHypha(m_x - 1, m_y - 1);
		addHypha(m_x + 1, m_y + 1);
		addHypha(m_x - 1, m_y + 1);
		addHypha(m_x + 1, m_y - 1);
		return m_post;
	}

	int o_x = m_x - m_pre->getX();
	int o_y = m_y - m_pre->getY();

	/*
	if (!(rand() % H_TARGET)) {
		int ot_x = m_x - m_root->getX();
		int ot_y = m_y - m_root->getY();
		int ot_m = abs(ot_x);
		if (abs(ot_y) > abs(ot_x))
			ot_m = abs(ot_y);

		if (ot_x != 0)
			ot_x = int(round(ot_x / ot_m));
		if (ot_y != 0)
			ot_y = int(round(ot_y / ot_m));
		addHypha(m_x + ot_x, m_y + ot_y);
		return m_post;
	}
	*/

	// diagonaal
	if (o_x * o_y) {
		if (!(rand() % branch)) {
			addHypha(m_x + o_x, m_y + o_y);
			addHypha(m_x, m_y + o_y);
			addHypha(m_x + o_x, m_y);
		}
		else if (!(rand() % turn)) {
			if (rand() % 2)
				addHypha(m_x, m_y + o_y);
			else
				addHypha(m_x + o_x, m_y);
		}
		else
			addHypha(m_x + o_x, m_y + o_y);
	}
	// horizont-/vertik-aal
	else {
		if (!(rand() % branch))
			if (o_x) {
				addHypha(m_x + o_x, m_y - 1);
				addHypha(m_x + o_x, m_y + 1);
			}
			else {
				addHypha(m_x - 1, m_y + o_y);
				addHypha(m_x + 1, m_y + o_y);
			}
		else if (!(rand() % turn)) {
			int o_n = -1;
			if (rand() % 2)
				o_n = 1;

			if (o_x)
				addHypha(m_x + o_x, m_y + o_n);
			else
				addHypha(m_x + o_n, m_y + o_y);
		}
		else
			addHypha(m_x + o_x, m_y + o_y);
	}

	return m_post;
}

void Hypha::addHypha(int x, int y) {
	if (x < 0 || y < 0 || x >= GRID_WIDTH || y >= GRID_HEIGHT)
		return;
	m_post.push_back(new Hypha(x, y, m_root, this));
}

int Hypha::count() {
	int i = 0;
	for (Hypha* h : m_post) {
		i += h->count();
	}
	return i + 1;
}

std::vector<Hypha*> Hypha::getAll() {
	std::vector<Hypha*> r;
	for (Hypha* h : m_post) {
		std::vector<Hypha*> n_r = h->getAll();
		r.insert(r.end(), n_r.begin(), n_r.end());
	}
	r.push_back(this);
	return r;
}

std::vector<Hypha*> Hypha::getRandom(int factor) {
	std::vector<Hypha*> r;
	for (Hypha* h : m_post) {
		std::vector<Hypha*> n_r = h->getRandom(factor);
		r.insert(r.end(), n_r.begin(), n_r.end());
	}
	if (!(rand() % factor))
		r.push_back(this);
	return r;
}