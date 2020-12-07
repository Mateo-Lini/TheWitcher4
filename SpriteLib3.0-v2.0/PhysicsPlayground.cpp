#include "PhysicsPlayground.h"
#include "Utilities.h"

#include <iostream>
#include <random>
#include <string>

PhysicsPlayground::PhysicsPlayground(std::string name)
	: Scene(name)
{
	//No gravity this is a top down scene
	m_gravity = b2Vec2(0.f, -98.f);
	m_physicsWorld->SetGravity(m_gravity);

	m_physicsWorld->SetContactListener(&listener);
}

void PhysicsPlayground::InitScene(float windowWidth, float windowHeight)
{
	//Dynamically allocates the register
	m_sceneReg = new entt::registry;

	//Attach the register
	ECS::AttachRegister(m_sceneReg);

	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;

	//Setup MainCamera Entity
	{
		/*Scene::CreateCamera(m_sceneReg, vec4(-75.f, 75.f, -75.f, 75.f), -100.f, 100.f, windowWidth, windowHeight, true, true);*/

		//Creates Camera entity
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainCamera(entity, true);
		camera = entity;

		//Creates new orthographic camera
		ECS::AttachComponent<Camera>(entity);
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<HorizontalScroll>(entity);
		ECS::AttachComponent<VerticalScroll>(entity);

		vec4 temp = vec4(-115.f, 115.f, -115.f, 115.f);
		ECS::GetComponent<Camera>(entity).SetOrthoSize(temp);
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		//Attaches the camera to vert and horiz scrolls
		ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
	}
	
	// WITCHER
	{
		/*Scene::CreatePhysicsSprite(m_sceneReg, "LinkStandby", 80, 60, 1.f, vec3(0.f, 30.f, 2.f), b2_dynamicBody, 0.f, 0.f, true, true)*/

		auto entity = ECS::CreateEntity();
		ECS::SetIsMainPlayer(entity, true);
		witcher = entity;

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<CanJump>(entity);
		ECS::AttachComponent<Sword>(entity);
		ECS::AttachComponent<HP>(entity);

		//Sets up the components
		std::string fileName = "witcherLeft.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 40);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		//tempDef.position.Set(float32(4480.f), float32(650.f));
		tempDef.position.Set(float32(1560.f), float32(80.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		//tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);
		tempPhsBody = PhysicsBody(entity, tempBody, 17, tempSpr.GetHeight(), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);

		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(true);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody.SetGravityScale(1.4f);

		// HP BAR

		entity = ECS::CreateEntity();
		witcherHP = entity;

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

	}

	// VILLAGE
	{
		/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
		auto entity = ECS::CreateEntity();
		village = entity;

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "village.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 450, 180);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(390.f, 37.f, 0.f));
	}


	//Setup cave rubble 
	/*{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 120, 10);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(2150.f), float32(55.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
		tempPhsBody.SetRotationAngleDeg(30.f);

	}*/

	/*
	
	MONSTER CAMP
	
	*/

	std::string fileName = "rocktexture.png";

	MakeRamp(fileName, 137.1f, -29.3f, 80, 10, -30.f);
	MakeRamp(fileName, 678.f, -13.f, 150, 10, 30.f);
	MakeRamp(fileName, 930.5f, -0.7f, 100, 10, -30.f);
	MakeRamp(fileName, 1337.f, 0.f, 150, 10, 20.f);
	MakeRamp(fileName, 1030.f, 180.f, 50, 10, 20.f);
	MakeRamp(fileName, 2263.f, 120.f, 150, 10, 30.f);
	MakeRamp(fileName, 2153.f, -3.8f, 120, 10, -30.f);
	MakeRamp(fileName, 2563.f, -71.f, 150, 10, -30.f);
	MakeRamp(fileName, 2987.f, -145.7f, 150, 10, -30.f);
	MakeRamp(fileName, 3100.f, -35.f, 500, 10, 90.f);
	MakeRamp(fileName, 2600.f, -245.f, 80, 10, 90.f);
	MakeRamp(fileName, 2430.f, -220.f, 250, 10, 90.f);
	MakeRamp(fileName, 2650.f, -380.f, 450, 10, -10.f);
	MakeRamp(fileName, 3015.f, -350.f, 150, 10, 90.f);
	MakeRamp(fileName, 2414.f, 207.f, 200, 10, 30.f);
	MakeRamp(fileName, 2650.f, 343.f, 450, 10, 30.f);
	MakeRamp(fileName, 3150.f, 517.f, 250, 10, 30.f);
	MakeRamp(fileName, 3606.f, 508.1f, 250, 10, 145.f);

	MakePlatform(fileName, -790.f, -45.f, 10, 150);
	MakePlatform(fileName, -420.f, -120.f, 750, 10);
	MakePlatform(fileName, -400.f, -100.f, 30, 30);
	MakePlatform(fileName, -150.f, -35.f, 100, 10);
	MakePlatform(fileName, -275.f, -60.f, 80, 10);
	MakePlatform(fileName, -40.f, -70.f, 10, 110);
	MakePlatform(fileName, 30.f, -10.f, 150, 10);
	MakePlatform(fileName, 240.f, -50.f, 150, 10);
	MakePlatform(fileName, 390.f, -50.f, 150, 10);
	MakePlatform(fileName, 540.f, -50.f, 150, 10);
	MakePlatform(fileName, 815.f, 23.6f, 150, 10);
	MakePlatform(fileName, 1045.f, -26.f, 150, 10);
	MakePlatform(fileName, 1195.f, -26.f, 150, 10);
	MakePlatform(fileName, 1480.f, 25.2f, 150, 10);
	MakePlatform(fileName, 1655.f, 35.f, 200, 30);
	MakePlatform(fileName, 1660.f, 95.f, 10, 150);
	MakePlatform(fileName, 1280.f, 125.f, 150, 10);
	MakePlatform(fileName, 1500.f, 90.2f, 50, 10);
	MakePlatform(fileName, 1395.f, 99.9f, 50, 10);
	MakePlatform(fileName, 1088.f, 100.f, 90, 10);
	MakePlatform(fileName, 1010.f, 120.f, 50, 10);
	MakePlatform(fileName, 940.f, 140.f, 50, 10);
	MakePlatform(fileName, 1082.f, 188.f, 60, 10);
	MakePlatform(fileName, 1200.f, 210.f, 50, 10);
	MakePlatform(fileName, 1310.f, 218.f, 80, 10);
	MakePlatform(fileName, 1480.f, 190.f, 90, 10);
	MakePlatform(fileName, 1660.f, 175.f, 80, 10);
	MakePlatform(fileName, 1980.f, 25.2f, 250, 10);

	MakePlatform(fileName, 2275.f, -34.f, 150, 10);
	MakePlatform(fileName, 2425.f, -34.f, 150, 10);
	MakePlatform(fileName, 2700.f, -34.f, 150, 10);
	MakePlatform(fileName, 2850.f, -34.f, 150, 10);
	MakePlatform(fileName, 3000.f, -34.f, 50, 10);
	MakePlatform(fileName, 2700.f, -109.f, 150, 10);
	MakePlatform(fileName, 2850.f, -109.f, 150, 10);
	MakePlatform(fileName, 2850.f, -280.f, 500, 10);
	MakePlatform(fileName, 2700.f, -225.f, 50, 10);
	MakePlatform(fileName, 2940.f, -419.f, 150, 10);
	MakePlatform(fileName, 3490.f, 157.f, 2320, 10);
	MakePlatform(fileName, 2943.f, 455.f, 200, 10);
	MakePlatform(fileName, 2943.f, 455.f, 200, 10);
	MakePlatform(fileName, 3381.f, 578.9f, 250, 10);
	MakePlatform(fileName, 3930.f, 438.f, 450, 10);
	MakePlatform(fileName, 4260.f, 500.f, 250, 10);
	MakePlatform(fileName, 4469.2f, 562.f, 205, 10);
	MakePlatform(fileName, 4620.2f, 681.9f, 10, 250);
	MakePlatform(fileName, 3671.f, 578.9f, 110, 10);
	MakePlatform(fileName, 3850.2f, 561.9f, 110, 10);
	MakePlatform(fileName, 4020.2f, 578.9f, 110, 10);
	MakePlatform(fileName, 4200.2f, 578.9f, 120, 10);
	MakePlatform(fileName, 4571.2f, 561.9f, 100, 10);
	MakePlatform(fileName, 4620.2f, 300.9f, 10, 1000);
	MakePlatform(fileName, 4569.2f, 437.f, 10, 250);

	MakeEnemy("bandit.png", 1520.f, 50.f, 30, 50);
	MakeEnemy("bandit2.png", 1100.f, 50.f, 30, 50);
	MakeEnemy("witchhag.png", 2944.f, -359.f, 85, 85);
	MakeEnemy("drowner.png", 2885.f, -250.f, 40, 70);
	MakeEnemy("drowner.png", 2945.f, -250.f, 40, 70);
	MakeEnemy("drowner.png", 3015.f, -250.f, 40, 70);

	// BOSS BANDITS 
	MakeEnemy("bandit.png", -750.f, -45.f, 90, 150);
	MakeEnemy("bandit2.png", -700.f, -45.f, 90, 150);



	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));

}

