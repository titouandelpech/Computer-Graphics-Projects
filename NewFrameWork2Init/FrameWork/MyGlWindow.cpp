#include "MyGlWindow.h"

#include <iostream>
#include "drawUtils.h"
#include "timing.h"


static double DEFAULT_VIEW_POINT[3] = {30, 30, 30};
static double DEFAULT_VIEW_CENTER[3] = {0, 0, 0};
static double DEFAULT_UP_VECTOR[3] = {0, 1, 0};



void drawStrokeText(char* string, int x, int y, int z)
{
	char* c;
	glPushMatrix();
	glTranslated(x, y + 8, z);
	glScaled(0.2, 0.2, 0.2);
	for (c = string; *c != '\0'; c++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
	}
	glPopMatrix();
}


void MyGlWindow::putText(char* string, int x, int y, float r, float g, float b)
{
	glDisable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	ortho();

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);


	glColor3f(r, g, b);
	drawStrokeText(string, x, y, 0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}




MyGlWindow::MyGlWindow(int x, int y, int w, int h) :
  Fl_Gl_Window(x,y,w,h)
//==========================================================================
{
    mode( FL_RGB|FL_ALPHA|FL_DOUBLE | FL_STENCIL );
 	
	fieldOfView = 45;

	glm::vec3 viewPoint(DEFAULT_VIEW_POINT[0], DEFAULT_VIEW_POINT[1], DEFAULT_VIEW_POINT[2]);
	glm::vec3 viewCenter(DEFAULT_VIEW_CENTER[0], DEFAULT_VIEW_CENTER[1], DEFAULT_VIEW_CENTER[2]);
	glm::vec3 upVector(DEFAULT_UP_VECTOR[0], DEFAULT_UP_VECTOR[1], DEFAULT_UP_VECTOR[2]);

	float aspect = (w / (float)h);
	m_viewer = new Viewer(viewPoint, viewCenter, upVector, 45.0f, aspect);
	m_viewer->zoom(-1);
	//m_resolver = new cyclone::ParticleContactResolver(1);

	initMovers();

	selected = -1;
	//maxPossibleContact = 21;

//	glutInit(0,0);

	TimingData::init();
	run = 0;
}

const float degrees2Radians = 3.141592f / 180;

cyclone::Quaternion generateQuaternion(float degrees, const cyclone::Vector3& axis) {
	cyclone::Quaternion orientation;

	float angle = degrees * degrees2Radians * 0.5f;

	orientation.r = cos(angle);
	cyclone::Vector3 v = axis * sin(angle);
	orientation.i = v.x;
	orientation.j = v.y;
	orientation.k = v.z;

	orientation.normalise();

	return orientation;
}

