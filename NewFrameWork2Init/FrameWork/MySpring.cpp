#include "MySpring.h"

cyclone::MySpring::MySpring(cyclone::Particle* p, double springConstant, double restLenght)
{
	other = p;
	this->springConstant = springConstant;
	this->restLength = restLenght;

    this->anchor = nullptr;
}

cyclone::MySpring::MySpring(cyclone::Vector3* anchor, double springConstant, double restLenght)
{
    this->anchor = anchor;
    this->springConstant = springConstant;
    this->restLength = restLenght;
    
    this->other = nullptr;
}

void cyclone::MySpring::updateForce(cyclone::Particle* p, double duration)
{
    cyclone::Vector3 force = p->getPosition() - ((other != nullptr) ? other->getPosition() : *anchor);

    double currentLength = force.magnitude();

    //Hooke's Law
    double forceMagnitude = (currentLength - restLength) * springConstant;

    force.normalise();
    force *= -forceMagnitude;

	p->addForce(force);
}