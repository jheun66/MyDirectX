#pragma once

class Zombie : public ModelAnimator
{
	enum AnimState
	{
		IDLE,
		RUN
	}state;
public:
	Zombie();
	~Zombie();

	void Update();
	void Render();

	void AutoMove();
	void SetAnimation(AnimState state);
	void SetTerrain(Terrain* terrain) { this->terrain = terrain; }
	void SetTarget(Vector3 targetPos);

	void SetSpeed(float value) { this->moveSpeed = value; }

	// 빈 오브젝트 (position, rotation, scale 조절)
	Transform offset;
	
	void SetCollider(Collider* collider);

	bool CheckCollision(Ray ray, Contact* contact = nullptr);
	void Pick(bool isPicked);
	bool isPicked;
	bool canMove;
private:
	float moveSpeed;
	float rotSpeed;

	// Terrain에서 높이 값 받아 오는 용도
	Terrain* terrain;

	Vector3 targetPos;

	// 충돌체
	Collider* collider;
};