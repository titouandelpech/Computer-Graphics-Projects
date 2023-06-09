#include <vector>
#include <chrono>
#include <random>
#include "particle.h"
#include "plinks.h"
namespace cyclone {
	class MyEventContact : public cyclone::ParticleContactGenerator
	{
		std::vector<cyclone::Particle*> particles; //Container for all particles
		double size; //particle size(all particles have a same size)
	public:
		float PosX;
		float PosZ;
		float wallHeight;
		bool moveUp;
		std::chrono::milliseconds startTime;
		std::chrono::milliseconds lifeTime;
		bool shouldDestroy() {
			auto currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
			return currentTime >= startTime + lifeTime;
		}
		MyEventContact(float x, float z);
		~MyEventContact();
		void init(cyclone::Particle* p, double size); //add particle p in the container
		//Detect collision. If there are collisions, write info in the contact (limit=max # of contacts)
		//If there are collision, return the number of collisions
		virtual unsigned addContact(cyclone::ParticleContact* contact, unsigned limit) const;
	};
}