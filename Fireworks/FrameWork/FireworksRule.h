#pragma once
#include <deque>
#include "particle.h"
#include <random.h>


class FireworksRule
{
public:
	FireworksRule() {};
	~FireworksRule() {};
	unsigned type; //Fire type
	cyclone::real minAge; //minimum age of Fire
	cyclone::real maxAge; //maximum age of Fire
	cyclone::Vector3 minVelocity; //minimum velocity of Fire
	cyclone::Vector3 maxVelocity; //maximum velocity of Fire
	cyclone::real damping; //Fire’s damping
	cyclone::Vector3 color;
	unsigned payloadCount; //how many Child Fires are spawned?
	void setParameters(unsigned type, cyclone::real minAge, cyclone::real maxAge,
		const cyclone::Vector3& minVelocity, const cyclone::Vector3& maxVelocity,
		cyclone::real damping, int count);
};

static cyclone::Random crandom;