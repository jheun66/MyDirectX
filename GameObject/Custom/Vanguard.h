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
public:
	Vanguard();
	~Vanguard();

	void Update();
	void Render();
	void PostRender();

	void Move();
	void Rotation();
	//void Jump();
	//void JumpEnd();
	void Attack();
	void AttackEnd();

	void SetAnimation(AnimState state, float speed = 1.0f, float takeTime = 1.0f);
	void SetTerrain(Terrain* terrain) { this->terrain = terrain; }

	void SetCollider(Collider* col);	// ĳ������ �ݶ��̴�`
	void SetWeapon();
private:
	float moveSpeed;
	float acceleration;
	float maxSpeed;

	float rotSpeed;

	// Terrain���� ���� �� �޾� ���� �뵵
	Terrain* terrain;

	// �� ������Ʈ (position, rotation, scale ����)
	Transform offset;

	// �Ѹ�
	ModelRender* sword;

	ModelBone* rightHand;
	XMMATRIX boneWorld;

	Collider* CharacterCollider;
	Collider* WeaponCollider;

};