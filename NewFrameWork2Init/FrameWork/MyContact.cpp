#include "MyContact.h"

cyclone::MyGroundContact::MyGroundContact()
{
}

cyclone::MyGroundContact::~MyGroundContact()
{
}

void cyclone::MyGroundContact::init(cyclone::Particle *p, double size)
{
	if (p)
		particles.push_back(p);
	this->size = size;
}

unsigned cyclone::MyGroundContact::addContact(cyclone::ParticleContact* contact, unsigned limit) const
{
	unsigned count = 0;
	for (int i = 0; i < particles.size(); i++) {
		cyclone::Particle* p = particles[i];
		cyclone::real y = p->getPosition().y;

		if (y - size <= 0) {
			contact->contactNormal = cyclone::Vector3(0, 1, 0);

			contact->particle[0] = p;
			contact->particle[1] = nullptr; // The floor is static

			contact->penetration = size - y;

			contact->restitution = 1.0;

			count++;
			contact++;
			if (count >= limit) return count;
		}
	}
	return count;
}
