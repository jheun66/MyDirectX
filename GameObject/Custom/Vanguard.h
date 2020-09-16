#pragma once

class Vanguard : public ModelAnimator
{
	enum AnimState
	{
		IDLE,
		WALK,
		RUN,
		ATTACK,
		HIT,
		DIE
	}state;

private:
	float moveSpeed = 30.0f;
	float rotSpeed = 10.0f;

	float accelation = 10.0f;
	float deceleration = 3.0f;

	Vector3 velocity = { 0, 0, 0 };

	Vector3 destPos = { 0, 0, 0 };

	Collider* damagedCollider = nullptr;

	Terrain* terrain = nullptr;

	ModelAnimator* enemy = nullptr;

	// Ä®¸ðµ¨
	ModelRender* sword = nullptr;
	ModelBone* rightHand = nullptr;
	XMMATRIX boneWorld;
	Collider* weaponCollider = nullptr;

	float hp = 30.0f;
	bool isDead = false;

	Transform offsetTransform;
public:
	Vanguard();
	~Vanguard();

	void Update();
	void Render();
	void PostRender();

	void Move();

	//void Move();
	//void Rotation();

	void AttackEnd();
	void SetAnimation(AnimState state);

	void SetCollider();
	void SetWeapon();

	void SetTerrain(Terrain* terrain) { this->terrain = terrain; }

	void Damaged(float damage);

	Collider* GetDamagedCollider() { return damagedCollider; }
	function<ModelAnimator* ()> Check = nullptr;
};