void PhysicsPlayground::Update() {

	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));

}

void PhysicsPlayground::KeyboardHold() {

	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());

	std::cout << player.GetPosition().x << ", " << player.GetPosition().y << "\n ";

	// Checks for Bandit Health
	for (int i = 0; i < enemy.size(); i++) {

		if (ECS::GetComponent<HP>(enemy[i]).m_health <= 0) {

			ECS::GetComponent<Sprite>(enemyHP[i]).SetTransparency(0.f);
			ECS::GetComponent<PhysicsBody>(enemy[i]).SetCollisionBit(GROUND, false);

		}

		else {

			std::string fileName = "hp" + std::to_string(ECS::GetComponent<HP>(enemy[i]).m_health) + ".png";

			int spriteHeight = ECS::GetComponent<Sprite>(enemy[i]).GetHeight();
			int spriteWidth = ECS::GetComponent<Sprite>(enemy[i]).GetWidth();

			ECS::GetComponent<Sprite>(enemyHP[i]).LoadSprite(fileName, 40 * (spriteWidth / 30), 5 * (spriteHeight / 50));
			ECS::GetComponent<Sprite>(enemyHP[i]).SetTransparency(1.f);

			b2Vec2 banditPos = ECS::GetComponent<PhysicsBody>(enemy[i]).GetPosition();

			/*fileName = "bandit.png";
			ECS::GetComponent<Sprite>(enemy[i]).LoadSprite(fileName, 30, 50);
			ECS::GetComponent<Sprite>(enemy[i]).SetTransparency(1.f);*/

			// Puts the Bandit HP bar above the bandit's head
			ECS::GetComponent<Transform>(enemyHP[i]).SetPosition(vec3(banditPos.x, banditPos.y + (spriteHeight / 2) + 10, 5.f));

		}

	}
	
	// Checks for Witcher Health
	if (ECS::GetComponent<HP>(witcher).m_health <= 0) {

	
		ECS::GetComponent<PhysicsBody>(witcher).SetPosition(b2Vec2(390.f, 0.f));
		ECS::GetComponent<HP>(witcher).m_health = 5;
		ECS::GetComponent<PhysicsBody>(witcher).SetVelocity(vec3(0.f, 0.f, 0.f));

		for (int i = 0; i < enemy.size(); i++) {
		
			ECS::GetComponent<HP>(enemy[i]).m_health = 5;
		
		}

	}
	else {

		std::string fileName = "hp" + std::to_string(ECS::GetComponent<HP>(witcher).m_health) + ".png";
		ECS::GetComponent<Sprite>(witcherHP).LoadSprite(fileName, 40, 5);
		b2Vec2 witcherPos = ECS::GetComponent<PhysicsBody>(witcher).GetPosition();
		ECS::GetComponent<Transform>(witcherHP).SetPosition(vec3(witcherPos.x, witcherPos.y + 30, 1.f));

	}


	float speed = 1.f;
	b2Vec2 vel = b2Vec2(0.f, 0.f);

	// Changes direction of sprite
	if (facingRight) {
	
		//Sets up the components
		std::string fileName = "witcherRight.png";
		ECS::GetComponent<Sprite>(witcher).LoadSprite(fileName, 40, 40);

	}
	if (facingLeft) {

		//Sets up the components
		std::string fileName = "witcherLeft.png";
		ECS::GetComponent<Sprite>(witcher).LoadSprite(fileName, 40, 40);

	}

	// Run
	if (Input::GetKey(Key::Shift))
	{
		speed *= 5.f;
	}

	// Left
	if (Input::GetKey(Key::A))
	{
		player.GetBody()->ApplyForceToCenter(b2Vec2(-400000.f * speed, 0.f), true);
		facingLeft = true;
		facingRight = false;
	}
	
	// Right
	if (Input::GetKey(Key::D))
	{
		player.GetBody()->ApplyForceToCenter(b2Vec2(400000.f * speed, 0.f), true);
		facingRight = true;
		facingLeft = false;
	}

	/*
	Dash
	*/

	// Dash Order
	// 1. Dash
	// 2. Cooldown
	// 3. Refresh Cooldown

	if (Input::GetKey(Key::E) && !dashing) {
	
		if (facingRight) {

			directionMod = 1;

		}
		if (facingLeft) {

			directionMod = -1;

		}

		dashing = true;
		ECS::GetComponent<PhysicsBody>(witcher).GetBody()->ApplyForceToCenter(b2Vec2(800000000000000.f * directionMod * speed, 0.f), true);
	
	}

	if (dashing && dashTime < 5.0) {

		dashTime += Timer::deltaTime;

	}

	else if (dashing && dashTime > 5.0) {
	
		dashing = false;
		dashTime = 0.f;

	}


	/*
	Sword
	*/

	// Sword Order
	// 1. Player Swings Sword
	// 2. Sword Swings
	// 3. Sword Stops

	// 1.
	if (Input::GetKey(Key::F) && !swordSwinging) {

		if (facingRight) {

			directionMod = 1;

		}
		if (facingLeft) {

			directionMod = -1;

		}

		swordSwinging = true;

		//Creates entity
		auto entity = ECS::CreateEntity();
		sword = entity;

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		
		std::string fileName = "sword.png";

		if (ECS::GetComponent<Sword>(witcher).changeSword)
		{
			fileName = "sword.png";
		}
		else
		{
			fileName = "sword2.png";
		}

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 9, 36);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -2000.f, 2.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(player.GetPosition().x + (10 * directionMod), player.GetPosition().y);

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight()), vec2(0.f, 0.f), false, OBJECTS, GROUND);
		tempPhsBody.SetColor(vec4(1.f, 1.f, 0.f, 0.3f));
		tempPhsBody.SetRotationAngleDeg(-45.f * directionMod);

	}
	
	// 2.
	if (swordSwinging && swordTime < 0.5) {
	
		swordTime += Timer::deltaTime;
		ECS::GetComponent<PhysicsBody>(sword).SetRotationAngleDeg(ECS::GetComponent<PhysicsBody>(sword).GetRotationAngleDeg() - 1 * directionMod);
		ECS::GetComponent<PhysicsBody>(sword).SetPosition(b2Vec2(player.GetPosition().x + (13 * directionMod), player.GetPosition().y - 3.5));
		ECS::GetComponent<Sprite>(sword).SetTransparency(1.f);

	}
	
	// 3.
	else if (swordSwinging && swordTime > 0.5) {

		swordTime = 0.f;
		swordSwinging = false;

		ECS::GetComponent<PhysicsBody>(sword).DeleteBody();
		ECS::DestroyEntity(sword);

	}

	// Change Sword
	if (Input::GetKey(Key::G) && !swordSwinging) {

		ECS::GetComponent<Sword>(witcher).changeSword = !ECS::GetComponent<Sword>(witcher).changeSword;

	}

	/*
	Potion
	*/

	// Potion Order
	// 1. Drink Potion
	// 2. Sprite Lingers
	// 3. Sprite Disappears

	/*
	// 1.
	if (Input::GetKey(Key::Q) && potions > 0 && potionTime == 0.f) {
	
		if (facingRight) {

			directionMod = 1;

		}
		if (facingLeft) {

			directionMod = -1;

		}

		auto entity = ECS::CreateEntity();
		potion = entity;

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "potion.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 7.5, 7.5);

		b2Vec2 witcherPos = ECS::GetComponent<PhysicsBody>(witcher).GetPosition();
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(witcherPos.x + (5 * directionMod), witcherPos.y + 10, 1.f));

		ECS::GetComponent<HP>(witcher).m_health = ECS::GetComponent<HP>(witcher).m_health + 2;
		
		if (ECS::GetComponent<HP>(witcher).m_health > 5) {
		
			ECS::GetComponent<HP>(witcher).m_health = 5;

		}

		potions = potions - 1;
	
	}

	// 2.
	if (potionTime > 0.f && potionTime < 0.3f) {

		potionTime += Timer::deltaTime;

	}

	// 3.
	else if (potionTime > 0.3) {

		potionTime = 0.f;
		ECS::DestroyEntity(potion);

	}
	
	*/


	/*
	Bomb
	*/

	// Bomb Order
	// 1. Player Throws Bomb
	// 2. Bomb Travels
	// 3. Bomb Explodes
	// 4. Explosion Stops
	
	// 1.
	if (Input::GetKey(Key::R) && !bombThrown) {

		if (facingRight) {

			directionMod = 1;

		}
		if (facingLeft) {

			directionMod = -1;

		}

		bombThrown = true;
		
		auto entity = ECS::CreateEntity();
		bomb = entity;

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up the components
		std::string fileName = "bomb.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 8.5, 10);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(45.f, -8.f, 3.f));


		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(player.GetPosition().x + (25 * directionMod)), float32(player.GetPosition().y));
		tempBody = m_physicsWorld->CreateBody(&tempDef);
		ECS::GetComponent<PhysicsBody>(entity) = PhysicsBody(entity, tempBody, 4.5, vec2(0.f, 0.f), false, BOMB, GROUND, 0.3f);

		// Launches Bomb
		ECS::GetComponent<PhysicsBody>(entity).GetBody()->ApplyForceToCenter(b2Vec2(800000.f * directionMod * speed, 0.f), true);
		ECS::GetComponent<PhysicsBody>(entity).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, 20000.f), true);

	}
	
	// 2.
	if (bombThrown && bombTime < 1.1f) {
	
		bombTime += Timer::deltaTime;
	
	}
	
	// 3.
	else if (bombThrown && bombTime > 1.f && bombTime < 2.0f) {

		b2Vec2 pos;

		if (!bombExploded) {

			bombExploded = true; 
			
			// Stores the position of the bomb then deletes it
			pos = ECS::GetComponent<PhysicsBody>(bomb).GetPosition();

			// Deletes the Bomb
			ECS::GetComponent<PhysicsBody>(bomb).DeleteBody();
			ECS::DestroyEntity(bomb);
			
			auto entity = ECS::CreateEntity();
			explosion = entity;

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up the components
			std::string fileName = "explosion.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 100);
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(45.f, -8.f, 3.f));

			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(pos.x, pos.y+50);
			tempBody = m_physicsWorld->CreateBody(&tempDef);
			ECS::GetComponent<PhysicsBody>(entity) = PhysicsBody(entity, tempBody, 100, 100, vec2(0.f, 0.f), false, EXPLOSION, ENEMY | ENEMY2, 0.3f);

			pos = ECS::GetComponent<PhysicsBody>(explosion).GetPosition();
		
		}

		bombTime += Timer::deltaTime;

	}
	
	// 4.
	else if (bombThrown && bombTime > 2.0f) {

		bombExploded = false;
		bombTime = 0.f;
		bombThrown = false;
		
		// Deletes the Explosion
		ECS::GetComponent<PhysicsBody>(explosion).DeleteBody();
		ECS::DestroyEntity(explosion);

	}

	/*
	Scaling
	*/
	
	//Change physics body size for circle	
	// +
	if (Input::GetKey(Key::O))
	{
		player.ScaleBody(1.3 * Timer::deltaTime, 0);
	}
	// -
	else if (Input::GetKey(Key::I))
	{
		player.ScaleBody(-1.3 * Timer::deltaTime, 0);
	}

}

