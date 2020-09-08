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
	void SetPlayer(Transform* player) { this->player = player; }

	void SetSpeed(float value) { this->moveSpeed = value; }

	// 빈 오브젝트 (position, rotation, scale 조절)
	Transform offset;
private:
	float moveSpeed;
	float rotSpeed;

	// Terrain에서 높이 값 받아 오는 용도
	Terrain* terrain;

	Transform* player;

};