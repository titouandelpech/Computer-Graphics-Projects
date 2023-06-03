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

#include "MySpring.h"
#include "MyParticleBuoyancy.h"

class Mover
{
	public:
		float size;
		cyclone::Particle *m_particle;
		cyclone::MySpring* m_spring;
		cyclone::MyParticleBuoyancy* m_buoyancy;
		bool has_collisions;
		bool displayAxis;
		int type;
		cyclone::Vector3 color;
	
		cyclone::ParticleGravity *m_gravity;
		cyclone::ParticleDrag *m_drag;
		cyclone::ParticleForceRegistry* m_forces;
		std::vector<cyclone::ParticleForceGenerator *> m_forces_list;

		cyclone::Matrix4 transformMatrix;
		cyclone::Quaternion orientation;

		Mover();
		Mover(cyclone::Vector3, cyclone::Vector3, float, float, int type = 0);
		~Mover() { };
	
		void update(float duration);
		void draw(int shadow);
		void checkEdges();
		void setConnection(Mover* a);
		void setConnection(cyclone::Vector3* a);
		void getGLTransform(float matrix[16]);

	private:
		void drawMyCube(int shadow, cyclone::Vector3 color);
		void drawSphere(int shadow, cyclone::Vector3 color);
		void drawWater();
		void drawBlackHole();
};
#pragma once
