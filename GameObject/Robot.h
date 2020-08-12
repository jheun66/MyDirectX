#pragma once

class Sphere;
class Cube;

class Robot : public Transform
{
public:
	Robot();
	~Robot();

	void Update();
	void Render();

	void Create();

	bool isMoving = false;
	bool isAttack = false;
private:
	Sphere* head = nullptr;
	Cube* body = nullptr;
	Cube* leftArm = nullptr;
	Cube* rightArm = nullptr;
	Cube* leftLeg = nullptr;
	Cube* rightLeg = nullptr;
	
};