#pragma once

#include <vector>
#include "Scene.h"
#include "PhysicsPlaygroundListener.h"

class PhysicsPlayground : public Scene
{
public:
	PhysicsPlayground(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;

	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;

protected:
	PhysicsPlaygroundListener listener;
	int ball = 0;
	int bomb = 0;
	int sword = 0;

	double previousTime = 0.f;
	double currentTime = 0.f;

	bool facingRight = true;
	bool facingLeft = false;

	double swordTime = 0.f;
	bool swordSwinging = false;

	double bombTime = 0.f;
	bool bombThrown = false;
	bool bombExploded = false;

};
