
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
#include <FL/Fl_Light_Button.H>

class MyGlWindow : public Fl_Gl_Window {
	public:
		MyGlWindow(int x, int y, int w, int h);
		
		void putText(char* string, int x, int y, float r, float g, float b);
		std::vector<Vec3f *> history;
		int selected;

		Fl_Slider* time;	
		int run;
		void update();
		void drawStuff();
		void doPick();
		void test();
		Fl_Light_Button* ui;
	private:
		void draw();					// standard FlTk
		
		
		int handle(int);				// standard FlTk
		
		float fieldOfView;
		Viewer *m_viewer;
		std::vector<Mover *> m_movers;
		MoverConnection *m_moverConnection;
		cyclone::Vector3 p1selected;
		void setProjection(int clearProjection = 1);
		void getMouseNDC(float& x, float& y);
		void setupLight(float x, float y, float z);
		Mover *getSelectedBall(int selected);
};

