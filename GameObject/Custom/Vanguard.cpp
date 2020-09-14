#include "framework.h"
#include <algorithm>

Vanguard::Vanguard()
	:ModelAnimator("Vanguard/Vanguard"), maxSpeed(7.5f), moveSpeed(0), acceleration(3.5f), rotSpeed(5)
{
	scale = { 0.01f, 0.01f, 0.01f };

	// 모아서 한꺼번에 해주기 한번만
	ModelReader* reader = new ModelReader();
	reader->ReadFile("ModelData/Animations/Vanguard/SwordIdle.fbx");
	reader->ExportClip(0, "Vanguard/Idle");
	reader->ReadFile("ModelData/Animations/Vanguard/SwordWalk.fbx");
	reader->ExportClip(0, "Vanguard/Walk");
	reader->ReadFile("ModelData/Animations/Vanguard/SwordRun.fbx");
	reader->ExportClip(0, "Vanguard/Run");
	reader->ReadFile("ModelData/Animations/Vanguard/SwordAttack.fbx");
	reader->ExportClip(0, "Vanguard/Attack");
	delete reader;



	// 미리 모델 읽어주고 나서 사용하기, export material 등
	sword = new ModelRender("Sword/Sword");
	sword->position = { -7.5f,-4.0f,-22.0f };
	sword->scale = { 1.5f, 1.5f, 1.5f };
	sword->rotation = {0,XM_PI,0};
	rightHand = GetBoneByName("mixamorig:RightHand");
	sword->SetParent(&boneWorld);


	ReadClip("Vanguard/Idle");
	ReadClip("Vanguard/Walk");
	ReadClip("Vanguard/Run");
	ReadClip("Vanguard/Attack");

	//SetEndEvent(ATTACK, bind(&Vanguard::AttackEnd, this));

	PlayClip(0);

	// offset을 부모 행렬로, 마치 빈 오브젝트에 담는거 처럼 
	offset.rotation.y = XM_PI;
	offset.UpdateWorld();
	parent = offset.GetWorld();
}

Vanguard::~Vanguard()
{
	delete sword;
}

void Vanguard::Update()
{
	Move();
	Rotation();
	Attack();

	SetWeapon();

	ModelAnimator::Update();

	sword->Update();
}

void Vanguard::Render()
{
	ModelAnimator::Render();
	sword->Render();
}

void Vanguard::PostRender()
{
	ImGui::SliderFloat3("weapon position", (float*)&sword->position, -40, 40, "%.1f", 1);
	ImGui::SliderFloat3("weapon scale", (float*)&sword->scale, -2, 2, "%.1f", 1);
	ImGui::SliderFloat3("weapon rotation", (float*)&sword->rotation, -XM_PI, XM_PI, "%.3f", 1);

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

	// 속도에 따라 애니메이션 처리
	if (moveSpeed <= maxSpeed && moveSpeed > 0)
	{
		SetAnimation(WALK, 2.0, 0.0f);
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

void Vanguard::Attack()
{
	if (KEY_DOWN('A'))
	{
		SetAnimation(ATTACK, 2.0f, 0.0f);
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

void Vanguard::SetCollider(Collider* col)
{
	CharacterCollider = col;
	if (CharacterCollider != nullptr)
	{
		CharacterCollider->position.y += 0.6f;
		CharacterCollider->SetParent(offset.GetWorld());
	}
}

void Vanguard::SetWeapon()
{
	boneWorld = GetCurBoneMatrix(rightHand->index);

	boneWorld = rightHand->transform * boneWorld * world;
}
