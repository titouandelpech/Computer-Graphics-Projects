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
		cyclone::Vector3 position = p->getPosition();
		cyclone::real y = position.y;
		cyclone::real x = position.x;
		cyclone::real z = position.z;
		const float wallPosX = 50;
		const float wallPosZ = 100;
		const float wallThickness = 20.0f;
		const float wallHeight = 20.0f;

		if (x > wallPosX + wallThickness || x < -wallPosX - wallThickness || z > wallPosZ + wallThickness || z < -wallPosZ - wallThickness)
			continue;

		if (y - size <= 0) {
			contact->contactNormal = cyclone::Vector3(0, 1, 0);
			contact->penetration = size - y;
		}
		else if (x - size <= -wallPosX && y <= wallHeight) { // left wall
			contact->contactNormal = cyclone::Vector3(1, 0, 0);
			contact->penetration = size - x - wallPosX;
		}
		else if (x + size >= wallPosX && y <= wallHeight) { // right wall
			contact->contactNormal = cyclone::Vector3(-1, 0, 0);
			contact->penetration = - size + x - wallPosX;
		}
		else if (z - size <= -wallPosZ && y <= wallHeight) { // front wall
			contact->contactNormal = cyclone::Vector3(0, 0, 1);
			contact->penetration = size - z - wallPosZ;
		}
		else if (z + size >= wallPosZ && y <= wallHeight) { // back wall
			contact->contactNormal = cyclone::Vector3(0, 0, -1);
			contact->penetration = - size + z - wallPosZ;
		}
		else continue;
		contact->particle[0] = p;
		contact->particle[1] = nullptr; // static
		contact->restitution = 1.0;

		count++;
		contact++;
		if (count >= limit) return count;
	}
	return count;
}
