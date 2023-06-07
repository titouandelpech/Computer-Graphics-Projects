#include <vector>
#include "particle.h"
#include "plinks.h"
namespace cyclone {
	class MyEdgeContact : public cyclone::ParticleContactGenerator
	{
		std::vector<cyclone::Particle*> particles; //Container for all particles
		double size; //particle size(all particles have a same size)
	public:
		MyEdgeContact();
		~MyEdgeContact();
		void init(cyclone::Particle* p, double size); //add particle p in the container
		//Detect collision. If there are collisions, write info in the contact (limit=max # of contacts)
		//If there are collision, return the number of collisions
		virtual unsigned addContact(cyclone::ParticleContact* contact, unsigned limit) const;
	};
}