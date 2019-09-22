#include <iostream>
#include "Box2D\Box2D.h"
#include "GL\freeglut.h"

class Box {
public:
	float x, y;
	float w, h;

	b2Body* body;
	b2PolygonShape shape;
	b2Vec3 color = b2Vec3(0.5f, 0.8f, 0.8f);

	Box()
	{
	}
	Box(float x_, float y_, float w_, float h_, bool lock, b2World* world_) 
	{
		x = x_;
		y = y_;

		w = w_;
		h = h_;
		m_world = world_;

		// Step1 : define body
		b2BodyDef bd;
		
		if(lock)
			bd.type = b2_staticBody;
		else
			bd.type = b2_dynamicBody;

		bd.position.Set(x, y);

		// Step2 : create body
		body = m_world->CreateBody(&bd);

		// Step3 : crate shape
		//b2PolygonShape shape;
		shape.SetAsBox(w, h);

		// Step4 : create Fixture
		b2FixtureDef fd;
		fd.shape = &shape;
		fd.density = 1.0f;
		fd.friction = 0.3f;
		//fd.restitution = 0.5f;

		// Step5 : Attach shape to body with fixture
		body->CreateFixture(&fd);
	}
	void setColor(float r_, float g_, float b_)
	{
		color = b2Vec3(r_, g_, b_);
	}

	void display()
	{
		b2Vec2 pos = body->GetPosition();
		float a = body->GetAngle();

		glMatrixMode(GL_MODELVIEW);

		glPushMatrix();
		glTranslatef(pos.x, pos.y, 0.0f);
		glRotatef(a, 0.0f, 0.0f, 1.0f);
		glColor3f(color.x, color.y, color.z);

		glLineWidth(1.0f);
		glBegin(GL_QUADS);
		for (int i = 0; i < 4; i++) {
			glVertex2f(shape.m_vertices[i].x, shape.m_vertices[i].y);
		}
		glEnd();
		glPopMatrix();

	}
private:
	b2World* m_world;
	
};