#include <iostream>
#include "GL\freeglut.h"
#include "Box2D\Box2D.h"
#include <math.h>

#define RADTODEG b2_pi/180
// Window screen size
int scr_width = 640;
int scr_height = 640;
// world, bodies, shapes
b2World* world;
b2Body* ground;
b2Body* box;
b2PulleyJoint* m_joint;	// Pulley Joint
b2Body* body1 = NULL;	// Body1 of Pulley joint 
b2Body* body2 = NULL;	// Body2 of Pulley joint 

b2EdgeShape gnd_shape;
b2PolygonShape boxshape;	//for Body

int32 velocityIterations = 8;	//the number of iterations for computing the impulses
int32 positionIterations = 3;	//the number of iterations for adjusting the position

float32 g_hz = 60.0f;			//frequency
float32 timeStep = 1.0f / g_hz;

bool bstart = true;

enum MyExample
{
	mygravity = 1, mypulley
};

int nExample = mygravity;


void Render()
{
	// Initialize glut
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluOrtho2D(-25.0f, 25.0f, -5.0f, 45.0f);

	//Draw ground
	if (ground != NULL)
	{
		// get position and angle by body(ground)
		b2Vec2 position = ground->GetPosition();
		float32 angle = ground->GetAngle();

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(position.x, position.y, 0.0f);	// Translation
		glRotatef(angle, 0.0f, 0.0f, 1.0f);			// Rotation
		glColor3f(0.8f, 0.8f, 0.8f);				// Set color

		//Draw the edge shape with 2 vertices
		glLineWidth(5.0f);
		glBegin(GL_LINES);
		glVertex2d(gnd_shape.m_vertex1.x, gnd_shape.m_vertex1.y);
		glVertex2d(gnd_shape.m_vertex2.x, gnd_shape.m_vertex2.y);
		glEnd();
		glPopMatrix();
	}

	switch (nExample)
	{
	case mygravity:
		//Box
		if (box != NULL)
		{
			//get position and angle by body1(box)
			b2Vec2 pos = box->GetPosition();
			float32 ang = box->GetAngle();

			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glTranslatef(pos.x, pos.y, 0.0f);	// Translation
			glRotatef(ang, 0.0f, 0.0f, 1.0f);	// Rotation
			glColor3f(0.9f, 0.2f, 0.4f);		// Set color
			
			//Draw the polygon shape with 4 vertices
			glBegin(GL_QUADS);
			for (int i = 0; i < 4; i++) {
				glVertex2f(boxshape.m_vertices[i].x, boxshape.m_vertices[i].y);
			}
			glEnd();
			glPopMatrix();
		}
		break;
	case mypulley:
		//Draw Pulleys
		if (body1 != NULL && body2 != NULL)
		{
			//Draw Pulleys
			//Body Box_1
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
				glVertex2f(boxshape.m_vertices[i].x,
					boxshape.m_vertices[i].y);
			}
			glEnd();
			glPopMatrix();
			//Body Box_2
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
				glVertex2f(boxshape.m_vertices[i].x,
					boxshape.m_vertices[i].y);
			}
			glEnd();
			glPopMatrix();

			//Draw the string
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glColor3f(0.8f, 0.8f, 0.8f);

			glLineWidth(1.0f);
			glBegin(GL_LINES);
			glVertex2d(m_joint->GetAnchorA().x, m_joint->GetAnchorA().y);
			glVertex2d(m_joint->GetGroundAnchorA().x, m_joint->GetGroundAnchorA().y);
			glVertex2d(m_joint->GetGroundAnchorA().x, m_joint->GetGroundAnchorA().y);
			glVertex2d(m_joint->GetGroundAnchorB().x, m_joint->GetGroundAnchorB().y);
			glVertex2d(m_joint->GetGroundAnchorB().x, m_joint->GetGroundAnchorB().y);
			glVertex2d(m_joint->GetAnchorB().x, m_joint->GetAnchorB().y);
			glEnd();
			glPopMatrix();
		}
		break;
	default:
		break;
	}
	
	glutSwapBuffers();
}//end of Render()


