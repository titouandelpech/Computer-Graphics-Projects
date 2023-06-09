
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
#include "Mover.h"
#include "MoverConnection.h"
#include "MyContact.h"
#include "MyEventContact.h"
#include "ParticleCollision.h"
#include <FL/Fl_Light_Button.H>
#include <pworld.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

const int CABLE_COUNT = 10;
const int ROD_COUNT = 6;
const int SUPPORT_COUNT = 12;

class MyGlWindow : public Fl_Gl_Window {
	public:
		MyGlWindow(int x, int y, int w, int h);
		
		void putText(char* string, int x, int y, float r, float g, float b);
		std::vector<Vec3f *> history;
		int selected;

		Fl_Slider* time;	
		int run;
		bool checkRandomNb(float randomX, float randomZ);
		void update();
		void drawStuff();
		void doPick();
		void restart();
		void testValue(float t);
		Fl_Light_Button* ui;
	private:
		void initMovers();
		void getParticles(Mover* m);
		void draw();					// standard FlTk
		void addEventContact(float x, float y);
		void removeEventContact(cyclone::MyEventContact *);

		void drawBridge(int shadow);
		
		int handle(int);				// standard FlTk
		
		float fieldOfView;
		Viewer *m_viewer;
		std::vector<Mover *> m_movers;
		cyclone::ParticleWorld *m_world;
		cyclone::Vector3 p1selected;
		cyclone::Vector3 prevPoint;
		void setProjection(int clearProjection = 1);
		void getMouseNDC(float& x, float& y);
		void setupLight(float x, float y, float z);
		Mover *getSelectedBall(int selected);

		//Container for MyGroundContact
		int maxPossibleContact;
		int myScore;
		bool canPlay;
		bool endGame;

		bool edge1;
		bool edge2;
		bool edge3;
		bool edge4;
		std::vector<cyclone::MyEventContact*> m_event_contacts;
};