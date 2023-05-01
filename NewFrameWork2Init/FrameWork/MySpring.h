#include <FL/Fl_Gl_Window.h>
#include <Fl/Fl.h>
#include <Fl/Fl_Value_Slider.H>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <Fl/Fl_Double_Window.h>

#include "stdio.h"
#include "math.h"
#include "3DUtils.h"

#include "Vec3f.h"
#include "Viewer.h"
#include <vector>

#include "core.h"
#include "particle.h"
#include "pfgen.h"

namespace cyclone {
    class MySpring : public ParticleForceGenerator  //Force generating class
    {
        cyclone::Vector3* anchor;
        cyclone::Particle* other;  //Target particle
        double springConstant; //Spring constant (k)
        double restLength;  //lest length
    public:
        MySpring(cyclone::Particle* p, double springConstant, double restLenght);
        MySpring(cyclone::Vector3* constant, double springConstant, double restLenght);
        virtual void updateForce(cyclone::Particle* p, double duration);

        const Vector3* getAnchor() const { return anchor; } //return anchor position
    };
}
