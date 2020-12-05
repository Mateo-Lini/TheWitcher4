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
	 
	std::cout << "Collision:\n";
	std::cout << filterA.categoryBits << "\n";
	std::cout << filterB.categoryBits << "\n";

	// BANDIT 1
	if ((filterA.categoryBits == ENEMY && filterB.categoryBits == EXPLOSION) || (filterB.categoryBits == ENEMY && filterA.categoryBits == EXPLOSION) ||
		(filterA.categoryBits == ENEMY && filterB.categoryBits == OBJECTS) || (filterB.categoryBits == ENEMY && filterA.categoryBits == OBJECTS))
	{
		if (filterA.categoryBits == ENEMY)
		{
			ECS::RemoveComponent<PhysicsBody>((int)fixtureA->GetBody()->GetUserData());
			ECS::DestroyEntity((int)fixtureA->GetBody()->GetUserData());
		}
		else if (filterB.categoryBits == ENEMY)
		{
			ECS::RemoveComponent<PhysicsBody>((int)fixtureB->GetBody()->GetUserData());
			ECS::DestroyEntity((int)fixtureB->GetBody()->GetUserData());
		}
	}

	// BANDIT 2
	if ((filterA.categoryBits == ENEMY2 && filterB.categoryBits == EXPLOSION) || (filterB.categoryBits == ENEMY2 && filterA.categoryBits == EXPLOSION) ||
		(filterA.categoryBits == ENEMY2 && filterB.categoryBits == OBJECTS) || (filterB.categoryBits == ENEMY2 && filterA.categoryBits == OBJECTS))
	{
		if (filterA.categoryBits == ENEMY2)
		{
			ECS::RemoveComponent<PhysicsBody>((int)fixtureA->GetBody()->GetUserData());
			ECS::DestroyEntity((int)fixtureA->GetBody()->GetUserData());
		}
		else if (filterB.categoryBits == ENEMY2)
		{
			ECS::RemoveComponent<PhysicsBody>((int)fixtureB->GetBody()->GetUserData());
			ECS::DestroyEntity((int)fixtureB->GetBody()->GetUserData());
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
