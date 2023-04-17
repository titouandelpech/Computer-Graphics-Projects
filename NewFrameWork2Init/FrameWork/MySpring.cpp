#include "MySpring.h"

cyclone::MySpring::MySpring(cyclone::Particle* p, double springConstant, double restLenght)
{
	other = p;
	this->springConstant = springConstant;
	this->restLength = restLenght;
}

void cyclone::MySpring::updateForce(cyclone::Particle* p, double duration)
{
    cyclone::Vector3 force = p->getPosition() - other->getPosition();

    double currentLength = force.magnitude();

    //Hooke's Law
    double forceMagnitude = (currentLength - restLength) * springConstant;

    force.normalise();
    force *= -forceMagnitude;

	p->addForce(force);
}