#include <iostream>
#include "GL\freeglut.h"
#include "Box2D\Box2D.h"

int scr_width =  640;
int scr_height =  480;

b2World* world;
b2Body* ground;

b2Body* wall[3];
b2Body* prevBody[4];
b2Body* stackBody[10];
b2Body* guideBody[3];

b2EdgeShape gnd_shape;

b2RopeJointDef ropeDef;
b2Joint* rope;

b2PolygonShape boxshape;
b2PolygonShape boxshape2;
b2PolygonShape guide;


float32 g_hz = 60.0f;
int32 velocityIterations = 8;
int32 positionIterations = 3;
float32 timeStep = 1.0f / g_hz;

void Dokeyboard(unsigned char key, int x, int y)
{
	std::cout << "Input key : " << key << "\n" << std::endl;
	switch (key)
	{
	case 'w' :
		//prevBody[3]->ApplyLinearImpulseToCenter(b2Vec2(0.0f, 4000.0f), true);
		break;
	case 'a' :
		//prevBody[3]->ApplyLinearImpulseToCenter(b2Vec2(-400.0f, 0.0f), true);
		break;
	case 'd' :
		//prevBody[3]->ApplyLinearImpulseToCenter(b2Vec2(400.0f, 0.0f), true);
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
	//glOrtho();
	//gluPerspective(45.0f, (float)scr_width / (float)scr_height, 1.0f, 100.0f);
	//

	gluOrtho2D(-25.0f, 25.0f, -5.0f, 25.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	

	for (int i = 0; i < 3; i++)
	{
		glPushMatrix();
		b2Vec2 position = ground->GetPosition();
		float32 angle = ground->GetAngle();

		glTranslatef(position.x, position.y, 0.0f);
		glRotatef(angle, 0.0f, 0.0f, 1.0f);
		glColor3f(0.8f, 0.8f, 0.8f);

		glLineWidth(1.0f);
		glBegin(GL_LINES);
		glVertex2d(gnd_shape.m_vertex1.x, gnd_shape.m_vertex1.y);
		glVertex2d(gnd_shape.m_vertex2.x, gnd_shape.m_vertex2.y);
		glEnd();
		glPopMatrix();
	}

	//wall
	for (int i = 0; i < 3; i++)
	{
		glPushMatrix();
		b2Vec2 position = wall[i]->GetPosition();
		float32 angle = wall[i]->GetAngle();

		glTranslatef(position.x, position.y, 0.0f);
		glRotatef(angle, 0.0f, 0.0f, 1.0f);
		glColor3f(0.8f, 0.8f, 0.8f);

		glLineWidth(1.0f);
		glBegin(GL_LINES);
		glVertex2d(gnd_shape.m_vertex1.x, gnd_shape.m_vertex1.y);
		glVertex2d(gnd_shape.m_vertex2.x, gnd_shape.m_vertex2.y);
		glEnd();
		glPopMatrix();
	}
	//wall

	//box
	for (int i = 0; i < 4; i++)
	{
		glPushMatrix();
		b2Vec2 position = prevBody[i]->GetPosition();
		float32 angle = prevBody[i]->GetAngle();
		float camX, objX = 0.0f;
		objX = position.x;
		camX = objX + cos(80.0f);
		//gluLookAt(camX, 15.0f, 80.0f, objX, 15.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		glTranslatef(position.x, position.y, 0.0f);
		//std::cout << "x: " << position.x << ", y: " << position.y << std::endl;
		//glRotatef(angle, 0.0f, 0.0f, 1.0f);
		if(i == 3)
			glColor3f(0.8f, 0.3f, 0.3f);
		else
			glColor3f(0.5f, 0.8f, 0.8f);
		
		glBegin(GL_QUADS);
		for (int i = 0; i < 4; i++) {
			glVertex2f(boxshape.m_vertices[i].x, boxshape.m_vertices[i].y);
		}
		glEnd();
		glPopMatrix();
		
	}
	//std::cout << "" << std::endl;
	//box

	//box
	for (int i = 0; i < 10; i++)
	{
		glPushMatrix();
		b2Vec2 position = stackBody[i]->GetPosition();
		float32 angle = stackBody[i]->GetAngle();

		glTranslatef(position.x, position.y, 0.0f);
		//std::cout << "x: " << position.x << ", y: " << position.y << std::endl;
		//glRotatef(angle, 0.0f, 0.0f, 1.0f);
		glColor3f(0.5f, 0.2f, 0.2f+i*(0.1f));

		glBegin(GL_QUADS);
		for (int i = 0; i < 4; i++) {
			glVertex2f(boxshape2.m_vertices[i].x, boxshape2.m_vertices[i].y);
		}
		glEnd();
		glPopMatrix();

	}
	//box
	for (int i = 0; i < 3; i++)
	{
		glPushMatrix();
		b2Vec2 position = guideBody[i]->GetPosition();
		float32 angle = guideBody[i]->GetAngle();

		glTranslatef(position.x, position.y, 0.0f);
		//std::cout << "x: " << position.x << ", y: " << position.y << std::endl;
		//glRotatef(angle, 0.0f, 0.0f, 1.0f);
		glColor3f(0.5f, 0.8f, 0.1f);

		glBegin(GL_QUADS);
		for (int i = 0; i < 4; i++) {
			glVertex2f(guide.m_vertices[i].x, guide.m_vertices[i].y);
		}
		glEnd();
		glPopMatrix();

	}


	glutSwapBuffers();
}

void Update(int value)
{
	for(int i = 0; i < 2; i++)
		world->Step(timeStep, velocityIterations, positionIterations);
	glutPostRedisplay();
	glutTimerFunc(20, Update, 0);
}

void close()
{
	delete world;
	world = NULL;
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
		gnd_shape.Set(b2Vec2(-24.0f, 0.0f), b2Vec2(40.0f, 0.0f));
		ground->CreateFixture(&gnd_shape, 0.0f);
	}
	{
		for (int i = 0; i < 3; i++)
		{
			//wall[i] = ground;
			b2EdgeShape shape;
			switch (i)
			{
			case 0:
				shape.Set(b2Vec2(-24.0f, 0.0f), b2Vec2(-24.0f, 24.0f));
				break;
			case 1:
				shape.Set(b2Vec2(-24.0f, 24.0f), b2Vec2(24.0f, 24.0f));
				break;
			case 2:
				shape.Set(b2Vec2(24.0f, 24.0f), b2Vec2(24.0f, 0.0f));
				break;
			default:
				break;
			}
			
			b2BodyDef bd;
			b2FixtureDef fd;
			fd.shape = &shape;
			b2Body* body = world->CreateBody(&bd);
			body->CreateFixture(&fd);
			wall[i] = body;			
		}
	}
	{
		const float32 y = 15.0f;

		for (int i = 0; i < 4; i++)
		{
			prevBody[i] = ground;
			b2FixtureDef boxfd;
			b2BodyDef boxbd;
			boxbd.type = b2_dynamicBody;
			//boxbd.type = b2_staticBody;
			boxshape.SetAsBox(1.0f, 1.0f);
			boxfd.shape = &boxshape;
			boxfd.density = 100.0f;
			boxfd.restitution = 1.0f - i*0.3f;
			boxfd.filter.categoryBits = 0x0002;
			boxbd.position.Set(-3.0f + (i)*3.5f, 5.0f);
			b2Body* body = world->CreateBody(&boxbd);
			body->CreateFixture(&boxfd);
			prevBody[i] = body;
		}
	}
	{
		
		for (int i = 0; i < 10; i++)
		{
			stackBody[i] = ground;
			b2FixtureDef boxfd2;
			b2BodyDef boxbd2;
			boxbd2.type = b2_dynamicBody;
			//boxbd2.type = b2_staticBody;
			boxshape2.SetAsBox(0.5f, 0.5f);
			boxfd2.shape = &boxshape2;
			boxfd2.density = 100.0f;
			boxfd2.friction = 5.0f;
			boxbd2.position.Set(10.0f, 0.5f + i*(1.0f));
			b2Body* body = world->CreateBody(&boxbd2);
			body->CreateFixture(&boxfd2);
			stackBody[i] = body;
		}
	}
	{

		for (int i = 0; i < 3; i++)
		{
			guideBody[i] = ground;
			b2FixtureDef boxfd2;
			b2BodyDef boxbd2;
			//boxbd2.type = b2_dynamicBody;
			boxbd2.type = b2_staticBody;
			guide.SetAsBox(5.0f, 0.3f);
			boxfd2.shape = &guide;
			boxfd2.density = 100.0f;
			boxfd2.friction = 5.0f;
			boxbd2.position.Set(-20.0f + i*7.0f, 10.0f - i*(2.0f));
			b2Body* body = world->CreateBody(&boxbd2);
			body->CreateFixture(&boxfd2);
			guideBody[i] = body;
		}
	}
}

int main(int argc, char** argv)
{
	glutInitWindowSize(scr_width, scr_height);
	glutInit(&argc, argv);
	glutCreateWindow("Box2D_example");

	Setup();	

	glutKeyboardFunc(Dokeyboard);
	glutDisplayFunc(Render);
	glutReshapeFunc(Reshape);
	glutTimerFunc(20, Update, 0);
	glEnable(GL_DEPTH_TEST);

	glutMainLoop();

	close();
	return 0;
}
