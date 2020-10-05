#pragma once

class Maria : public ModelAnimator
{
	enum AnimState
	{
		IDLE,
		RUN,
		SLASH,
		HIT,
		DIE
	}state;

private:
	// 플레이어 보다는 느리게 이동
	float moveSpeed = 20.0f;
	float rotSpeed = 20.0f;

	float accelation = 10.0f;
	float deceleration = 3.0f;

	Vector3 velocity = { 0, 0, 0 };

	Vector3 destPos = { 0, 0, 0 };
	vector<Vector3> path;

	Collider* damagedCollider = nullptr;
	Collider* attackCollider = nullptr;
	Collider* detectCollider = nullptr;

	// 외부에서 세팅
	Terrain* terrain = nullptr;
	AStar* aStar = nullptr;

	// 플레이어(적)
	Vanguard* player = nullptr;

	float hp = 30.0f;;
	bool isDead = false;


	RenderTarget* renderTarget = nullptr;
	DepthStencil* depthStencil = nullptr;
	class Render2D* render2D = nullptr;
	SizeBuffer* sizeBuffer = nullptr;
public:
	Maria();
	~Maria();

	void PreRender();
	void Update();
	void Render();
	void PostRender();

	void OutlineRender();

	void SetTerrain(Terrain* value) { terrain = value; };
	void SetAStar(AStar* value) { aStar = value; }
	void SetPlayer(Vanguard* value) { player = value; }

	Collider* GetDamagedCollider() { return damagedCollider; }
	void Damaged(float damage);

private:
	void Input();
	void Move();
	void MovePath();
	void Rotate();

	void Slash();
	void SlashEnd();

	void SetAnimation(AnimState value);
	void SetColliders();
};