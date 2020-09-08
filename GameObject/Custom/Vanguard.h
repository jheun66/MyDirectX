#pragma once

class Vanguard : public ModelAnimator
{
	enum AnimState
	{
		IDLE,
		WALK,
		RUN,
		JUMP,
		ATTACK
	}state;
public:
	Vanguard();
	~Vanguard();

	void Update();
	void Render();

	void Move();
	void Rotation();
	void Jump();
	void JumpEnd();
	void Attack();
	void AttackEnd();

	void SetAnimation(AnimState state, float speed = 1.0f, float takeTime = 1.0f);
	void SetTerrain(Terrain* terrain) { this->terrain = terrain; }

private:
	float moveSpeed;
	float acceleration;
	float maxSpeed;

	float rotSpeed;

	// Terrain에서 높이 값 받아 오는 용도
	Terrain* terrain;

	// 빈 오브젝트 (position, rotation, scale 조절)
	Transform offset;

	// 총모델
	ModelRender* mp44;



};