void MyGlWindow::initMovers()
{
	m_movers = std::vector<Mover*>();

	Mover* a = new Mover(cyclone::Vector3(1, 2, 0), cyclone::Vector3(0, 0, 0), 5, 0.9, 1);
	a->color = cyclone::Vector3(1, 0, 0);
	//Mover* c = new Mover(cyclone::Vector3(10, 2, 0), cyclone::Vector3(0, 0, 0), 5, 0.9, 1);
	//c->color = cyclone::Vector3(0, 1, 0);
	//Mover* d = new Mover(cyclone::Vector3(7, 2, 4), cyclone::Vector3(0, 0, 0), 5, 0.9, 1);
	//d->color = cyclone::Vector3(0, 0, 1);
	//Mover* e = new Mover(cyclone::Vector3(14, 2, -2), cyclone::Vector3(0, 0, 0), 5, 0.9, 1);
	//e->color = cyclone::Vector3(1, 0, 1);

	//Must input a 1/2 angle that you want to rotate
	a->orientation = generateQuaternion(-45, cyclone::Vector3(1, 0, 0));
	m_movers.push_back(a);
	//m_movers.push_back(c);
	//m_movers.push_back(d);
	//m_movers.push_back(e);
	
	//for (unsigned int i = 0; i < m_movers.size(); i++) {
	//	m_movers[i] = new Mover();
	//}
	m_moverConnection = new MoverConnection();

	//particle world
	m_world = new cyclone::ParticleWorld(21, 210);
	for each (Mover * m in m_movers) {
		getParticles(m);
	}
	//for each (Mover * m in m_moverConnection->m_movers) {
	//	getParticles(m);
	//}
	
	//collision between particles
	for (int i = 0; i < m_movers.size(); i++) {
		for (int j = i + 1; j < m_movers.size(); j++) {
			cyclone::ParticleCollision* myParticleCollisions = new cyclone::ParticleCollision();
			myParticleCollisions->particle[0] = m_movers[i]->m_particle;
			myParticleCollisions->particle[1] = m_movers[j]->m_particle;
			myParticleCollisions->size = m_movers[i]->size;
			//m_contactGenerators.push_back(myParticleCollisions);
			m_world->getContactGenerators().push_back(myParticleCollisions);
		}
	}

	//collision with the ground
	cyclone::MyEdgeContact* myEdgeContact = new cyclone::MyEdgeContact();
	for each (Mover * m in m_movers) {
		myEdgeContact->init(m->m_particle, m->size);
	}
	//for each (Mover * m in m_moverConnection->m_movers) {
	//	myGroundContact->init(m->m_particle, m->size);
	//}
	//m_contactGenerators.push_back(myGroundContact);
	m_world->getContactGenerators().push_back(myEdgeContact);

	myScore = 0;
	//bridge
	//m_particleArray = std::vector<cyclone::Particle*>(12);
	//for (int i = 0; i < 12; i++)
	//{
	//	m_particleArray[i] = new cyclone::Particle();
	//	m_particleArray[i]->setPosition(-5 + (2 * (i / 2)), 8, (i % 2 == 0) ? 1 : -1);
	//	m_particleArray[i]->setVelocity(0, 0, 0);
	//	m_particleArray[i]->setDamping(0.8f);
	//	m_particleArray[i]->setMass(5.0f);
	//	m_particleArray[i]->setAcceleration(cyclone::Vector3::GRAVITY);
	//	m_particleArray[i]->clearAccumulator();
	//	m_world->getParticles().push_back(m_particleArray[i]);
	//}
	//for (int i = 0; i < CABLE_COUNT; i++)
	//{
	//	cables[i].particle[0] = m_particleArray[i];
	//	cables[i].particle[1] = m_particleArray[i + 2];
	//	cables[i].maxLength = 3.0f;
	//	cables[i].restitution = 0.1f;
	//	m_world->getContactGenerators().push_back(&cables[i]);
	//}
	//for (int i = 0; i < ROD_COUNT; i++)
	//{
	//	rods[i].particle[0] = m_particleArray[2 * i];
	//	rods[i].particle[1] = m_particleArray[2 * i + 1];
	//	rods[i].length = 2;
	//	m_world->getContactGenerators().push_back(&rods[i]);
	//}
	//for (int i = 0; i < SUPPORT_COUNT; i++)
	//{
	//	supports[i].particle = m_particleArray[i];
	//	cyclone::Vector3 supportAnchor = m_particleArray[i]->getPosition();
	//	supportAnchor.y += 3;
	//	supports[i].anchor = supportAnchor;
	//	if (i > 3 && i < 8)
	//		supports[i].maxLength = 4.0f;
	//	else if (i == 2 || i == 3 || i == 8 || i == 9)
	//		supports[i].maxLength = 3.5f;
	//	else
	//		supports[i].maxLength = 3.0f;
	//	supports[i].restitution = 0.5f;
	//	m_world->getContactGenerators().push_back(&supports[i]);
	//}
}

void MyGlWindow::getParticles(Mover *m)
{
	m_world->getParticles().push_back(m->m_particle);
	for each (cyclone::ParticleForceGenerator *force in m->m_forces_list)
		m_world->getForceRegistry().add(m->m_particle, force);
	if (m->m_spring != nullptr)
		m_world->getForceRegistry().add(m->m_particle, m->m_spring);
}

  void MyGlWindow::setupLight(float x, float y, float z)
  {
	  // set up the lighting
	  GLfloat lightPosition[] = {500.0f, 900.0f, 500.0f, 1.0f};
	  GLfloat lightPosition2[] = {1, 0, 0, 0};
	  GLfloat lightPosition3[] = {0, -1, 0, 0};

	  GLfloat violetLight[] = {0.5f, 0.1f, .5f, 1.0f};
	  GLfloat whiteLight[] = {1.0f, 1.0f, 1.0f, 1.0f};
	  GLfloat whiteLight2[] = { .3f, .3f, .3f, 1.0f };
	  GLfloat blueLight[] = {.1f,.1f,.3f,1.0f};


	  glEnable(GL_COLOR_MATERIAL);
	  glEnable(GL_DEPTH_TEST);
	  glEnable(GL_LIGHTING);
	  glEnable(GL_LIGHT0);
	  glEnable(GL_LIGHT1);
	  glEnable(GL_LIGHT2);
	  glEnable(GL_DEPTH);

	  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);


	  lightPosition[0] = x;
	  lightPosition[1] = y;
      lightPosition[2] = z;

	  glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	  glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLight);

 	  glLightfv(GL_LIGHT1, GL_POSITION, lightPosition2);
 	  glLightfv(GL_LIGHT1, GL_AMBIENT, whiteLight2);
