#include "Fireworks.h"

Fireworks::Fireworks()
{
	passedTime = 0;

	//set child rules
	m_rule[0].setParameters(1, 0.5f, 1.0f, cyclone::Vector3(-5, -10, -5), cyclone::Vector3(5, 15, 5), 0.9, 25);
	m_rule[0].color = cyclone::Vector3(1, 0, 0); //red
	m_rule[2].setParameters(1, 6.0f, 6.5f, cyclone::Vector3(-25, -25, -25), cyclone::Vector3(25, 25, 25), 0.9, 150);
	m_rule[2].color = cyclone::Vector3(0, 1, 1); //cyan
	
	//set parent rules
	m_rule[1].setParameters(0, 4.0f, 4.1f, cyclone::Vector3(-0.1f, 30, -0.1f), cyclone::Vector3(0.1f, 30, 0.1f), 1.0, 1);
	m_rule[1].color = cyclone::Vector3(1, 0.5f, 0); //orange
	m_rule[3].setParameters(0, 4.0f, 4.1f, cyclone::Vector3(-0.1f, 35, -0.1f), cyclone::Vector3(0.1f, 35, 0.1f), 1.0, 1);
	m_rule[3].color = cyclone::Vector3(0.125f, 0.698f, 0.666f); //seagreen

	//set firework waves
	CreateLine(0, &m_rule[0], cyclone::Vector3(0, 0, 0), &m_rule[1]);
	waves.emplace_back(5, &m_rule[2], cyclone::Vector3(), &m_rule[3]);
	CreateRevertLine(8, &m_rule[0], cyclone::Vector3(0, 0, 0), &m_rule[1]);
	waves.emplace_back(13, &m_rule[2], cyclone::Vector3(-40, 0, 0), &m_rule[3]);
	waves.emplace_back(13, &m_rule[2], cyclone::Vector3(40, 0, 0), &m_rule[3]);
	//CreateLine(2, &m_rule[0], cyclone::Vector3(0, 0, 0), &m_rule[4]);
	//CreateLine(4, &m_rule[0], cyclone::Vector3(0, 0, 0), &m_rule[4]);
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
	for (auto it = waves.begin(); it != waves.end();)
	{
		if (passedTime > it->triggerTime)
		{
			create(it->rule, &it->position, it->ruleParent);
			it = waves.erase(it);
		}
		else
		{
			++it;
		}
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

void Fireworks::create(FireworksRule *rule, cyclone::Vector3 *startPos, FireworksRule* parentRule)
{
	Fire *initFire = new Fire(0);
	if (!rule)
		initFire->setRule(&m_rule[rand() % 4]);
	else
		initFire->setRule(rule);
	if (startPos)
		initFire->m_particle->setPosition(*startPos);
	if (parentRule)
	{
		initFire->m_age = crandom.randomReal(parentRule->maxAge, parentRule->maxAge);
		initFire->m_particle->setVelocity(crandom.randomVector(parentRule->minVelocity, parentRule->maxVelocity));
		initFire->m_particle->setDamping(parentRule->damping);
		initFire->m_color = parentRule->color;
	}
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
		childFire->m_color = parent->m_rule->color;

		fireworks.push_back(childFire);
	}
}

void Fireworks::draw(int shadow)
{
	std::vector<Fire* >::iterator iter; //Fire container
	for (iter = fireworks.begin(); iter != fireworks.end();) {
		Fire* m = *iter;
		if (!m->m_dead)
			m->draw(shadow);
		if (!shadow)
			m->drawHistory();
		++iter;
	}
}

void Fireworks::CreateLine(float triggerTime, FireworksRule* rule, const cyclone::Vector3& pos, FireworksRule* ruleParent)
{
	for (int i = 0; i < 10; ++i)
	{
		float currentTime = triggerTime + 0.5f * i;
		int xOffset = 50 - 5 * i;
		waves.emplace_back(currentTime, rule, cyclone::Vector3(pos.x + xOffset, pos.y, pos.z), ruleParent);
		waves.emplace_back(currentTime, rule, cyclone::Vector3(pos.x - xOffset, pos.y, pos.z), ruleParent);
	}
}

void Fireworks::CreateRevertLine(float triggerTime, FireworksRule* rule, const cyclone::Vector3& pos, FireworksRule* ruleParent)
{
	for (int i = 0; i < 10; ++i)
	{
		float currentTime = triggerTime + 0.5f * i;
		int xOffset = 5 * i;
		waves.emplace_back(currentTime, rule, cyclone::Vector3(pos.x + xOffset, pos.y, pos.z), ruleParent);
		waves.emplace_back(currentTime, rule, cyclone::Vector3(pos.x - xOffset, pos.y, pos.z), ruleParent);
	}
}

void Fireworks::CreateZigZagLine(float triggerTime, FireworksRule* rule, const cyclone::Vector3& pos, FireworksRule* ruleParent)
{
	float timeInterval = 0.5f;
	int offset = 5;

	for (int i = 0; i < 10; ++i)
	{
		float currentTime = triggerTime + timeInterval * i;
		int xCoordinate = (i % 2 == 0) ? pos.x + offset * i : pos.x - offset * i;
		waves.emplace_back(currentTime, rule, cyclone::Vector3(xCoordinate, pos.y, pos.z), ruleParent);
	}
}
