#include "Mover.h"

Mover::Mover()
{
	size = 2;
	has_collisions = true; //false;
	m_particle = new cyclone::Particle();

	//m_particle->setPosition(5, 20, 0);  //initial pos
	//m_particle->setVelocity(0, 0, 35.0f); //initial vel
	//m_particle->setMass(2.0f); //mass
	//m_particle->setDamping(0.99f); //damping
	//m_particle->setAcceleration(0.0f, -1.0f, 0.0f);  //initial acc.

	m_particle->setPosition(5, 5, 0);  //initial pos
	m_particle->setVelocity(0, 0, 0); //initial vel
	m_particle->setMass(5.0f); //mass
	m_particle->setDamping(0.9f); //damping
	m_particle->setAcceleration(0, 0, 0);  //initial acc.

	m_gravity = new cyclone::ParticleGravity(cyclone::Vector3::GRAVITY);
	m_drag = new cyclone::ParticleDrag(0.1, 0.1);
	m_buoyancy = new cyclone::MyParticleBuoyancy(1, 1, 10, 2);

	m_forces = new cyclone::ParticleForceRegistry();
	m_forces->add(m_particle, m_gravity);
	m_forces_list.push_back(m_gravity);
	//m_forces->add(m_particle, m_drag);
	//m_forces_list.push_back(m_drag);
	//m_forces->add(m_particle, m_buoyancy);
	//m_forces_list.push_back(m_buoyancy);

	m_spring = nullptr;
}

Mover::Mover(cyclone::Vector3 position, cyclone::Vector3 velocity, float mass, float damping, int _type)
{
	type = _type;
	displayAxis = false;
	size = (type == 2) ? 0.1f : 3;
	has_collisions = true; //false;
	m_particle = new cyclone::Particle();
	color = cyclone::Vector3(0.7f, 0.7f, 0);

	//m_particle->setPosition(5, 20, 0);  //initial pos
	//m_particle->setVelocity(0, 0, 35.0f); //initial vel
	//m_particle->setMass(2.0f); //mass
	//m_particle->setDamping(0.99f); //damping
	//m_particle->setAcceleration(0.0f, -1.0f, 0.0f);  //initial acc.

	m_particle->setPosition(position);  //initial pos
	m_particle->setVelocity(velocity); //initial vel
	m_particle->setMass(mass); //mass
	m_particle->setDamping(damping); //damping
	m_particle->setAcceleration(0, 0, 0);  //initial acc.

	m_gravity = new cyclone::ParticleGravity(cyclone::Vector3::GRAVITY);
	m_drag = new cyclone::ParticleDrag(0.1, 0.1);

	m_spring = nullptr;
	m_buoyancy = nullptr;

	//m_forces = nullptr;
	m_forces = new cyclone::ParticleForceRegistry();
	if (type != 2)
	{
		m_forces->add(m_particle, m_gravity);
		m_forces_list.push_back(m_gravity);
	}
	//m_forces->add(m_particle, m_drag);
	//m_forces_list.push_back(m_drag);
}

void Mover::update(float duration)
{
	cyclone::Vector3 wind(1.0f, 0, 0);  //Wind blows in x direction
	//m_particle->addForce(wind);

	if (m_forces != nullptr)
		m_forces->updateForces(duration);
	m_particle->integrate(duration);
	//if (has_collisions) checkEdges();
}

void Mover::draw(int shadow)
{
	if (type == 0)
	{
		drawMyCube(shadow, color);
	}
	else if (type == 1 || type == 2)
	{
		drawSphere(shadow, color);
	}
	if (!shadow && displayAxis) {
		float x = m_particle->getPosition().x;
		float y = m_particle->getPosition().y;
		float z = m_particle->getPosition().z;

		glPushMatrix();

		glLineWidth(3.0f);
		glBegin(GL_LINES);
		glColor3f(1.0f, 0.0f, 0.0f);

		glVertex3f(x, y, z);
		glVertex3f(x, y + size * 2, z);

		glColor3f(0.0f, 1.0f, 0.0f);

		glVertex3f(x, y, z);
		glVertex3d(x + size * 2, y, z);

		glColor3f(0.0f, 0.0f, 1.0f);

		glVertex3f(x, y, z);
		glVertex3f(x, y, z + size * 2);
		glEnd();
		glLineWidth(1.0f);
		
		glPopMatrix();
	}
}

