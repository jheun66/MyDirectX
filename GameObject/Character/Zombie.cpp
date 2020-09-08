#include "framework.h"

Zombie::Zombie()
	:ModelAnimator("Zombie/Zombie"), moveSpeed(10), rotSpeed(10)
{
	scale = { 0.01f, 0.01f, 0.01f };

	ReadClip("Zombie/Idle");
	ReadClip("Zombie/Run");

	PlayClip(0);

	// offset을 부모 행렬로, 마치 빈 오브젝트에 담는거 처럼 
	
	offset.UpdateWorld();
	parent = offset.GetWorld();
}

Zombie::~Zombie()
{
}

void Zombie::Update()
{
	AutoMove();
	offset.UpdateWorld();
	ModelAnimator::Update();
}

void Zombie::Render()
{
	ModelAnimator::Render();
}

void Zombie::AutoMove()
{
	if ((player->WorldPos() - offset.position).Length() < 20.0f)
	{
		SetAnimation(RUN);

		Vector3 forward = XMVector3Normalize(player->WorldPos() - offset.position);
		Vector3 up = (Vector3)GameMath::kUp;
		Vector3 right = XMVector3Normalize(up.Cross(forward));

		XMFLOAT3X3 tmp(right.x, right.y, right.z, up.x, up.y, up.z, forward.x, forward.y, forward.z);
		XMMATRIX mat = XMLoadFloat3x3(&tmp);
		XMVECTOR quater = XMQuaternionRotationMatrix(mat);

		float rotationY = 0.0f;
		XMQuaternionToAxisAngle(&up.data, &rotationY, quater);
		offset.rotation = up * rotationY;
		offset.rotation.y += XM_PI;

		offset.position -= Forward() * moveSpeed * Time::Delta();

		offset.position.y = terrain->GetAltitude(offset.position);
	}
	else
		SetAnimation(IDLE);
	
}

void Zombie::SetAnimation(AnimState state)
{
	if (this->state != state)
	{
		this->state = state;
		PlayClip(state);
	}
}
