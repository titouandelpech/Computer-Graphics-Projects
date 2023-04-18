#include "FireworksRule.h"
#include <random.h>

void FireworksRule::setParameters(unsigned type, cyclone::real minAge, cyclone::real maxAge, const cyclone::Vector3& minVelocity, const cyclone::Vector3& maxVelocity, cyclone::real damping, int count)
{
	this->type = type;
	this->minAge = minAge;
	this->maxAge = maxAge;
	this->minVelocity = minVelocity;
	this->maxVelocity = maxVelocity;
	this->damping = damping;
	this->payloadCount = count;
	this->color = crandom.randomVector(cyclone::Vector3(0, 0, 0), cyclone::Vector3(1, 1, 1));
}