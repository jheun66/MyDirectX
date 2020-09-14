#pragma once

class Vanguard : public ModelAnimator
{
	enum AnimState
	{
		IDLE,
		WALK,
		RUN,
		ATTACK
	}state;

private:
	// 마우스 이용에 사용
	float moveSpeed;
	float rotSpeed;
	float acceleration;
	float deceleration;
	Vector3 velocity;
	Vector3 destPos;

	// Terrain에서 높이 값 받아 오는 용도
	Terrain* terrain;

	// 플레이어가 캐릭터를 선택할 수 있도록
	Collider* characterCollider;
	bool isPicked = false;
	bool canMove = false;

	// 칼모델
	ModelRender* sword;
	ModelBone* rightHand;
	XMMATRIX boneWorld;
	Collider* weaponCollider;
	bool isAttack =false;

	// 상대콜라이더(별로임)
	Zombie* enemy;

public:
	Vanguard();
	~Vanguard();

	void Update();
	void Render();
	void PostRender();

	void Move();
	void Rotation();

	void AttackEnd();

	void SetAnimation(AnimState state);
	void SetTerrain(Terrain* terrain) { this->terrain = terrain; }

	void SetCollider(Collider* col);	// 캐릭터의 콜라이더
	void SetWeapon();
	void SetEnemy(ModelAnimator* model);
};