// 
	  glLightfv(GL_LIGHT2, GL_POSITION, lightPosition3);
	  glLightfv(GL_LIGHT2, GL_DIFFUSE, whiteLight);  

	  	
	  glEnable (GL_BLEND); 
	  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
  }

  void drawArrowTargetDir(cyclone::Vector3 pos, cyclone::Vector3 targetDirection) {
	  // Arrow parameters
	  float arrowLength = 12.0f;
	  float arrowHeadHeight = 2.0f;
	  float arrowHeadRadius = 1.2f;

	  glColor3f(0.7f, 0.7f, 0);

	  glPushMatrix();

	  // Translate to the position
	  glTranslatef(pos.x, pos.y, pos.z);

	  // Rotate the arrow to point in the desired direction
	  cyclone::Vector3 defaultDirection(1, 0, 0);
	  cyclone::Vector3 rotationAxis = defaultDirection % targetDirection; // % is the cross product
	  float rotationAngle = acos(defaultDirection * targetDirection); // * is the dot product

	  // Convert the rotation angle from radians to degrees
	  rotationAngle = rotationAngle * 180.0 / 3.141592653589793;

	  // Apply the rotation
	  glRotatef(rotationAngle, rotationAxis.x, rotationAxis.y, rotationAxis.z);

	  // Draw arrow body (as a line along x-axis)
	  glLineWidth(20.0f);  // Make the arrow body thicker
	  glBegin(GL_LINES);
	  glVertex3f(0, 0, 0);
	  glVertex3f(arrowLength - arrowHeadHeight, 0, 0);
	  glEnd();

	  // Draw arrow head (as a cone)
	  glTranslatef(arrowLength - arrowHeadHeight, 0, 0);
	  glRotatef(90, 0, 1, 0); // Rotate the cone so its base lies in the XZ plane
	  glutSolidCone(arrowHeadRadius, arrowHeadHeight, 10, 2);

	  glPopMatrix();
	  glLineWidth(1);
  }

  void drawArrowTargetPos(cyclone::Vector3 pos, cyclone::Vector3 targetPos) {
	  // Arrow parameters
	  float arrowLength = 5.0f;
	  float arrowHeadHeight = 0.5f;
	  float arrowHeadRadius = 0.3f;
	  cyclone::Vector3 direction = (targetPos - pos).unit();

	  glColor3f(0.7f, 0.7f, 0);

	  glPushMatrix();
	  // Translate to the position
	  glTranslatef(pos.x, pos.y, pos.z);

	  // Rotate the arrow to point in the desired direction
	  // We find the rotation angle and axis using the cross product between the 
	  // default direction (1, 0, 0) and the target direction
	  cyclone::Vector3 defaultDirection(1, 0, 0);
	  cyclone::Vector3 rotationAxis = defaultDirection % direction; // % is the cross product
	  float rotationAngle = acos(defaultDirection * direction); // * is the dot product
	  // Convert the rotation angle from radians to degrees
	  rotationAngle = rotationAngle * 180.0 / 3.141592653589793;
	  // Apply the rotation
	  glRotatef(rotationAngle, rotationAxis.x, rotationAxis.y, rotationAxis.z);

	  // Draw arrow body (as a line along x-axis)
	  glLineWidth(20.0f);
	  glBegin(GL_LINES);
	  glVertex3f(0, 0, 0);
	  glVertex3f(arrowLength - arrowHeadHeight, 0, 0);
	  glEnd();

	  // Draw arrow head (as a cone or a solid sphere)
	  glTranslatef(arrowLength - arrowHeadHeight, 0, 0);
	  glRotatef(90, 0, 1, 0);
	  glutSolidCone(arrowHeadRadius, arrowHeadHeight, 10, 2);

	  glPopMatrix();
	  glLineWidth(1);
  }

  void drawEdges(cyclone::Vector3 pos, cyclone::Vector3 size)
  {
	  setupFloor();

	  glEnable(GL_LIGHTING);
	  glPushMatrix();
	  glColor3f(0.04, 0.20, 0.00);
	  glTranslatef(pos.x, pos.y, pos.z);
	  drawCube(size.x, size.y, size.z);
	  glPopMatrix();
  }

