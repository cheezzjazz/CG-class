#include <iostream>
#include "Box2D\Box2D.h"
#include "GL\freeglut.h"

class Line {

public:

	b2Body* body;
	b2EdgeShape shape;

	Line(b2Vec2 beginPoint, b2Vec2 endPoint, b2World* world_)
	{
		m_world = world_;

		// Step1 : define body
		b2BodyDef bd;

		// Step2 : create body
		body = m_world->CreateBody(&bd);

		// Step3 : crate shape
		shape.Set(beginPoint, endPoint);

		// Step4 : create Fixture
		b2FixtureDef fd;
		fd.shape = &shape;

		// Step5 : Attach shape to body with fixture
		body->CreateFixture(&fd);
		//body->CreateFixture(&shape, 0.0f);
	}
	void display()
	{
		b2Vec2 pos = body->GetPosition();
		float a = body->GetAngle();

		glMatrixMode(GL_MODELVIEW);

		glPushMatrix();
		glTranslatef(pos.x, pos.y, 0.0f);
		glRotatef(a, 0.0f, 0.0f, 1.0f);
		glColor3f(0.5f, 0.8f, 0.8f);

		glLineWidth(1.0f);
		glBegin(GL_LINES);
		glVertex2d(shape.m_vertex1.x, shape.m_vertex1.y);
		glVertex2d(shape.m_vertex2.x, shape.m_vertex2.y);
		glEnd();
		glPopMatrix();

	}
private:
	b2World* m_world;
};