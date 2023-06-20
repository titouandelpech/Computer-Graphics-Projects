#include "BlackHoleEvent.h"
#include <iostream>

cyclone::BlackHoleEvent::BlackHoleEvent(cyclone::Vector3 pos)
{
    blackHolePosition = pos;
	startTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	lifeTime = std::chrono::milliseconds(2000 + (rand() % (100000 - 20000 + 1)));
	this->attractionDistance = 1;
}

cyclone::BlackHoleEvent::~BlackHoleEvent()
{
}

void cyclone::BlackHoleEvent::init(cyclone::Particle* p)
{
	if (p)
		particles.push_back(p);
}

unsigned cyclone::BlackHoleEvent::addContact(cyclone::ParticleContact* contact, unsigned limit) const
{
    unsigned count = 0;

    for (int i = 0; i < particles.size(); i++) {
        cyclone::Particle* p = particles[i];
        cyclone::Vector3 position = p->getPosition();
        cyclone::Vector3 displacement = blackHolePosition - position;
        double distance = displacement.magnitude();
        const float G = 40;

        // Check if the particle is within the black hole's gravitational influence
        if (distance < attractionDistance) {
            cyclone::Vector3 direction = displacement;
            direction.unit();

            // Apply gravitational force
            cyclone::Vector3 force = direction * G;
            p->addForce(force);
        }
    }

    return count;
}
