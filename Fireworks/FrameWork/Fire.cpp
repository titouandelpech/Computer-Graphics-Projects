#include "Fire.h"

Fire::Fire(int type)
{
	m_type = type;
	m_size = 1;
	m_age = crandom.randomReal(0.5f, 3.0f);
	
	m_particle = new cyclone::Particle();

	m_particle->setPosition(crandom.randomReal(-5, 5), 10, crandom.randomReal(-5, 5));
	m_particle->setVelocity(crandom.randomVector(cyclone::Vector3(-10, 20, -10), cyclone::Vector3(10, 50, 10)));
	m_particle->setMass(1.0f);
	m_particle->setDamping(0.99f);
	cyclone::Vector3 gravity(0, -9.81, 0); // Gravity acceleration vector (e.g., -9.81 m/s^2 on the y-axis)
	m_particle->setAcceleration(gravity);

	m_color = crandom.randomVector(cyclone::Vector3(0, 0, 0), cyclone::Vector3(1, 1, 1));
	m_rule = NULL;
}

Fire::~Fire()
{
}

bool Fire::update(float duration)
{
	putHistory();
	m_particle->integrate(duration);
	m_age -= duration;

	if (m_age < 0 || m_particle->getPosition().y < 0)
		return true;
	return false;
}

void Fire::draw(int shadow)
{
	cyclone::Vector3 position;
	m_particle->getPosition(&position);

	if (!shadow)
		glColor3f(m_color.x, m_color.y, m_color.z);

	glPushMatrix(); // Add this line to isolate the transformation

	glTranslatef(position.x, position.y, position.z);
	glutSolidSphere(m_size, 30, 30);

	glPopMatrix(); // Add this line to isolate the transformation

	if (shadow)
		glColor3f(0.1f, 0.1f, 0.1f);
}

void Fire::drawHistory()
{
	glLineWidth(2.0f);
	glPushMatrix();
	glBegin(GL_LINE_STRIP);
	for (unsigned int i = 0; i < m_history.size(); i += 2) {
		cyclone::Vector3 pos = m_history[i];
		glVertex3f(pos.x, pos.y, pos.z);
	}
	glEnd();
	glPopMatrix();
	glLineWidth(1.0f);

}

void Fire::setRule(FireworksRule* r)
{
	m_rule = r;
}

void Fire::putHistory()
{
	if (m_history.size() < 30) {
		m_history.push_back(m_particle->getPosition());
	}
	else {
		m_history.pop_front();
		m_history.push_back(m_particle->getPosition());
	}
}
