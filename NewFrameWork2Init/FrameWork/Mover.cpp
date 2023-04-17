#include "Mover.h"

Mover::Mover()
{
	size = 1.5;
	has_collisions = true; //false;
	m_particle = new cyclone::Particle();

	//m_particle->setPosition(5, 20, 0);  //initial pos
	//m_particle->setVelocity(0, 0, 35.0f); //initial vel
	//m_particle->setMass(2.0f); //mass
	//m_particle->setDamping(0.99f); //damping
	//m_particle->setAcceleration(0.0f, -1.0f, 0.0f);  //initial acc.

	m_particle->setPosition(5, 20, 0);  //initial pos
	m_particle->setVelocity(0, 0, 0); //initial vel
	m_particle->setMass(1.0f); //mass
	m_particle->setDamping(1); //damping
	m_particle->setAcceleration(0, 0, 0);  //initial acc.

	m_gravity = new cyclone::ParticleGravity(cyclone::Vector3(0, -10, 0));
	m_drag = new cyclone::ParticleDrag(0.1, 0.1);

	m_forces = new cyclone::ParticleForceRegistry();  //Container
	m_forces->add(m_particle, m_gravity);
	m_forces->add(m_particle, m_drag);

	m_spring = NULL;
}

Mover::Mover(cyclone::Vector3 position, cyclone::Vector3 velocity, float mass, float damping)
{
	size = 1.5;
	has_collisions = true; //false;
	m_particle = new cyclone::Particle();

	//m_particle->setPosition(5, 20, 0);  //initial pos
	//m_particle->setVelocity(0, 0, 35.0f); //initial vel
	//m_particle->setMass(2.0f); //mass
	//m_particle->setDamping(0.99f); //damping
	//m_particle->setAcceleration(0.0f, -1.0f, 0.0f);  //initial acc.

	m_particle->setPosition(position);  //initial pos
	m_particle->setVelocity(velocity); //initial vel
	m_particle->setMass(mass); //mass
	m_particle->setDamping(damping); //damping
	m_particle->setAcceleration(0, 0, 0);  //initial acc.

	m_gravity = new cyclone::ParticleGravity(cyclone::Vector3(0, -10, 0));
	m_drag = new cyclone::ParticleDrag(0.1, 0.1);

	m_forces = NULL;
	m_spring = NULL;
	//m_forces = new cyclone::ParticleForceRegistry();  //Container
	//m_forces->add(m_particle, m_gravity);
	//m_forces->add(m_particle, m_drag);
}

void Mover::update(float duration)
{
	cyclone::Vector3 wind(1.0f, 0, 0);  //Wind blows in x direction
	//m_particle->addForce(wind);

	if (m_forces != NULL)
		m_forces->updateForces(duration);
	m_particle->integrate(duration);
	if (has_collisions) checkEdges();
}

void Mover::draw(int shadow)
{
	cyclone::Vector3 position;
	m_particle->getPosition(&position);

	//if (!shadow)
	//	glColor3f(1.0f, 0.0f, 0.0f);

	glPushMatrix(); // Add this line to isolate the transformation

	glTranslatef(position.x, position.y, position.z);
	glutSolidSphere(size, 30, 30);

	glPopMatrix(); // Add this line to isolate the transformation

	if (shadow)
		glColor3f(0.1f, 0.1f, 0.1f);
}

void Mover::checkEdges()
{
	cyclone::Vector3 position;
	cyclone::Vector3 velocity;
	m_particle->getPosition(&position);
	m_particle->getVelocity(&velocity);

	if (position.y - size <= 0)
	{
		velocity = cyclone::Vector3(velocity.x, -velocity.y, velocity.z);
		position = cyclone::Vector3(position.x, size, position.z);
	}
	if (position.x + size >= 100)
	{
		velocity = cyclone::Vector3(-velocity.x, velocity.y, velocity.z);
		position = cyclone::Vector3(100 - size, position.y, position.z);
	}
	else if (position.x - size <= -100)
	{
		velocity = cyclone::Vector3(-velocity.x, velocity.y, velocity.z);
		position = cyclone::Vector3(-100 + size, position.y, position.z);
	}
	if (position.z + size >= 100)
	{
		velocity = cyclone::Vector3(velocity.x, velocity.y, -velocity.z);
		position = cyclone::Vector3(position.x, position.y, 100 - size);
	}
	else if (position.z - size <= -100)
	{
		velocity = cyclone::Vector3(velocity.x, velocity.y, -velocity.z);
		position = cyclone::Vector3(position.x, position.y, -100 + size);
	}

	m_particle->setVelocity(velocity);
	m_particle->setPosition(position);
}

void Mover::setConnection(Mover* a)
{
	m_spring = new cyclone::MySpring(a->m_particle, 20.0f, 3);
}