void Mover::drawMyCube(int shadow, cyclone::Vector3 color)
{
	cyclone::Vector3 position;
	m_particle->getPosition(&position);

	transformMatrix.setOrientationAndPos(orientation, m_particle->getPosition());
	GLfloat mat[16];
	getGLTransform(mat);

	if (!shadow)
	{
		glColor3f(color.x, color.y, color.z);
		//std::cout << position.x << " " << position.y << " " << position.z << std::endl;
	}

	glPushMatrix(); // Add this line to isolate the transformation

	glTranslatef(position.x, position.y, position.z);
	glutSolidCube(size);
	glMultMatrixf(mat);
	//glutSolidSphere(size, 30, 30);

	glPopMatrix(); // Add this line to isolate the transformation

	if (shadow)
		glColor3f(0.1f, 0.1f, 0.1f);
}

void Mover::drawSphere(int shadow, cyclone::Vector3 color)
{
	cyclone::Vector3 position;
	m_particle->getPosition(&position);

	transformMatrix.setOrientationAndPos(orientation, m_particle->getPosition());
	GLfloat mat[16];
	getGLTransform(mat);

	if (!shadow)
	{
		if (type == 2)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4f(1, 1, 1, 0);
		}
		else
		{
			glDisable(GL_BLEND);
			glColor3f(color.x, color.y, color.z);
		}
		//std::cout << position.x << " " << position.y << " " << position.z << std::endl;
	}

	glPushMatrix();

	glTranslatef(position.x, position.y, position.z);
	glutSolidSphere(size, 30, 30);
	glMultMatrixf(mat);

	glPopMatrix();

	if (shadow)
		glColor3f(0.1f, 0.1f, 0.1f);
}


void Mover::drawWater()
{
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glPushMatrix();
	glColor4f(0, 0, 1, 0.2f);
	glTranslatef(0, 5.0, 0);
	drawCube(size, 10, size);
	glPopMatrix();
}

void Mover::checkEdges()
{
	cyclone::Vector3 position;
	cyclone::Vector3 velocity;
	m_particle->getPosition(&position);
	m_particle->getVelocity(&velocity);

	if (position.y - size <= 0)
	{
		velocity = cyclone::Vector3(velocity.x, -velocity.y, velocity.z);
		position = cyclone::Vector3(position.x, size, position.z);
	}
	if (position.x + size >= 100)
	{
		velocity = cyclone::Vector3(-velocity.x, velocity.y, velocity.z);
		position = cyclone::Vector3(100 - size, position.y, position.z);
	}
	else if (position.x - size <= -100)
	{
		velocity = cyclone::Vector3(-velocity.x, velocity.y, velocity.z);
		position = cyclone::Vector3(-100 + size, position.y, position.z);
	}
	if (position.z + size >= 100)
	{
		velocity = cyclone::Vector3(velocity.x, velocity.y, -velocity.z);
		position = cyclone::Vector3(position.x, position.y, 100 - size);
	}
	else if (position.z - size <= -100)
	{
		velocity = cyclone::Vector3(velocity.x, velocity.y, -velocity.z);
		position = cyclone::Vector3(position.x, position.y, -100 + size);
	}

	m_particle->setVelocity(velocity);
	m_particle->setPosition(position);
}

void Mover::setConnection(Mover* a)
{
	m_spring = new cyclone::MySpring(a->m_particle, 20.0f, 3);
}

void Mover::setConnection(cyclone::Vector3* a)
{
	m_spring = new cyclone::MySpring(a, 5, 3);
}

void Mover::getGLTransform(float matrix[16])
{
	matrix[0] = (float)transformMatrix.data[0];
	matrix[1] = (float)transformMatrix.data[4];
	matrix[2] = (float)transformMatrix.data[8];
	matrix[3] = 0;
	matrix[4] = (float)transformMatrix.data[1];
	matrix[5] = (float)transformMatrix.data[5];
	matrix[6] = (float)transformMatrix.data[9];
	matrix[7] = 0;
	matrix[8] = (float)transformMatrix.data[2];
	matrix[9] = (float)transformMatrix.data[6];
	matrix[10] = (float)transformMatrix.data[10];
	matrix[11] = 0;
	matrix[12] = (float)transformMatrix.data[3];
	matrix[13] = (float)transformMatrix.data[7];
	matrix[14] = (float)transformMatrix.data[11];
	matrix[15] = 1;
}