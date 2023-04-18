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
#include "FireworksRule.h"

#include "pfgen.h"
#include <random.h>

class Fire
{
public:
	Fire(int type); //Fire type (0=Init Fire, 1=Child fire)
	~Fire();
	float m_size; // Fire size (0.5)
	int m_type; //Fire type (set in the constructor)
	float m_age; //Fire’s current age
	bool m_dead; //if ball 
	
	cyclone::Particle* m_particle;

	FireworksRule* m_rule;//Which rule this particle is influenced by?
	cyclone::Vector3 m_color; //Fire color
	std::deque <cyclone::Vector3> m_history; //history saving
	bool update(float duration); //return bool
	void draw(int shadow);
	void drawHistory(float alpha);
	void setRule(FireworksRule* r); //set a particular rule
	void putHistory(); //Save current position in the m_history
};