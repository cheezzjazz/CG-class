#include <iostream>
#include "GL\freeglut.h"
#include "Box2D\Box2D.h"

#include "Box.cpp"
#include "Line.cpp"
#include "Pulley.cpp"
#include "ContactListener.cpp"

#include <list>

b2World* world;

int scr_width = 640;
int scr_height = 640;

float32 g_hz = 50.0f;

int32 velocityIterations = 8;
int32 positionIterations = 3;
//float32 timeStep = 1.0f / 60.0f;

float32 timeStep = 1.0f / g_hz;

Box* box1;
Box* box2;
Box* player;

Pulley* pulley;

Line* ground;

Box* wall[3];
b2Body* water_body;
b2Body* obejct_body;
b2Fixture* water_sensor;
bool bwater_istouching[2];
b2PolygonShape water_shape;
b2PolygonShape object_shape;

ContactListener myContactListener;

bool bstart = false;
void Dokeyboard(unsigned char key, int x, int y)
{
	std::cout << "Input key : " << key << "\n" << std::endl;
	switch (key)
	{
	case 'w':
		//box2->body->ApplyForce(b2Vec2(0.0f, 200.0f), b2Vec2(6.0f, 9.0f), true);
		player->body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, 20.0f), true);
		break;
	case 'a':
		player->body->ApplyLinearImpulseToCenter(b2Vec2(-20.0f, 0.0f), true);
		break;
	case 'd':
		player->body->ApplyLinearImpulseToCenter(b2Vec2(20.0f, 0.0f), true);
		break;
	case 's' :
		bstart = true;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void Render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluOrtho2D(-25.0f, 25.0f, -5.0f, 45.0f);

	//
	//if(box1 != NULL)
	//	box1->display();
	//if(box2 != NULL)
	//	box2->display();
	//if (player != NULL)
	//	player->display();
	if(ground != NULL)
		ground->display();
	if (pulley != NULL)
		pulley->Display();
	/*for (int i = 0; i < 3; i++)
	{
		if (wall[i] != NULL)
			wall[i]->display();
	}*/
	//
	////object
	//b2Vec2 posA = obejct_body->GetPosition();
	//float aA = obejct_body->GetAngle();

	//glMatrixMode(GL_MODELVIEW);

	//glPushMatrix();
	//glTranslatef(posA.x, posA.y, 0.0f);
	//glRotatef(aA, 0.0f, 0.0f, 1.0f);
	//glColor3f(0.5f, 0.8f, 0.8f);

	//glBegin(GL_QUADS);
	//for (int i = 0; i < 4; i++) {
	//	glVertex2f(object_shape.m_vertices[i].x, object_shape.m_vertices[i].y);
	//}
	//glEnd();
	//glPopMatrix();
	////object

	////water
	//b2Vec2 pos = water_body->GetPosition();
	//float a = water_body->GetAngle();

	//glMatrixMode(GL_MODELVIEW);

	//glPushMatrix();
	//glTranslatef(pos.x, pos.y, 0.0f);
	//glRotatef(a, 0.0f, 0.0f, 1.0f);
	//glColor3f(0.1f, 0.3f, 0.6f);

	//glBegin(GL_QUADS);
	//for (int i = 0; i < 4; i++) {
	//	glVertex2f(water_shape.m_vertices[i].x, water_shape.m_vertices[i].y);
	//}
	//glEnd();
	//glPopMatrix();
	////water
	glutSwapBuffers();
}
bool previstouching = false;
void Update(int value)
{
	if (bstart)
	{
		world->Step(timeStep, velocityIterations, positionIterations);

		//
		bool* touching = bwater_istouching;
		static float mass;
		if (*touching == true)
		{
			previstouching = *touching;
			
			//if (previstouching == true)
			{
				//std::cout << "touching!! " << mass << std::endl;

				b2Body* body = obejct_body;
				b2Body* ground = water_body;
				b2PolygonShape* ground_Shape = (b2PolygonShape*)water_sensor->GetShape();
				b2Vec2 center = ground->GetWorldPoint(ground_Shape->m_centroid);
				//std::cout << "x:" << center.x << ", y:" << center.y << std::endl;
				center.Set(center.x, 2.0f*center.y-1.0f);
				b2Vec2 position = body->GetPosition();
				b2Vec2 distance = center - position;
				float Length = 7.0f*distance.Length();
				if (distance.LengthSquared() >FLT_EPSILON * FLT_EPSILON)
				{
					//std::cout << "dd:" << FLT_EPSILON * FLT_EPSILON;
					distance.Normalize();
					//mass = 1.5f*water_body->GetFixtureList()->GetDensity();
					b2Vec2 Force = Length *distance;//20.0f* distance;
					float angularDrag = body->GetAngularVelocity();
					body->ApplyTorque(angularDrag, false);
					mass = 20.0f*distance.Length();
					
					if (position != center)
					{
						std::cout << "leng:" << Length << std::endl;
						if (distance.y > 0)
						{
							std::cout << "x : " << Force.x;
							std::cout << "y : " << Force.y << std::endl;
							body->ApplyForce(Force, position, false);
	

						}
						else
						{
							std::cout << "?" << std::endl;
							std::cout << "x : " << Force.x;
							std::cout << "y : " << Force.y << std::endl;
							Force.Set(Force.x, Force.y*Length/2);
							//b2Vec2 force = body->GetLinearVelocity();
							//body->SetLinearVelocity(b2Vec2(force.x, force.y / Length));
							body->ApplyForce(Force, position, false);
						}
					}
						//
				}

			}

				/*std::cout << "x:" << center.x << " , y:" << center.y << std::endl;
				
				b2Vec2 buoyancyForce = mass * -1 * world->GetGravity();
				body->ApplyForceToCenter(buoyancyForce, true);*/
			
		}
		else
		{
			if (previstouching == true)
			{
				previstouching = *touching;

				
				b2Body* body = obejct_body;
				b2Body* ground = water_body;
				b2PolygonShape* ground_Shape = (b2PolygonShape*)water_sensor->GetShape();
				b2Vec2 center = ground->GetWorldPoint(ground_Shape->m_centroid);
				//std::cout << "x:" << center.x << ", y:" << center.y << std::endl;
				center.Set(center.x, 2.0f*center.y - 1.0f);
				b2Vec2 position = body->GetPosition();
				b2Vec2 distance = center - position;
				b2Vec2 Force = body->GetLinearVelocity();
				if (Force.y > 0 && (position != distance))
					Force.Set(Force.x, -Force.y/2);
					//body->SetLinearVelocity(b2Vec2(Force.x, -Force.y));
				//b2Vec2 Force = body->GetLinearVelocity();
				
				//body->ApplyForce(Force, position, true);
			}
		}


	}
	glutPostRedisplay();
	glutTimerFunc(20, Update, 0);
}

