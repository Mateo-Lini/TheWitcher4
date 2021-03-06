#include "PhysicsPlaygroundListener.h"
#include "ECS.h"

PhysicsPlaygroundListener::PhysicsPlaygroundListener()
	: b2ContactListener()
{

}

void PhysicsPlaygroundListener::BeginContact(b2Contact* contact)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	bool sensorA = fixtureA->IsSensor();
	bool sensorB = fixtureB->IsSensor();

	//if neither or both are sensors, will be false
	if ((sensorA ^ sensorB))
	{
		if (sensorA)
		{
			TriggerEnter(fixtureA);
		}
		else if (sensorB)
		{
			TriggerEnter(fixtureB);
		}
	}

	b2Filter filterA = fixtureA->GetFilterData();
	b2Filter filterB = fixtureB->GetFilterData();

	if ((filterA.categoryBits == PLAYER && filterB.categoryBits == GROUND) || (filterB.categoryBits == PLAYER && filterA.categoryBits == GROUND))
	{
		if (filterA.categoryBits == PLAYER)
		{
			ECS::GetComponent<CanJump>((int)fixtureA->GetBody()->GetUserData()).m_canJump = true;
		}
		else if (filterB.categoryBits == PLAYER)
		{
			ECS::GetComponent<CanJump>((int)fixtureB->GetBody()->GetUserData()).m_canJump = true;
		}
	}

	// BANDIT
	if ((filterA.categoryBits == ENEMY && filterB.categoryBits == EXPLOSION) || 
		(filterB.categoryBits == ENEMY && filterA.categoryBits == EXPLOSION) ||
		(filterA.categoryBits == ENEMY && filterB.categoryBits == OBJECTS) || 
		(filterB.categoryBits == ENEMY && filterA.categoryBits == OBJECTS))
	
	{

		if (filterA.categoryBits == ENEMY)
		{

			int entityId = (int)fixtureA->GetBody()->GetUserData();
			
			if (filterB.categoryBits == OBJECTS) {
			
				ECS::GetComponent<HP>(entityId).m_health = ECS::GetComponent<HP>(entityId).m_health - 1;
			
			}
			else if (filterB.categoryBits == EXPLOSION) {

				ECS::GetComponent<HP>(entityId).m_health = ECS::GetComponent<HP>(entityId).m_health - 2;

			}
			
			if (ECS::GetComponent<HP>(entityId).m_health <= 0) {
			
				std::string fileName = "banditdead.png";
				ECS::GetComponent<Sprite>(entityId).LoadSprite(fileName, 40, 40);

			}

		}
		else if (filterB.categoryBits == ENEMY)
		{
			
			int entityId = (int)fixtureB->GetBody()->GetUserData();
			
			if (filterA.categoryBits == OBJECTS) {

				ECS::GetComponent<HP>(entityId).m_health = ECS::GetComponent<HP>(entityId).m_health - 1;

			}
			else if (filterA.categoryBits == EXPLOSION) {

				ECS::GetComponent<HP>(entityId).m_health = ECS::GetComponent<HP>(entityId).m_health - 2;

			}

			if (ECS::GetComponent<HP>(entityId).m_health == 0)
			{
			
				std::string fileName = "banditdead.png";
				ECS::GetComponent<Sprite>(entityId).LoadSprite(fileName, 40, 40);

			}

		}
	}

}
 
void PhysicsPlaygroundListener::EndContact(b2Contact* contact)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	bool sensorA = fixtureA->IsSensor();
	bool sensorB = fixtureB->IsSensor();

	//if neither or both are sensors, will be false
	if ((sensorA ^ sensorB))
	{
		if (sensorA)
		{
			TriggerExit(fixtureA);
		}
		else if (sensorB)
		{
			TriggerExit(fixtureB);
		}
	}
}

void PhysicsPlaygroundListener::TriggerEnter(b2Fixture* sensor)
{
	int entity = (int)sensor->GetBody()->GetUserData();

	ECS::GetComponent<Trigger*>(entity)->OnEnter();
}

void PhysicsPlaygroundListener::TriggerExit(b2Fixture* sensor)
{
	int entity = (int)sensor->GetBody()->GetUserData();

	ECS::GetComponent<Trigger*>(entity)->OnExit();
}
