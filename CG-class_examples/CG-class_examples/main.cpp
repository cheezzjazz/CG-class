#include <iostream>
#include "GL\freeglut.h"
#include "Box2D\Box2D.h"

int scr_width =  640;
int scr_height =  640;

b2World* g_world;
b2Body* g_groundBody;
b2PolygonShape groundBox;

float32 g_hz = 60.0f;
int32 velocityIterations = 8;
int32 positionIterations = 3;
float32 timeStep = 1.0f / g_hz;

void Dokeyboard(unsigned char key, int x, int y)
{
	std::cout << "Input key : " << key << "\n" << std::endl;
	switch (key)
	{
	case GLUT_KEY_UP :
		break;

	default:
		break;
	}
	glutPostRedisplay();
}

void Render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//
	gluPerspective(45.0f, (float)scr_width / (float)scr_height, 1.0f, 100.0f);
	//
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0f, 5.0f, 20.0f, 0.0f, 5.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	glPushMatrix();
	//float lowerX = -25.0f, upperX = 25.0f, lowerY = -5.0f, upperY = 25.0f;
	//gluOrtho2D(lowerX, upperX, lowerY, upperY);
	//glOrtho(0, SCR_WIDTH, 0, SCR_HEIGHT, 0, 1);
	

	b2Vec2 position = g_groundBody->GetPosition();
	float32 angle = g_groundBody->GetAngle();
	
	glTranslatef(position.x, position.y, 0.0f);
	glRotatef(angle, 0.0f, 0.0f, 1.0f);
	glColor3f(0.8f, 0.8f, 0.8f);

	glBegin(GL_QUADS);
	for (int i = 0; i < 4; i++)
	{
		glVertex2d(groundBox.m_vertices[i].x, groundBox.m_vertices[i].y);
	}
	glEnd();
	glPopMatrix();
	
	glutSwapBuffers();
}

void Update(int value)
{	g_world->Step(timeStep, velocityIterations, positionIterations);
	glutPostRedisplay();
	glutTimerFunc(20, Update, 0);
}

//void Initworld()
//{
//	
//}

void close()
{
	delete g_world;
	g_world = NULL;
}

void Reshape(int _width, int _height)
{
	scr_width = _width;
	scr_height = _height;
	glViewport(0, 0, _width, _height);
}

int main(int argc, char** argv)
{
	glutInitWindowSize(scr_width, scr_height);
	glutInit(&argc, argv);
	glutCreateWindow("Box2D_example_RopeJoint");

	//create world
	//Initworld();

	// Define the gravity vector
	b2Vec2 gravity;
	gravity.Set(0.0f, -10.0f);

	// Construct a world object
	g_world = new b2World(gravity);

	// Define the ground body
	b2BodyDef bodyDef;
	bodyDef.position.Set(0.0f, -10.0f);
	g_groundBody = g_world->CreateBody(&bodyDef);

	
	groundBox.SetAsBox(4.0f, 10.0f);
	g_groundBody->CreateFixture(&groundBox, 0.0f);

	glutKeyboardFunc(Dokeyboard);
	glutDisplayFunc(Render);
	glutReshapeFunc(Reshape);
	glutTimerFunc(20, Update, 0);
	glEnable(GL_DEPTH_TEST);

	glutMainLoop();

	close();
	return 0;
}
