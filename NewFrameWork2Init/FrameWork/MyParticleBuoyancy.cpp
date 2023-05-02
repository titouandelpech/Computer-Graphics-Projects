#include "MyParticleBuoyancy.h"

void cyclone::MyParticleBuoyancy::updateForce(cyclone::Particle* particle, double duration)
{
	cyclone::Vector3 force;
	cyclone::Vector3 pos = particle->getPosition();

	double h = pos.y;  //Particle’s y position

	//For each 3 cases, calculate buoyancy force separatel
	if (pos.y - maxDepth > waterHeight)								//outside of water
		force = cyclone::Vector3::GRAVITY;
	else if (pos.y + maxDepth < waterHeight)						//totally in water
		force = cyclone::Vector3(0, volume * liquidDensity, 0);
	else															//partially in water
		force = cyclone::Vector3(0, volume * liquidDensity * ((pos.y - waterHeight - maxDepth) / 2 * maxDepth), 0);
	particle->addForce(force);
}