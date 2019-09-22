#include <iostream>
#include "Box2D\Box2D.h"
#include "GL\freeglut.h"
#include <vector>
class ContactListener : public b2ContactListener
{
public:
	void BeginContact(b2Contact* contact)
	{
		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();

		if (fixtureA->IsSensor() && fixtureB->GetBody()->GetType() == b2_dynamicBody)
		{
			void* userData = fixtureB->GetBody()->GetUserData();
			if (userData)
			{
				bool* touching = (bool*)userData;
				*touching = true;
			}
		}
		else if (fixtureB->IsSensor() && fixtureA->GetBody()->GetType() == b2_dynamicBody)
		{
			void* userData = fixtureA->GetBody()->GetUserData();
			if (userData)
			{
				bool* touching = (bool*)userData;
				*touching = true;
			}
		}
	}

	void EndContact(b2Contact* contact)
	{
		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();

		if (fixtureA->IsSensor() && fixtureB->GetBody()->GetType() == b2_dynamicBody)
		{
			std::vector<b2Vec2> output;
			findIntersectionOfFixtures(fixtureA, fixtureB, output);
			void* userData = fixtureB->GetBody()->GetUserData();
			if (userData)
			{
				bool* touching = (bool*)userData;
				*touching = false;
			}
		}
		else if (fixtureB->IsSensor() && fixtureA->GetBody()->GetType() == b2_dynamicBody)
		{
			std::vector<b2Vec2> output;
			findIntersectionOfFixtures(fixtureA, fixtureB, output);
			void* userData = fixtureA->GetBody()->GetUserData();
			if (userData)
			{
				bool* touching = (bool*)userData;
				*touching = false;
			}
		}
	}

	bool inside(b2Vec2 cp1, b2Vec2 cp2, b2Vec2 p)
	{
		return (cp2.x - cp1.x)* (p.y - cp1.y) > (cp2.y - cp1.y)*(p.x - cp1.x);
	}

	b2Vec2 intersection(b2Vec2 cp1, b2Vec2 cp2, b2Vec2 s, b2Vec2 e)
	{
		b2Vec2 dc(cp1.x - cp2.x, cp1.y - cp2.y);
		b2Vec2 dp(s.x - e.x, s.y - e.y);
		float n1 = cp1.x*cp2.y - cp1.y*cp2.x;
		float n2 = s.x*e.y - s.y*e.x;
		float n3 = 1.0f / (dc.x*dp.y - dc.y*dp.x);

		return b2Vec2((n1*dp.x - n2*dc.x)*n3, (n1*dp.y - n2*dc.y)*n3);
	}

	bool findIntersectionOfFixtures(b2Fixture* fA, b2Fixture* fB, std::vector<b2Vec2>& outputVertices)
	{
		if (fA->GetShape()->GetType() != b2Shape::e_polygon || fB->GetShape()->GetType() != b2Shape::e_polygon)
			return false;

		b2PolygonShape* polyA = (b2PolygonShape*)fA->GetShape();
		b2PolygonShape* polyB = (b2PolygonShape*)fB->GetShape();

		for (int i = 0; i < 4; i++)
		{
			outputVertices.push_back(fA->GetBody()->GetWorldPoint(polyA->m_vertices[i]));
		}
		std::vector<b2Vec2> clipPolygon;
		for (int i = 0; i < 4; i++)
			clipPolygon.push_back(fB->GetBody()->GetWorldPoint(polyA->m_vertices[i]));

		b2Vec2 cp1 = clipPolygon[clipPolygon.size() - 1];
		for (int j = 0; j < clipPolygon.size(); j++) {
			b2Vec2 cp2 = clipPolygon[j];
			if (outputVertices.empty())
				return false;
			std::vector<b2Vec2> inputList = outputVertices;
			outputVertices.clear();
			b2Vec2 s = inputList[inputList.size() - 1]; //last on the input list
			for (int i = 0; i < inputList.size(); i++) {
				b2Vec2 e = inputList[i];
				if (inside(cp1, cp2, e)) {
					if (!inside(cp1, cp2, s)) {
						outputVertices.push_back(intersection(cp1, cp2, s, e));
					}
					outputVertices.push_back(e);
				}
				else if (inside(cp1, cp2, s)) {
					outputVertices.push_back(intersection(cp1, cp2, s, e));
				}
				s = e;
			}
			cp1 = cp2;
		}

		return !outputVertices.empty();
	}
	
};