void Update(int value)
{
	if (bstart)
	{	// update the simulation
		//for (int i = 0; i < 2; i++)
			world->Step(timeStep, velocityIterations, positionIterations);
		////Print
		//b2Vec2 position = box->GetPosition();	// get position of the box body
		//std::cout << "Box position (" << position.x << ", " << position.y << ")" << std::endl;
	}
	glutPostRedisplay();
	glutTimerFunc(20, Update, 0);	//Recursive function
}


void Reshape(int _width, int _height)
{
	scr_width = _width;
	scr_height = _height;
	glViewport(0, 0, _width, _height);
}

void Setup()
{
	// Define the gravity vector
	b2Vec2 gravity;
	gravity.Set(0.0f, -10.0f);

	// Construct a world object
	world = new b2World(gravity);

	// Define the ground body
	{
		b2BodyDef gnd_bd;
		ground = world->CreateBody(&gnd_bd);
		gnd_shape.Set(b2Vec2(-40.0f, 0.0f), b2Vec2(40.0f, 0.0f));
		ground->CreateFixture(&gnd_shape, 0.0f);
	}

	switch (nExample)
	{
	case mygravity:
	{
		// Step1 : define body
		b2BodyDef boxbd;
		boxbd.type = b2_dynamicBody;
		boxbd.position.Set(0.0f, 30.0f);
		// Step2 : create body
		b2Body* body = world->CreateBody(&boxbd);
		// Step3 : create shape
		boxshape.SetAsBox(5.0f, 5.0f);
		// Step4 : create Fixture
		b2FixtureDef boxfd;
		boxfd.shape = &boxshape;
		boxfd.density = 1.0f;
		boxfd.restitution = 0.0f;
		// Step5 : Attach shape to body with fixture
		body->CreateFixture(&boxfd);
		box = body;
	}
		break;
	case mypulley:
	{
		// Joint
		// body _ box size
		float32 box_w = 2.0f;
		float32 box_h = 2.0f;
		// need ground anchor1,2 / anchor1,2
		b2Vec2 groundanchor1, groundanchor2;
		b2Vec2 anchor1, anchor2;

		//set ground position
		groundanchor1.Set(-10.0f, 35.0f);
		groundanchor2.Set(10.0f, 35.0f);

		//set length of pulleys
		float length = 40.0f;
		float Length = 0.0f;
		float distance = b2Distance(groundanchor1, groundanchor2);
		if (length > distance)
			Length = (length - distance) / 2;

		//create the shape of body
		boxshape.SetAsBox(box_w, box_h);

		//create Fixture of body
		b2BodyDef bd;				// you can reuse one bodydef for 2 body
		bd.type = b2_dynamicBody;
		bd.position.Set(groundanchor1.x, groundanchor1.y - Length - box_h);
		body1 = world->CreateBody(&bd);
		body1->CreateFixture(&boxshape, 0.5f);	//set different density of body 1

		bd.position.Set(groundanchor2.x, groundanchor2.y - Length - box_h);
		body2 = world->CreateBody(&bd);
		body2->CreateFixture(&boxshape, 0.4f); //set different density of body 2

											   // define and create pulleys joint
		b2PulleyJointDef pulleyDef;
		anchor1.Set(groundanchor1.x, groundanchor1.y - Length);
		anchor2.Set(groundanchor2.x, groundanchor2.y - Length);
		pulleyDef.Initialize(body1, body2, groundanchor1, groundanchor2, anchor1, anchor2, 1.5f);
		m_joint = (b2PulleyJoint*)world->CreateJoint(&pulleyDef);
	}
		break;
	default:
		break;
	}
	

}//end Setup()

void Dokeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'g':
		nExample = mygravity;
		Setup();
		break;
	case 'p':
		nExample = mypulley;
		Setup();
		break;
	case 's':
		bstart = true;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	// Initialize glut
	glutInitWindowSize(scr_width, scr_height);
	glutInit(&argc, argv);
	glutCreateWindow("Box2D");

	// Setting Box2D elements
	Setup();
	
	glutDisplayFunc(Render);		//Rendering
	glutReshapeFunc(Reshape);		//Reshape by window size
	glutTimerFunc(20, Update, 0);	//Update physics simulation

	glutKeyboardFunc(Dokeyboard);	//If you want to use keyborad event,
									//Activate this!
	glutMainLoop();					

	return 0;
}