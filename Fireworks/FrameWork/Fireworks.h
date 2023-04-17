#include "Fire.h"

class Fireworks
{
public:
	Fireworks();
	~Fireworks();
	void update(float duration); //Fire create/update
	void handleFireworkSpawn(float duration);
	void handleDelete(float duration);
	void create(); //create init Fires 
	void create(Fire* parent); //create Child Fires
	void draw(int shadow); //draw
	FireworksRule m_rule[4]; //rules
	std::vector<Fire*> fireworks; //contains Fires

	bool wave1;
	bool wave2;
	bool wave3;

	float passedTime;
};