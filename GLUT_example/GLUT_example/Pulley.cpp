#include <iostream>
#include "Box2D\Box2D.h"
#include "GL\freeglut.h"

class Pulley {
public:
	b2Body* body1 = NULL;
	b2Body* body2 = NULL;

	b2PolygonShape shape;

	float32 box_w = 2.0f;
	float32 box_h = 2.0f;

	b2Vec2 groundanchor1;
	b2Vec2 groundanchor2;

	b2Vec2 anchor1;
	b2Vec2 anchor2;

	b2PulleyJoint* m_joint;

	Pulley(float anchor1_x_, float anchor1_y_, float anchor2_x_, float anchor2_y_, b2World* world_, float length_)
	{
		m_world = world_;

		groundanchor1.Set(anchor1_x_, anchor1_y_);
		groundanchor2.Set(anchor2_x_, anchor2_y_);
		
		float dis = b2Distance(groundanchor1, groundanchor2);
		std::cout << "dis : " << dis << std::endl;
		float length = 0;
		if(length_ > dis)
			length = (length_- dis)/2;

		//b2PolygonShape shape;
		shape.SetAsBox(box_w, box_h);

		b2BodyDef bd;
		bd.type = b2_dynamicBody;
		bd.position.Set(groundanchor1.x, groundanchor1.y - length - box_h);
		body1 = m_world->CreateBody(&bd);
		body1->CreateFixture(&shape, 0.5f);

		bd.position.Set(groundanchor2.x, groundanchor2.y - length - box_h);
		body2 = m_world->CreateBody(&bd);
		body2->CreateFixture(&shape, 0.5f);

		b2PulleyJointDef pulleyDef;
		anchor1.Set(groundanchor1.x, groundanchor1.y - length);
		anchor2.Set(groundanchor2.x, groundanchor2.y - length);
		pulleyDef.Initialize(body1, body2, groundanchor1, groundanchor2, anchor1, anchor2, 1.5f);

		m_joint = (b2PulleyJoint*)m_world->CreateJoint(&pulleyDef);
	}

	
	void Display()
	{
		b2Vec2 pos1 = body1->GetPosition();
		float a1 = body1->GetAngle();

		glMatrixMode(GL_MODELVIEW);

		glPushMatrix();
		glTranslatef(pos1.x, pos1.y, 0.0f);
		glRotatef(a1, 0.0f, 0.0f, 1.0f);
		glColor3f(0.1f, 0.8, 0.2f);
		
		glLineWidth(1.0f);
		glBegin(GL_QUADS);
		for (int i = 0; i < 4; i++) {
			glVertex2f(shape.m_vertices[i].x, shape.m_vertices[i].y);
		}
		glEnd();
		glPopMatrix();

		b2Vec2 pos2 = body2->GetPosition();
		float a2 = body2->GetAngle();

		glMatrixMode(GL_MODELVIEW);

		glPushMatrix();
		glTranslatef(pos2.x, pos2.y, 0.0f);
		glRotatef(a2, 0.0f, 0.0f, 1.0f);
		glColor3f(0.1f, 0.8, 0.2f);

		glLineWidth(1.0f);
		glBegin(GL_QUADS);
		for (int i = 0; i < 4; i++) {
			glVertex2f(shape.m_vertices[i].x, shape.m_vertices[i].y);
		}
		glEnd();
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);

		glPushMatrix();
		//glTranslatef(pos.x, pos.y, 0.0f);
		//glRotatef(a, 0.0f, 0.0f, 1.0f);
		glColor3f(0.8f, 0.8f, 0.8f);

		glLineWidth(1.0f);
		glBegin(GL_LINES);
		//glVertex2d(groundanchor1.x, groundanchor1.y);
		//glVertex2d(groundanchor2.x, groundanchor2.y);
		/*glVertex2d(anchor1.x, anchor1.y);
		glVertex2d(anchor2.x, anchor2.y);*/
		glVertex2d(pos1.x, pos1.y);
		glVertex2d(groundanchor1.x, groundanchor1.y);
		glVertex2d(groundanchor1.x, groundanchor1.y);
		glVertex2d(groundanchor2.x, groundanchor2.y);
		glVertex2d(groundanchor2.x, groundanchor2.y);
		glVertex2d(pos2.x, pos2.y);
		glEnd();
		glPopMatrix();
	}

private:
	b2World* m_world;
};