void PhysicsPlayground::KeyboardDown()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& canJump = ECS::GetComponent<CanJump>(MainEntities::MainPlayer());

	if (Input::GetKeyDown(Key::T))
	{
		PhysicsBody::SetDraw(!PhysicsBody::GetDraw());
	}
	if (canJump.m_canJump)
	{
		if (Input::GetKeyDown(Key::Space))
		{
			player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, 200500.f), true);
			canJump.m_canJump = false;
		}
	}

}

void PhysicsPlayground::KeyboardUp()
{
	

}

void PhysicsPlayground::MakePlatform(std::string fileName, float32 x, float32 y, int fx, int fy) {

	auto entity = ECS::CreateEntity();

	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);

	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, fx, fy);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	float shrinkX = 0.f;
	float shrinkY = 0.f;
	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_staticBody;
	tempDef.position.Set(float32(x), float32(y));

	tempBody = m_physicsWorld->CreateBody(&tempDef);

	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
		float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS);
	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

}

void PhysicsPlayground::MakeRamp(std::string fileName, float32 x, float32 y, int fx, int fy, float rotation)
{
	
	auto entity = ECS::CreateEntity();

	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);

	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, fx, fy);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	float shrinkX = 0.f;
	float shrinkY = 0.f;
	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_staticBody;
	tempDef.position.Set(float32(x), float32(y));

	tempBody = m_physicsWorld->CreateBody(&tempDef);

	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
		float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS);
	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	tempPhsBody.SetRotationAngleDeg(rotation);

}

void PhysicsPlayground::MakeEnemy(std::string fileName, float32 x, float32 y, int fx, int fy) {

	// BANDIT

	auto entity = ECS::CreateEntity();
	enemy.push_back(entity);

	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);
	ECS::AttachComponent<Trigger*>(entity);
	ECS::AttachComponent<HP>(entity);

	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, fx, fy);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));
	ECS::GetComponent<Trigger*>(entity) = new DestroyTrigger();
	ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
	ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(sword);
	ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(bomb);

	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	float shrinkX = 0.f;
	float shrinkY = 0.f;
	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_dynamicBody;
	tempDef.position.Set(x, y);

	tempBody = m_physicsWorld->CreateBody(&tempDef);
	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, ENEMY, GROUND | BOMB | OBJECTS | PLAYER);
	tempPhsBody.SetColor(vec4(1.f, 1.f, 0.f, 0.5f));

	// HP BAR

	entity = ECS::CreateEntity();
	enemyHP.push_back(entity);

	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);

}
