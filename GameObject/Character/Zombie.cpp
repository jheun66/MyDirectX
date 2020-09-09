#include "framework.h"

Zombie::Zombie()
	:ModelAnimator("Zombie/Zombie"), moveSpeed(10), rotSpeed(10), collider(nullptr), isPicked(false), canMove(false)
{
	scale = { 0.01f, 0.01f, 0.01f };

	ReadClip("Zombie/Idle");
	ReadClip("Zombie/Run");

	PlayClip(0);

	// offset을 부모 행렬로, 마치 빈 오브젝트에 담는거 처럼 
	
	offset.UpdateWorld();
	SetParent(offset.GetWorld());

}

Zombie::~Zombie()
{
	if (collider != nullptr)
	{
		delete collider;
	}
}

void Zombie::Update()
{
	AutoMove();
	offset.UpdateWorld();
	ModelAnimator::Update();

	if (collider != nullptr)
	{
		collider->UpdateWorld();
	}
}

void Zombie::Render()
{
	ModelAnimator::Render();

	if (collider != nullptr)
	{
		collider->Render();
	}
}

void Zombie::AutoMove()
{
	if (canMove)
	{
		if ((targetPos - offset.position).Length() > 0.1f)
		{
			SetAnimation(RUN);

			Vector3 forward = XMVector3Normalize(targetPos - offset.position);
			Vector3 up = (Vector3)GameMath::kUp;
			Vector3 right = XMVector3Normalize(up.Cross(forward));

			XMFLOAT3X3 tmp(right.x, right.y, right.z, up.x, up.y, up.z, forward.x, forward.y, forward.z);
			XMMATRIX mat = XMLoadFloat3x3(&tmp);
			XMVECTOR quater = XMQuaternionRotationMatrix(mat);

			float rotationY = 0.0f;
			XMQuaternionToAxisAngle(&up.data, &rotationY, quater);
			offset.rotation = up * rotationY;
			offset.rotation.y += XM_PI;

			offset.position += forward * moveSpeed * Time::Delta();

			offset.position.y = terrain->GetAltitude(offset.position);
		}
		else
		{
			canMove = false;
		}
	}
	else
	{
		SetAnimation(IDLE);
	}
}

void Zombie::SetAnimation(AnimState state)
{
	if (this->state != state)
	{
		this->state = state;
		PlayClip(state);
	}
}

void Zombie::SetCollider(Collider* collider)
{
	this->collider = collider;
	if (collider != nullptr)
	{
		collider->position.y += 0.6f;
		collider->SetParent(offset.GetWorld());
	}
}

bool Zombie::CheckCollision(Ray ray, Contact* contact)
{
	if (collider->IsCollision(ray, contact))
		return true;
	else
		return false;
}

void Zombie::Pick(bool isPicked)
{
	this->isPicked = isPicked;
	if (isPicked)
	{
		collider->SetColor({ 0,0,1,1 });
	}
	else
	{
		collider->SetColor({ 0,1,0,1 });
	}
}

void Zombie::SetTarget(Vector3 targetPos)
{
	this->targetPos = targetPos;
	canMove = true;
}