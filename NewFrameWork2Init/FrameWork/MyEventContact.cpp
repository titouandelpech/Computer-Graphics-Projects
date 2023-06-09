#include "MyEventContact.h"

cyclone::MyEventContact::MyEventContact(float x, float z)
{
	PosX = x;
	PosZ = z;
	wallHeight = 0;
	moveUp = true;
	startTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	lifeTime = std::chrono::milliseconds(5000 + (rand() % (25000 - 5000 + 1)));
}

cyclone::MyEventContact::~MyEventContact()
{
}

void cyclone::MyEventContact::init(cyclone::Particle* p, double size)
{
	if (p)
		particles.push_back(p);
	this->size = size;
}

unsigned cyclone::MyEventContact::addContact(cyclone::ParticleContact* contact, unsigned limit) const
{
	unsigned count = 0;
	for (int i = 0; i < particles.size(); i++) {
		cyclone::Particle* p = particles[i];
		cyclone::Vector3 position = p->getPosition();
		cyclone::real y = position.y;
		cyclone::real x = position.x;
		cyclone::real z = position.z;
		const float wallThickness = 2.5f;

		if (x >= PosX - wallThickness - size && x <= PosX + wallThickness + size &&
			z >= PosZ - wallThickness - size && z <= PosZ + wallThickness + size &&
			y <= wallHeight + size) {
			
			cyclone::real minDistance = std::min(
				std::min(x - PosX + wallThickness, PosX + wallThickness - x), 
				std::min(std::min(z - PosZ + wallThickness, PosZ + wallThickness - z), 
				wallHeight - y)
			);

			cyclone::Vector3 normal;

			if (minDistance == x - PosX + wallThickness)
				normal = cyclone::Vector3(-1, 0, 0);
			else if (minDistance == PosX + wallThickness - x)
				normal = cyclone::Vector3(1, 0, 0);
			else if (minDistance == z - PosZ + wallThickness)
				normal = cyclone::Vector3(0, 0, -1);
			else if (minDistance == PosZ + wallThickness - z)
				normal = cyclone::Vector3(0, 0, 1);
			else
				normal = cyclone::Vector3(0, -1, 0);

			contact->contactNormal = normal;
			contact->penetration = minDistance - size;
		}
		else continue;
		contact->particle[0] = p;
		contact->particle[1] = nullptr;
		contact->restitution = 1.0;

		count++;
		contact++;
		if (count >= limit) return count;
	}
	return count;
}
