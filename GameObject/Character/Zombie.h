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

	// �� ������Ʈ (position, rotation, scale ����)
	Transform offset;
private:
	float moveSpeed;
	float rotSpeed;

	// Terrain���� ���� �� �޾� ���� �뵵
	Terrain* terrain;

	Transform* player;

};