#include "framework.h"
#include <algorithm>

Vanguard::Vanguard()
	:ModelAnimator("Vanguard/Vanguard"), maxSpeed(7.5f), moveSpeed(0), acceleration(3.5f), rotSpeed(5)
{
	scale = { 0.01f, 0.01f, 0.01f };

	// ��Ƽ� �Ѳ����� ���ֱ� �ѹ���
	//ModelReader* reader = new ModelReader();
	//reader->ReadFile("ModelData/Animations/Vanguard/Idle2.fbx");
	//reader->ExportClip(0, "Vanguard/Idle2");
	//reader->ReadFile("ModelData/Animations/Vanguard/Walk.fbx");
	//reader->ExportClip(0, "Vanguard/Walk");
	//reader->ReadFile("ModelData/Animations/Vanguard/Run.fbx");
	//reader->ExportClip(0, "Vanguard/Run");
	//reader->ReadFile("ModelData/Animations/Vanguard/Jump.fbx");
	//reader->ExportClip(0, "Vanguard/Jump");
	//reader->ReadFile("ModelData/Animations/Vanguard/Attack.fbx");
	//reader->ExportClip(0, "Vanguard/Attack");
	//delete reader;



	// �̸� �� �о��ְ� ���� ����ϱ�, export material ��
	mp44 = new ModelRender("MP44/MP44");
	mp44->scale = { 0.03f, 0.03f, 0.03f };

	ReadClip("Vanguard/Idle2");
	ReadClip("Vanguard/Walk");
	ReadClip("Vanguard/Run");
	ReadClip("Vanguard/Jump");
	ReadClip("Vanguard/Attack");

	SetEndEvent(JUMP, bind(&Vanguard::JumpEnd, this));
	SetEndEvent(ATTACK, bind(&Vanguard::AttackEnd, this));

	PlayClip(0);

	// offset�� �θ� ��ķ�, ��ġ �� ������Ʈ�� ��°� ó�� 
	offset.rotation.y = XM_PI;
	offset.UpdateWorld();
	parent = offset.GetWorld();
}

Vanguard::~Vanguard()
{
	delete mp44;
}

void Vanguard::Update()
{
	Move();
	Rotation();
	Jump();
	Attack();
	ModelAnimator::Update();

	mp44->Update();
}

void Vanguard::Render()
{
	ModelAnimator::Render();
	mp44->Render();
}

void Vanguard::Move()
{
	if (KEY_PRESS(VK_LSHIFT))
	{
		if (KEY_PRESS(VK_UP))
		{
			moveSpeed += acceleration * Time::Delta();
			moveSpeed = min(moveSpeed, maxSpeed * 2);
			position += Forward() * moveSpeed * Time::Delta();
		}
		else if (KEY_PRESS(VK_DOWN))
		{
			moveSpeed += acceleration * Time::Delta();
			moveSpeed = min(moveSpeed, maxSpeed * 2);
			position -= Forward() * moveSpeed * Time::Delta();
		}
	}
	else
	{
		if (KEY_PRESS(VK_UP))
		{
			moveSpeed += acceleration * Time::Delta();
			moveSpeed = min(moveSpeed, maxSpeed);
			position += Forward() * moveSpeed * Time::Delta();
		}
		else if (KEY_PRESS(VK_DOWN))
		{
			moveSpeed += acceleration * Time::Delta();
			moveSpeed = min(moveSpeed, maxSpeed);
			position -= Forward() * moveSpeed * Time::Delta();
		}
	}

	if (KEY_UP(VK_UP) || KEY_UP(VK_DOWN))
	{
		moveSpeed = 0.0f;
		SetAnimation(IDLE, 2.0, 0.0f);
	}

	// �ӵ��� ���� �ִϸ��̼� ó��
	if (moveSpeed <= maxSpeed && moveSpeed > 0)
	{
		SetAnimation(WALK, 2.0, 0.1f);
	}
	else if (moveSpeed <= maxSpeed*2 && moveSpeed > maxSpeed)
	{
		SetAnimation(RUN, 2.0, 0.1f);
	}


	position.y = terrain->GetAltitude(WorldPos());
}

void Vanguard::Rotation()
{
	if (KEY_PRESS(VK_RIGHT))
	{
		rotation.y += rotSpeed * Time::Delta();
	}
	else if (KEY_PRESS(VK_LEFT))
	{
		rotation.y -= rotSpeed * Time::Delta();
	}
}

void Vanguard::Jump()
{
	if (KEY_DOWN(VK_SPACE))
	{
		SetAnimation(JUMP, 1.0f, 0.0f);
	}
}

void Vanguard::JumpEnd()
{
	SetAnimation(IDLE);
}

void Vanguard::Attack()
{
	if (KEY_DOWN('A'))
	{
		SetAnimation(ATTACK, 1.0f, 0.0f);
	}
}

void Vanguard::AttackEnd()
{
	SetAnimation(IDLE);
}

void Vanguard::SetAnimation(AnimState state, float speed, float takeTime)
{
	if (this->state != state)
	{
		this->state = state;
		PlayClip(state);
	}
}