void MyGlWindow::drawStuff()
{
   polygonf( 4, 20., 0.,-25.,  20., 0., 25.,  20., 30., 25.,  20., 30., -25.);
}

//==========================================================================
void MyGlWindow::draw()
//==========================================================================
{
  glViewport(0,0,w(),h());

	// clear the window, be sure to clear the Z-Buffer too
  glClearColor(0.1f,0.1f,0.1f,1.0f);		// background should be blue


  glClearStencil(0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  glEnable(GL_DEPTH);

  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

  
  
  // now draw the ground plane
  setProjection();
  //setupFloor();

  /*glPushMatrix();
	drawFloor(200,20);
  glPopMatrix();*/


  setupLight(m_viewer->getViewPoint().x,m_viewer->getViewPoint().y,m_viewer->getViewPoint().z);

    
 // Add a sphere to the scene.
//Draw axises
  /*glLineWidth(3.0f);
  glBegin(GL_LINES);
  glColor3f(1,0,0);

  glVertex3f(0.0f,0.1f,0.0f);
  glVertex3f(0.0f,100.0f,0.0f);

  glColor3f(0.0f,11.0f,0.0f);

  glVertex3d(0.0f,0.1f,0.0f);
  glVertex3d(100.0f,0.1,0.0f);

  glColor3f(0.0f,0.0f,1.0f);

  glVertex3f(0.0f,0.1f,0.0f);
  glVertex3f(0.0f,0.1f,100.0f);
  glEnd();
  glLineWidth(1.0f);*/

 for (unsigned int i = 0; i < m_movers.size(); i++) {
	//draw shadow
	setupShadows();
	m_movers[i]->draw(1);
	unsetupShadows();
	
	glEnable(GL_LIGHTING);
	
	//draw objects
	glPushMatrix();
	glLoadName(i + 1);
	m_movers[i]->draw(0);
	glPopMatrix();
 }

  drawArrowTargetDir(m_movers[0]->m_particle->getPosition(), cyclone::Vector3(1, 0, 0).unit());

  setupShadows();
  m_moverConnection->draw(1, m_movers.size());
  unsetupShadows();

  glEnable(GL_LIGHTING);
  
  glPushMatrix();
  m_moverConnection->draw(0, m_movers.size());
  glPopMatrix();


  glPushMatrix();
  // Set position and scale for the black hole
  glTranslatef(1, 1, 1);
  glScalef(1, 1, 1);

  glPopMatrix();

  drawEdges(cyclone::Vector3(0, -0.5f, 0), cyclone::Vector3(80, 1, 10));
  drawEdges(cyclone::Vector3(0, -0.5f, 97), cyclone::Vector3(80, 1, 10));
  drawEdges(cyclone::Vector3(0, -0.5f, -97), cyclone::Vector3(80, 1, 10));
  drawEdges(cyclone::Vector3(0, -0.5f, 48.5), cyclone::Vector3(104, 1, 87));
  drawEdges(cyclone::Vector3(0, -0.5f, -48.5), cyclone::Vector3(104, 1, 87));
  drawEdges(cyclone::Vector3(52.5, 10, 0), cyclone::Vector3(5, 20, 210));
  drawEdges(cyclone::Vector3(-52.5, 10, 0), cyclone::Vector3(5, 20, 210));
  drawEdges(cyclone::Vector3(0, 10, 102.5), cyclone::Vector3(100, 20, 5));
  drawEdges(cyclone::Vector3(0, 10, -102.5), cyclone::Vector3(100, 20, 5));

  //glutSwapBuffers();

  /////////////////////////
  putText("mksung", 0, 0, 1, 1, 0);

  setProjection();

 //glEnable(GL_COLOR_MATERIAL);
}

void MyGlWindow::drawBridge(int shadow)
{
	glLineWidth(3.0);

	if (shadow)
		glColor3f(0.2, 0.2, 0.2);
	else
		glColor3f(0.8, 0, 0);

	int name = 1;
	cyclone::ParticleWorld::Particles& particles = m_world->getParticles();
	for (cyclone::ParticleWorld::Particles::iterator p = particles.begin();
		p != particles.end();
		p++)
	{
		cyclone::Particle* particle = *p;
		const cyclone::Vector3& pos = particle->getPosition();
		glPushMatrix();
		glTranslatef(pos.x, pos.y, pos.z);
		if (!shadow)
			glLoadName(name);
		glutSolidSphere(0.2f, 20, 10);
		glPopMatrix();
		name++;
	}

	glBegin(GL_LINES);

	if (shadow)
		glColor3f(0.2, 0.2, 0.2);
	else
		glColor3f(0, 0, 1);
	for (unsigned i = 0; i < ROD_COUNT; i++)
	{
		cyclone::Particle** particles = rods[i].particle;
		const cyclone::Vector3& p0 = particles[0]->getPosition();
		const cyclone::Vector3& p1 = particles[1]->getPosition();
		glVertex3f(p0.x, p0.y, p0.z);
		glVertex3f(p1.x, p1.y, p1.z);
	}

	if (shadow)
		glColor3f(0.2, 0.2, 0.2);
	else
		glColor3f(0, 1, 0);
	for (unsigned i = 0; i < CABLE_COUNT; i++)
	{
		cyclone::Particle** particles = cables[i].particle;
		const cyclone::Vector3& p0 = particles[0]->getPosition();
		const cyclone::Vector3& p1 = particles[1]->getPosition();
		glVertex3f(p0.x, p0.y, p0.z);
		glVertex3f(p1.x, p1.y, p1.z);
	}

	if (shadow)
		glColor3f(0.2, 0.2, 0.2);
	else
		glColor3f(0.7f, 0.7f, 0.7f);
	for (unsigned i = 0; i < SUPPORT_COUNT; i++)
	{
		const cyclone::Vector3& p0 = supports[i].particle->getPosition();
		const cyclone::Vector3& p1 = supports[i].anchor;
		glVertex3f(p0.x, p0.y, p0.z);
		glVertex3f(p1.x, p1.y, p1.z);
	}
	glEnd();

	glLineWidth(1.0);
}

void MyGlWindow::test()
{
	for (unsigned int i = 0; i < m_movers.size(); i++) {
		m_movers[i]->~Mover();
	}
	m_moverConnection->~MoverConnection();
	m_contactGenerators.clear();
	m_world->getParticles().clear();
	m_world->getContactGenerators().clear();

	initMovers();
}

void MyGlWindow::update()
{
	TimingData::get().update();

	if (!run) return;

	float duration = (float)TimingData::get().lastFrameDuration * 0.003f;
	
	m_world->runPhysics(duration);

	//----------------- OLD CODE -------------------
	//for (unsigned int i = 0; i < m_movers.size(); i++) {
	//	if (m_movers[i]) {
	//		m_movers[i]->update(duration);
	//	}
	//}
	//m_moverConnection->update(duration);
	//
	//unsigned limit = maxPossibleContact;
	//cyclone::ParticleContact* nextContact = m_contact; //cyclone::ParticleContact starting pointer
	//for (std::vector<cyclone::ParticleContactGenerator*>::iterator g = m_contactGenerators.begin(); g != m_contactGenerators.end(); g++)
	//{
	//	unsigned used = (*g)->addContact(nextContact, limit); //# of solved collision is saved in used
	//	limit -= used; //subtract limit by used
	//	nextContact += used; //move the pointer
	//	if (limit <= 0) break; //if nothing left, then return
	//}
	//int num = maxPossibleContact - limit; //how many collision are solved?
	//
	//if (num > 0)
	//{
	//	//For multiple contacts, set the max. iteration to (num) *2 
	//	m_resolver->setIterations(num * 2);
	//	m_resolver->resolveContacts(m_contact, num, duration);	//}
}


void MyGlWindow::doPick()
{
	make_current();		// since we'll need to do some GL stuff

	int mx = Fl::event_x(); // where is the mouse?
	int my = Fl::event_y();

	// get the viewport - most reliable way to turn mouse coords into GL coords
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	// set up the pick matrix on the stack - remember, FlTk is
	// upside down!
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPickMatrix((double)mx, (double)(viewport[3] - my), 5, 5, viewport);

	// now set up the projection
	setProjection(false);

	// now draw the objects - but really only see what we hit
	GLuint buf[100];
	glSelectBuffer(100, buf);
	glRenderMode(GL_SELECT);
	glInitNames();
	glPushName(0);

	// draw the cubes, loading the names as we go
	for (int i = 0; i < m_movers.size(); i++)
	{
		glLoadName(i + 1);
		m_movers[i]->draw(0);
	}
	m_moverConnection->draw(0, m_movers.size());
	//drawBridge(0);

	// go back to drawing mode, and see how picking did
	int hits = glRenderMode(GL_RENDER);
	if (hits) {
		// warning; this just grabs the first object hit - if there
		// are multiple objects, you really want to pick the closest
		// one - see the OpenGL manual 
		// remember: we load names that are one more than the index
		selected = buf[3] - 1;
	}
	//else {
	//	selected = -1;
	//}
	//printf("Selected Cube %d\n", selectedCube);
}



void MyGlWindow::setProjection(int clearProjection)
//==========================================================================
{
  glMatrixMode(GL_PROJECTION);
  glViewport(0,0,w(),h());
  if (clearProjection)
	glLoadIdentity();
  // compute the aspect ratio so we don't distort things
  double aspect = ((double) w()) / ((double) h());
  gluPerspective(fieldOfView, aspect, 1, 1000);

  // put the camera where we want it to be
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  // use the transformation in the ArcBall

  gluLookAt( 
	  m_viewer->getViewPoint().x, m_viewer->getViewPoint().y, m_viewer->getViewPoint().z, 
	  m_viewer->getViewCenter().x, m_viewer->getViewCenter().y, m_viewer->getViewCenter().z, 
	  m_viewer->getUpVector().x, m_viewer->getUpVector().y, m_viewer->getUpVector().z
	  );
  
  glDisable (GL_BLEND); 
}

static int last_push;
int m_pressedMouseButton;
int m_lastMouseX;
int m_lastMouseY;

int MyGlWindow::handle(int e)
//==========================================================================
{

  switch(e) {
  case FL_SHOW:		// you must handle this, or not be seen!
	  show();
	  return 1;
  case FL_PUSH:
	  {
		  m_pressedMouseButton = Fl::event_button();
		  m_lastMouseX = Fl::event_x();
		  m_lastMouseY = Fl::event_y();

		 if (m_pressedMouseButton == 1) {
			 doPick();
			 if (selected >= 0) {
				 std::cout << "picked" << std::endl;
				 //p1selected = m_particleArray[selected]->getPosition();
				 p1selected = getSelectedBall(selected)->m_particle->getPosition();
			 }
		 }
		 damage(1);
		 return 1;
	  }
	  break;
  case FL_RELEASE:
	  m_pressedMouseButton = -1;
	  if (selected != -1)
	  {
		  //cyclone::Vector3 p2selected = m_particleArray[selected]->getPosition();
		  cyclone::Vector3 p2selected = getSelectedBall(selected)->m_particle->getPosition();
		  
		  cyclone::Vector3 newVelocity(p2selected.x - p1selected.x, p2selected.y - p1selected.y, p2selected.z - p1selected.z);
		  
		  //m_particleArray[selected]->setVelocity(newVelocity);
		  getSelectedBall(selected)->m_particle->setVelocity(newVelocity);

		  //####### change particle velocity
		  //prevPoint = cyclone::Vector3();
		  //#######

		  selected = -1;

		  run = 1;
		  ui->value(1);
	  }
	  damage(1);
	  return 1;
  case FL_DRAG: // if the user drags the mouse
	  {
		  if (selected >= 0 && m_pressedMouseButton == 1) {
			  double r1x, r1y, r1z, r2x, r2y, r2z;
			  getMouseLine(r1x, r1y, r1z, r2x, r2y, r2z);

			  double rx, ry, rz;
			  mousePoleGo(r1x, r1y, r1z, r2x, r2y, r2z,
				  //static_cast<double>(m_particleArray[selected]->getPosition().x),
				  //static_cast<double>(m_particleArray[selected]->getPosition().y),
				  //static_cast<double>(m_particleArray[selected]->getPosition().z),
				  static_cast<double>(getSelectedBall(selected)->m_particle->getPosition().x),
				  static_cast<double>(getSelectedBall(selected)->m_particle->getPosition().y),
				  static_cast<double>(getSelectedBall(selected)->m_particle->getPosition().z),
				  rx, ry, rz,
				  (Fl::event_state() & FL_CTRL) != 0);
			  damage(1);

			  //####### change particle velocity
			  //cyclone::Vector3 v(rx, ry, rz);
			  //if (prevPoint.magnitude() > 0)
				 // //m_particleArray[selected]->setVelocity((v - prevPoint) * 40.0);
				 // getSelectedBall(selected)->m_particle->setVelocity((v - prevPoint) * 40.0);
			  //prevPoint.x = rx;
			  //prevPoint.y = ry;
			  //prevPoint.z = rz;
			  //#######

			  getSelectedBall(selected)->m_particle->setPosition(rx, ry, rz);
			  //m_particleArray[selected]->setPosition(rx, ry, rz);

		  } else {
		      float fractionChangeX = static_cast<float>(Fl::event_x() - m_lastMouseX) / static_cast<float>(this->w());
		      float fractionChangeY = static_cast<float>(m_lastMouseY - Fl::event_y()) / static_cast<float>(this->h());
		      
		      if( m_pressedMouseButton == 1 ) {
		      	  m_viewer->rotate( fractionChangeX, fractionChangeY );
		      } else if( m_pressedMouseButton == 2 ) {
		      	  m_viewer->zoom( fractionChangeY );
		      } else if( m_pressedMouseButton == 3 ) {
		      	  m_viewer->translate( -fractionChangeX, -fractionChangeY, (Fl::event_key(FL_Shift_L) == 0) || (Fl::event_key(FL_Shift_R) == 0));
		      } else {
		      	  std::cout << "Warning: dragging with unknown mouse button!  Nothing will be done" << std::endl;
		      }
		      
		      m_lastMouseX = Fl::event_x();
		      m_lastMouseY = Fl::event_y();
		      redraw();
		  }
	  }
	 
	  return 1;
  
  case FL_KEYBOARD:
	  return 0;

  default:
	return 0;
  }
}

Mover* MyGlWindow::getSelectedBall(int selected)
{
	if (selected >= m_movers.size())
		return m_moverConnection->m_movers[selected - m_movers.size()];
	else
		return m_movers[selected];
}

//
// get the mouse in NDC
//==========================================================================
void MyGlWindow::getMouseNDC(float& x, float& y)
//==========================================================================
{
	// notice, we put everything into doubles so we can do the math
	  float mx = (float) Fl::event_x();	// remeber where the mouse went down
	  float my = (float) Fl::event_y();

	  // we will assume that the viewport is the same as the window size
	  float wd = (float) w();
	  float hd = (float) h();

	  // remember that FlTk has Y going the wrong way!
	  my = hd-my;
	
	  x = (mx / wd) * 2.0f - 1.f;
	  y = (my / hd) * 2.0f - 1.f;
}

void MyGlWindow::testValue(float t)
{
	cyclone::Vector3 movablePos0 = m_movers[0]->m_particle->getPosition();
	cyclone::Quaternion movableRot0 = m_movers[0]->orientation;
	cyclone::Vector3 movablePos2 = m_movers[2]->m_particle->getPosition();
	cyclone::Quaternion movableRot2 = m_movers[2]->orientation;

	glm::vec3 aPos(movablePos0.x, movablePos0.y, movablePos0.z);
	glm::quat aRot(movableRot0.r, movableRot0.i, movableRot0.j, movableRot0.k);
	glm::vec3 cPos(movablePos2.x, movablePos2.y, movablePos2.z);
	glm::quat cRot(movableRot2.r, movableRot2.i, movableRot2.j, movableRot2.k);

	glm::vec3 bPos = glm::mix(aPos, cPos, t);
	glm::quat bRot = glm::slerp(aRot, cRot, t);

	m_movers[1]->m_particle->setPosition(cyclone::Vector3(bPos.x, bPos.y, bPos.z));
	m_movers[1]->orientation = cyclone::Quaternion(bRot.w, bRot.x, bRot.y, bRot.z);
}