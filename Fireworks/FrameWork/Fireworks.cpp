#include "Fireworks.h"

Fireworks::Fireworks()
{
	passedTime = 0;

	//set child rules
	m_rule[0].setParameters(1, 0.5f, 1.0f, cyclone::Vector3(-5, -10, -5), cyclone::Vector3(5, 15, 5), 0.9, 25);
	m_rule[0].color = cyclone::Vector3(1, 0, 0); //red
	m_rule[2].setParameters(1, 3.0f, 3.5f, cyclone::Vector3(-20, -20, -20), cyclone::Vector3(20, 20, 20), 0.9, 100);
	m_rule[2].color = cyclone::Vector3(0, 1, 1); //cyan
	m_rule[4].setParameters(1, 1.0f, 1.5f, cyclone::Vector3(-16, -30, -16), cyclone::Vector3(16, 30, 16), 0.9, 2);
	m_rule[4].color = cyclone::Vector3(1, 0.780f, 1); //pink
	m_rule[6].setParameters(1, 0.4f, 0.6f, cyclone::Vector3(-25, -25, -25), cyclone::Vector3(25, 25, 25), 0.9, 10);
	m_rule[6].color = cyclone::Vector3(); //multicolor
	m_rule[8].setParameters(1, 3.0f, 3.5f, cyclone::Vector3(-20, -20, -20), cyclone::Vector3(20, 20, 20), 0.9, 60);
	m_rule[8].color = cyclone::Vector3(0.392f, 1, 0.392f); //green
	m_rule[10].setParameters(1, 4.0f, 4.5f, cyclone::Vector3(-25, -25, -25), cyclone::Vector3(25, 25, 25), 0.9, 60);
	m_rule[10].color = cyclone::Vector3(); //multicolor
	
	//set parent rules
	m_rule[1].setParameters(0, 4.0f, 4.1f, cyclone::Vector3(-0.1f, 30, -0.1f), cyclone::Vector3(0.1f, 30, 0.1f), 1.0, 1);
	m_rule[1].color = cyclone::Vector3(1, 0.5f, 0); //orange
	m_rule[3].setParameters(0, 4.0f, 4.1f, cyclone::Vector3(-0.1f, 35, -0.1f), cyclone::Vector3(0.1f, 35, 0.1f), 1.0, 1);
	m_rule[3].color = cyclone::Vector3(0.125f, 0.698f, 0.666f); //seagreen
	m_rule[5].setParameters(0, 2.0f, 2.5f, cyclone::Vector3(-0.1f, 30, -0.1f), cyclone::Vector3(0.1f, 33, 0.1f), 1.0, 1);
	m_rule[5].color = cyclone::Vector3(1, 0, 0.314f); //dark red
	m_rule[7].setParameters(0, 1.0f, 1.1f, cyclone::Vector3(-0.1f, 5, -0.1f), cyclone::Vector3(0.1f, 5, 0.1f), 1.0, 1);
	m_rule[7].color = cyclone::Vector3(1, 1, 1); //white
	m_rule[9].setParameters(0, 1.0f, 1.1f, cyclone::Vector3(-0.1f, 50, -0.1f), cyclone::Vector3(0.1f, 50, 0.1f), 1.0, 1);
	m_rule[9].color = cyclone::Vector3(1, 1, 1); //white

	//set firework waves
	CreateLine(0, &m_rule[0], cyclone::Vector3(0, 0, 0), &m_rule[1]);
	waves.emplace_back(5, &m_rule[2], cyclone::Vector3(), &m_rule[3]);
	CreateCross(6, &m_rule[6], cyclone::Vector3(0, 0, 0), &m_rule[7]);
	CreateCross(7, &m_rule[6], cyclone::Vector3(0, 0, 0), &m_rule[7]);
	CreateCross(8, &m_rule[6], cyclone::Vector3(0, 0, 0), &m_rule[7]);
	CreateRevertLine(8, &m_rule[0], cyclone::Vector3(0, 0, 0), &m_rule[1]);
	waves.emplace_back(13, &m_rule[8], cyclone::Vector3(-40, 0, 0), &m_rule[3]);
	waves.emplace_back(13, &m_rule[8], cyclone::Vector3(40, 0, 0), &m_rule[3]);
	CreateCross(20, &m_rule[4], cyclone::Vector3(0, 0, 0), &m_rule[5]);
	CreateCross(21, &m_rule[6], cyclone::Vector3(0, 0, 0), &m_rule[7]);
	CreateCross(22.5f, &m_rule[6], cyclone::Vector3(0, 0, 0), &m_rule[7]);
	CreateCross(24, &m_rule[6], cyclone::Vector3(0, 0, 0), &m_rule[7]);
	CreateCross(21, &m_rule[6], cyclone::Vector3(0, 0, 0), &m_rule[9]);
	CreateCross(22.5f, &m_rule[6], cyclone::Vector3(0, 0, 0), &m_rule[9]);
	CreateCross(24, &m_rule[6], cyclone::Vector3(0, 0, 0), &m_rule[9]);
	CreateCross(25.5f, &m_rule[6], cyclone::Vector3(0, 0, 0), &m_rule[7]);
	CreateCross(27, &m_rule[6], cyclone::Vector3(0, 0, 0), &m_rule[7]);
	CreateCross(28.5f, &m_rule[6], cyclone::Vector3(0, 0, 0), &m_rule[7]);
	CreateCross(30, &m_rule[6], cyclone::Vector3(0, 0, 0), &m_rule[7]);
	
	CreateLine(27, &m_rule[0], cyclone::Vector3(0, 0, 0), &m_rule[1]);
	CreateRevertLine(29, &m_rule[0], cyclone::Vector3(0, 0, 0), &m_rule[1]);
	CreateLine(31, &m_rule[0], cyclone::Vector3(0, 0, 0), &m_rule[1]);
	CreateRevertLine(33, &m_rule[6], cyclone::Vector3(0, 0, 0), &m_rule[1]);
	CreateLine(35, &m_rule[0], cyclone::Vector3(0, 0, 0), &m_rule[1]);
	CreateRevertLine(37, &m_rule[6], cyclone::Vector3(0, 0, 0), &m_rule[1]);
	CreateLine(39, &m_rule[0], cyclone::Vector3(0, 0, 0), &m_rule[1]);
	CreateRevertLine(41, &m_rule[6], cyclone::Vector3(0, 0, 0), &m_rule[1]);
	CreateLine(43, &m_rule[0], cyclone::Vector3(0, 0, 0), &m_rule[1]);
	CreateRevertLine(45, &m_rule[6], cyclone::Vector3(0, 0, 0), &m_rule[1]);
	waves.emplace_back(27, &m_rule[2], cyclone::Vector3(), &m_rule[3]);
	waves.emplace_back(35, &m_rule[8], cyclone::Vector3(), &m_rule[3]);
	waves.emplace_back(45, &m_rule[10], cyclone::Vector3(40, 0, 0), &m_rule[3]);
	waves.emplace_back(45, &m_rule[10], cyclone::Vector3(-40, 0, 0), &m_rule[3]);
	CreateCross(46, &m_rule[6], cyclone::Vector3(0, 0, 0), &m_rule[7]);
	CreateCross(47.5f, &m_rule[6], cyclone::Vector3(0, 0, 0), &m_rule[7]);

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
				delete m;
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
		delete m;
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
		if (parent->m_rule->color != cyclone::Vector3())
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
		++iter;
	}
	if (!shadow)
	{
		for (iter = fireworks.begin(); iter != fireworks.end();) {
			Fire* m = *iter;
			float alpha = (m->m_color == cyclone::Vector3(1, 1, 1)) ? 0 : 1;
			m->drawHistory(alpha);
			++iter;
		}
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

void Fireworks::CreateCross(float triggerTime, FireworksRule* rule, const cyclone::Vector3& pos, FireworksRule* ruleParent)
{
	for (int i = 0; i < 10; ++i)
	{
		int offset = 5 * i;
		waves.emplace_back(triggerTime, rule, cyclone::Vector3(pos.x + offset, pos.y, pos.z + (i % 2 == 0) ? offset : -offset), ruleParent);
		waves.emplace_back(triggerTime, rule, cyclone::Vector3(pos.x - offset, pos.y, pos.z + (i % 2 == 0) ? offset : -offset), ruleParent);
	}
}
