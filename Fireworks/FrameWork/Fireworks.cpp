#include "Fireworks.h"

Fireworks::Fireworks()
{
	passedTime = 0;

	wave1 = false;
	wave2 = false;
	wave3 = false;

	m_rule[0].setParameters(1, 0.5f, 5.4f, cyclone::Vector3(-5, 10, -5), cyclone::Vector3(5, 15, 5), 0.9, 20);
	m_rule[1].setParameters(1, 0.5f, 10.4f, cyclone::Vector3(-15, 5, -5), cyclone::Vector3(15, 20, 15), 0.2, 40);
	m_rule[2].setParameters(1, 0.5f, 12.4f, cyclone::Vector3(-5, 0, -5), cyclone::Vector3(5, 5, 5), 0.7, 25);
	m_rule[3].setParameters(1, 0.5f, 8.0f, cyclone::Vector3(-20, 30, -20), cyclone::Vector3(20, 35, 20), 0.5, 30);
}

Fireworks::~Fireworks()
{
}

void Fireworks::update(float duration)
{
	passedTime += duration;

	handleFireworkSpawn(duration);
	handleDelete(duration);
}

void Fireworks::handleFireworkSpawn(float duration)
{
	if (!wave1 && passedTime > 0.5f)
	{
		wave1 = true;
		for (int i = 0; i < 5; i++)
			create();
	}

	if (!wave2 && passedTime > 2.5f)
	{
		wave2 = true;
		for (int i = 0; i < 10; i++)
			create();
	}

	if (!wave3 && passedTime > 5.0f)
	{
		wave3 = true;
		for (int i = 0; i < 16; i++)
			create();
	}
}

void Fireworks::handleDelete(float duration)
{
	std::vector<Fire* >::iterator iter; //Fire container
	std::vector<Fire*> remove;

	for (iter = fireworks.begin(); iter != fireworks.end();) {
		Fire* m = *iter;
		if (m->update(duration)) {
			if (m->m_type == 0) { //init fire
				remove.push_back(m);
			}
			else {
				m->~Fire();
			}
			iter = fireworks.erase(iter);
		}
		else {
			++iter;
		}
	}

	for (iter = remove.begin(); iter != remove.end();) {
		Fire* m = *iter;
		create(m);
		m->~Fire();
		iter = remove.erase(iter);
	}
}

void Fireworks::create()
{
	Fire *initFire = new Fire(0);
	initFire->setRule(&m_rule[rand() % 4]);
	fireworks.push_back(initFire);
}

void Fireworks::create(Fire* parent)
{
	for (int i = 0; i < parent->m_rule->payloadCount; i++)
	{
		Fire* childFire = new Fire(1);

		childFire->m_age = crandom.randomReal(parent->m_rule->maxAge, parent->m_rule->maxAge);
		childFire->m_particle->setVelocity(crandom.randomVector(parent->m_rule->minVelocity, parent->m_rule->maxVelocity));
		childFire->m_particle->setDamping(parent->m_rule->damping);
		childFire->m_particle->setPosition(parent->m_particle->getPosition());

		fireworks.push_back(childFire);
	}
}

void Fireworks::draw(int shadow)
{
	std::vector<Fire* >::iterator iter; //Fire container
	for (iter = fireworks.begin(); iter != fireworks.end();) {
		Fire* m = *iter;
		m->draw(shadow);
		if (!shadow)
			m->drawHistory();
		++iter;
	}
}
