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

	// Terrain���� ���� �� �޾� ���� �뵵
	Terrain* terrain;

	Vector3 targetPos;

	// �浹ü
	Collider* collider;

	float HP = 50.0f;
	bool isDamaged = false;
public:
	// �� ������Ʈ (position, rotation, scale ����)
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