void Reshape(int _width, int _height)
{
	scr_width = _width;
	scr_height = _height;
	glViewport(0, 0, _width, _height);
}

void Setup()
{
	b2Vec2 gravity;
	gravity.Set(0.0f, -10.0f);
	
	world = new b2World(gravity);

	//world->SetContactListener(&myContactListener);

	ground = new Line(b2Vec2(-24.0f, 0.0f), b2Vec2(24.0f, 0.0f), world);
	
	//box1 = new Box(0.0f, 9.0f, 1.0f, 1.0f, true, world);
	//box1->setColor(0.8f, 0.1f, 0.1f);
	//box2 = new Box(0.0f, 9.0f, 8.0f, 1.0f, false, world);
	//box2->setColor(0.1f, 0.1f, 0.8f);
	//player = new Box(-20.0f, 1.0f, 1.0f, 1.0f, false, world);
	//player->setColor(0.1f, 8.0f, 0.2f);

	/*b2RevoluteJointDef jdf;
	jdf.bodyA = box1->body;
	jdf.bodyB = box2->body;
	jdf.enableMotor = true;
	jdf.motorSpeed = 100000.0f;
	jdf.collideConnected = true;
	jdf.localAnchorA.Set(0.0f, 0.0f);
	jdf.localAnchorB.Set(0.0f, 0.0f);
	b2RevoluteJoint* joint = (b2RevoluteJoint*)world->CreateJoint(&jdf);*/

	pulley = new Pulley(-10.0f,35.0f,10.0f,35.0f,world, 40);
	
	//wall[0] = new Box(0.0f, 0.5f, 11.0f, 0.5f, true, world);
	//wall[1] = new Box(-11.0f, 11.0f, 0.5f, 11.0f, true, world);
	//wall[2] = new Box(11.0f, 11.0f, 0.5f, 11.0f, true, world);

	//water_shape.SetAsBox(10.5f, 10.5f);
	//b2BodyDef bd;
	////bd.type = b2_staticBody;//b2_dynamicBody;
	//bd.position.Set(0.0f, 11.5f);
	//water_body = world->CreateBody(&bd);
	//	
	//b2FixtureDef fd;
	//fd.shape = &water_shape;
	//fd.density = 0.9f;
	//fd.isSensor = true;
	//water_sensor = water_body->CreateFixture(&fd);

	//// Step1 : define body
	//b2BodyDef bd2;
	//bd2.type = b2_dynamicBody;
	//bd2.position.Set(0.0f, 25.0f);
	//bd2.userData = bwater_istouching;
	//bwater_istouching[0] = false;
	//// Step2 : create body
	//obejct_body = world->CreateBody(&bd2);
	//// Step3 : crate shape
	////b2PolygonShape shape;
	//object_shape.SetAsBox(1.0f, 1.0f);
	//// Step4 : create Fixture
	//b2FixtureDef fd2;
	//fd2.shape = &object_shape;
	//fd2.density = 0.2f;
	//// Step5 : Attach shape to body with fixture
	//obejct_body->CreateFixture(&fd2);
	////box1 = new Box(0.0f, 25.0f, 1.0f, 1.0f, false, world);
}

void Close()
{
	delete world;
	world = NULL;
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
	return 0;
}