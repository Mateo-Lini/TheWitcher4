#pragma once

#include <vector>
#include "Scene.h"
#include "PhysicsPlaygroundListener.h"

class PhysicsPlayground : public Scene
{
public:
	PhysicsPlayground(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void MakePlatform(std::string fileName, float32 x, float32 y, int fx, int fy);

	void MakeRamp(std::string fileName, float32 x, float32 y, int fx, int fy, float rotation);

	void MakeEnemy(std::string fileName, float32 x, float32 y, int fx, int fy);

	void Update() override;

	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;
	
protected:
	PhysicsPlaygroundListener listener;
	int ball = 0;
	int bomb = 0;
	int explosion = 0;
	int sword = 0;

	int witcher = 0;
	int witcherHP = 0;

	std::vector<int> enemy;
	std::vector<int> enemyHP;

	int village = 0;

	int camera = 0;

	int directionMod = 1;

	double dashTime = 0.f;
	bool dashing = false;

	bool facingRight = true;
	bool facingLeft = false;

	double swordTime = 0.f;
	bool swordSwinging = false;

	double bombTime = 0.f;
	bool bombThrown = false;
	bool bombExploded = false;

};
