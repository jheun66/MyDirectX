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
	// ���콺 �̿뿡 ���
	float moveSpeed;
	float rotSpeed;
	float acceleration;
	float deceleration;
	Vector3 velocity;
	Vector3 destPos;

	// Terrain���� ���� �� �޾� ���� �뵵
	Terrain* terrain;

	// �÷��̾ ĳ���͸� ������ �� �ֵ���
	Collider* characterCollider;
	bool isPicked = false;
	bool canMove = false;

	// Į��
	ModelRender* sword;
	ModelBone* rightHand;
	XMMATRIX boneWorld;
	Collider* weaponCollider;
	bool isAttack =false;

	// ����ݶ��̴�(������)
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

	void SetCollider(Collider* col);	// ĳ������ �ݶ��̴�
	void SetWeapon();
	void SetEnemy(ModelAnimator* model);
};