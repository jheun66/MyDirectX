#pragma once

class Zombie : public ModelAnimator
{
	enum AnimState
	{
		IDLE,
		RUN,
		AGONY,
		DIE
	}state;

private:
	float moveSpeed;
	float rotSpeed;

	// Terrain에서 높이 값 받아 오는 용도
	Terrain* terrain;

	Vector3 targetPos;

	// 충돌체
	Collider* collider;

	float HP = 50.0f;
	bool isDamaged = false;
public:
	// 빈 오브젝트 (position, rotation, scale 조절)
	Transform offset;

	bool isPicked;
	bool canMove;

	bool isDead = false;
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

	void SetCollider(Collider* collider);
	Collider* GetCollider() { return collider; }

	bool CheckCollision(Ray ray, Contact* contact = nullptr);
	void Pick(bool isPicked);

	void Damaged(float damage);
	void DamagedEnd();
	void Die();
	void DieEnd();
};