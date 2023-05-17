#include <pcontacts.h>
#include "plinks.h"

namespace cyclone {
class ParticleCollision : public cyclone::ParticleLink
{
	public:
		ParticleCollision(double size = 0) : size(size) {}
		double size; //particle size
	private:
		virtual unsigned addContact(cyclone::ParticleContact* contact, unsigned limit) const;
	};
}