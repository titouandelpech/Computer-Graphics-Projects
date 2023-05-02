#include "core.h"
#include "particle.h"
#include "pfgen.h"

namespace cyclone {
    class MyParticleBuoyancy : public ParticleForceGenerator  //Force generating class
    {
        real maxDepth;
        real volume;
        real waterHeight;
        real liquidDensity;
    public:
        MyParticleBuoyancy::MyParticleBuoyancy(real maxDepth, real volume, real waterHeight, real liquidDensity) : maxDepth(maxDepth), volume(volume), waterHeight(waterHeight), liquidDensity(liquidDensity)
        { }
        virtual void updateForce(cyclone::Particle* p, double duration);
    };
}