#include "Fire.h"

class Fireworks
{
public:
	struct Wave
	{
		float triggerTime;
		FireworksRule* rule;
		cyclone::Vector3 position;
		FireworksRule* ruleParent;

		Wave(float triggerTime, FireworksRule* rule, const cyclone::Vector3& position, FireworksRule* ruleParent)
			: triggerTime(triggerTime), rule(rule), position(position), ruleParent(ruleParent) {}
	};
	
	Fireworks();
	~Fireworks();
	void update(float duration); //Fire create/update
	void handleFireworkSpawn(float duration);
	void handleDelete(float duration);
	void create(FireworksRule *rule = NULL, cyclone::Vector3 *startPos = NULL, FireworksRule *parentRule = NULL); //create init Fires 
	void create(Fire* parent); //create Child Fires
	void draw(int shadow); //draw
	FireworksRule m_rule[10]; //rules
	std::vector<Fire*> fireworks; //contains Fires

	std::vector<Wave> waves;
	void CreateLine(float triggerTime, FireworksRule* rule, const cyclone::Vector3& pos, FireworksRule* ruleParent);
	void CreateRevertLine(float triggerTime, FireworksRule* rule, const cyclone::Vector3& pos, FireworksRule* ruleParent);
	void CreateZigZagLine(float triggerTime, FireworksRule* rule, const cyclone::Vector3& pos, FireworksRule* ruleParent);

	float passedTime;
};