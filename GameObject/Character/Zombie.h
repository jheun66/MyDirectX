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

	// �� ������Ʈ (position, rotation, scale ����)
	Transform offset;
	
	void SetCollider(Collider* collider);

	bool CheckCollision(Ray ray, Contact* contact = nullptr);
	void Pick(bool isPicked);
	bool isPicked;
	bool canMove;
private:
	float moveSpeed;
	float rotSpeed;

	// Terrain���� ���� �� �޾� ���� �뵵
	Terrain* terrain;

	Vector3 targetPos;

	// �浹ü
	Collider* collider;
};