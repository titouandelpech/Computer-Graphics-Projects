#include "ParticleCollision.h"

unsigned cyclone::ParticleCollision::addContact(cyclone::ParticleContact* contact, unsigned limit) const
{
    contact->particle[0] = particle[0];
    contact->particle[1] = particle[1];
    cyclone::Vector3 position0 = contact->particle[0]->getPosition();
    cyclone::Vector3 position1 = contact->particle[1]->getPosition();

    cyclone::Vector3 difference = position0 - position1;
    cyclone::real distance = difference.magnitude();
    cyclone::real collisionThreshold = 2 * size;

    if (distance <= collisionThreshold)
    {
        // Calculate the contact normal
        cyclone::Vector3 normal = position0 - position1;
        normal.normalise();
        contact->contactNormal = normal;

        // Calculate the penetration
        contact->penetration = collisionThreshold - distance;

        // Set the restitution
        contact->restitution = 1.0;

        return 1;
    }

    return 0;
}