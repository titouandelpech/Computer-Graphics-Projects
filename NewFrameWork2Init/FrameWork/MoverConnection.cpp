#include "MoverConnection.h"

MoverConnection::MoverConnection()
{
	m_movers = std::vector<Mover*>();
	//m_movers.push_back(new Mover(cyclone::Vector3(5, 2, 5), cyclone::Vector3(), 10.0f, 0.9f));
	//m_movers.push_back(new Mover(cyclone::Vector3(0, 2, 0), cyclone::Vector3(), 10.0f, 0.9f));
	//m_movers.push_back(new Mover(cyclone::Vector3(0, 4, 0), cyclone::Vector3(0, 0, 0), 1.0f, 0.7f));

	//m_movers[0]->setConnection(m_movers[1]);
	//m_movers[1]->setConnection(m_movers[0]);
	//m_movers[0]->setConnection(new cyclone::Vector3(5, 15, 5));

	m_gravity = new cyclone::ParticleGravity(cyclone::Vector3(0, -10, 0));
	m_forces = new cyclone::ParticleForceRegistry();
	for (unsigned int i = 0; i < m_movers.size(); i++) {
		m_forces->add(m_movers[i]->m_particle, m_gravity);
		m_forces->add(m_movers[i]->m_particle, m_movers[i]->m_spring);
	}
}

void MoverConnection::update(float duration)
{
	m_forces->updateForces(duration);
	for (unsigned int i = 0; i < m_movers.size(); i++) {
		if (m_movers[i]) {
			m_movers[i]->update(duration);
		}
	}
}

void MoverConnection::draw(int shadow, int startI)
{
	for (unsigned int i = 0; i < m_movers.size(); i++) {
		if (!shadow) {
			glLoadName(i + startI + 1);
			if (i == 0) glColor3f(0.0f, 0.0f, 1.0f);
			else glColor3f(0.0f, 1.0f, 0.0f);
		}
		m_movers[i]->draw(shadow);
	}
	if (shadow) return;
	glColor3f(0, 0, 0);
	//Drawing a line for representing connection
	glBegin(GL_LINE_STRIP);
	for (unsigned int i = 0; i < m_movers.size(); i++) {
		cyclone::Vector3 p = m_movers[i]->m_particle->getPosition();
		glVertex3f(p.x, p.y, p.z);
		const cyclone::Vector3* anchor = m_movers[i]->m_spring ? m_movers[i]->m_spring->getAnchor() : nullptr;
		if (anchor)
		{
			glVertex3f(anchor->x, anchor->y, anchor->z);
		}
	}
	glEnd();
}