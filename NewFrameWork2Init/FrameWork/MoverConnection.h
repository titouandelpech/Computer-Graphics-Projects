#include "Mover.h"

class MoverConnection
{
public:
	MoverConnection();
	~MoverConnection() {
		for (unsigned int i = 0; i < m_movers.size(); i++) {
			m_movers[i]->~Mover();
		}
	};
	cyclone::ParticleGravity* m_gravity;
	cyclone::ParticleForceRegistry* m_forces;
	std::vector<Mover*> m_movers;
	void update(float duration);
	void draw(int shadow, int startI);
};