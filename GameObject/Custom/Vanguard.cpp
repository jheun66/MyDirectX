#include "framework.h"
#include <algorithm>

Vanguard::Vanguard()
	:ModelAnimator("Vanguard/Vanguard"), state(IDLE)
{
	scale = { 0.05f, 0.05f, 0.05f };

	// Ű����
	offsetTransform.rotation.y = XM_PI;
	offsetTransform.UpdateWorld();
	SetParent(offsetTransform.GetWorld());

	ReadClip("Vanguard/Idle");
	ReadClip("Vanguard/Walk");
	ReadClip("Vanguard/Run");
	ReadClip("Vanguard/Attack");
	ReadClip("Vanguard/Hit");
	ReadClip("Vanguard/Die");

	SetEndEvent(ATTACK, bind(&Vanguard::AttackEnd, this));

	PlayClip(0);

	SetCollider();

	// �̸� �� �о��ְ� ���� ����ϱ�, export material ��
	sword = new ModelRender("Sword/Sword");
	sword->position = { -7.5f,-4.0f,-22.0f };
	sword->scale = { 1.5f, 1.5f, 1.5f };
	sword->rotation = { 0,XM_PI,0 };
	rightHand = GetBoneByName("mixamorig:RightHand");
	sword->SetParent(&boneWorld);
	weaponCollider = new BoxCollider();
	weaponCollider->scale = { 10,10,60 };
	weaponCollider->SetParent(sword->GetWorld());
}

Vanguard::~Vanguard()
{
	delete sword;

	if (damagedCollider != nullptr)
		delete damagedCollider;

	delete weaponCollider;
}

void Vanguard::Update()
{
	////üũ��
	//Ray ray = Camera::Get()->ScreenPointToRay(Mouse::Get()->GetPosition());
	//if (damagedCollider->IsCollision(ray))
	//{
	//	damagedCollider->SetColor({ 0,0,1,1 });
	//}
	//else
	//{
	//	damagedCollider->SetColor({ 1,0,0,1 });
	//}
	
	Vector3 origin = position;
	if (state != HIT && state != DIE)
	{
		Move();
		//Rotation();
	}

	SetWeapon();
	sword->Update();

	ModelAnimator::Update();
	damagedCollider->UpdateWorld();
	weaponCollider->UpdateWorld();

	// �� �̻� ���ö󰡰�
	//if (position.y > 5.0f)
	//	position = origin;
}

void Vanguard::Render()
{
	ModelAnimator::Render();
	damagedCollider->Render();
	weaponCollider->Render();
	sword->Render();
}

void Vanguard::PostRender()
{
}

void Vanguard::Move()
{
	if (KEY_PRESS(VK_UP))
	{
		position += Forward() * moveSpeed * Time::Delta();
		SetAnimation(RUN);
	}
	if (KEY_PRESS(VK_DOWN))
	{
		position -= Forward() * moveSpeed * Time::Delta();
		SetAnimation(RUN);
	}
	if (KEY_PRESS(VK_RIGHT))
	{
		rotation.y += rotSpeed * Time::Delta();
		SetAnimation(RUN);
	}
	if (KEY_PRESS(VK_LEFT))
	{
		rotation.y -= rotSpeed * Time::Delta();
		SetAnimation(RUN);
	}

	if (KEY_UP(VK_UP) || KEY_UP(VK_DOWN) || KEY_UP(VK_RIGHT) || KEY_UP(VK_LEFT))
	{
		SetAnimation(IDLE);
	}

	if (KEY_DOWN('A'))
	{
		SetAnimation(ATTACK);
	}

	position.y = terrain->GetAltitude(WorldPos());
}

//void Vanguard::Move()
//{
//	// ���� �� �̵�
//	if (KEY_PRESS('A') && KEY_DOWN(VK_LBUTTON))
//	{
//		//terrain->ComputePicking(&destPos);
//		Ray ray = Camera::Get()->ScreenPointToRay(Mouse::Get()->GetPosition());
//
//		ModelAnimator* who = Check();
//		if (who)
//		{
//			enemy = who;
//		}
//		else
//		{
//			enemy = nullptr;
//			terrain->ComputePicking(&destPos);
//		}
//	}
//	
//	// �ܼ� �̵�
//	if (KEY_DOWN(VK_RBUTTON))
//	{
//		enemy = nullptr;
//		terrain->ComputePicking(&destPos);
//	}
//
//	if(enemy)
//	{
//		destPos = enemy->WorldPos();
//
//		velocity = destPos - position;
//
//		if (velocity.Length() > 5.0f)
//		{
//			position += velocity.Normal() * moveSpeed * Time::Delta();
//			SetAnimation(RUN);
//			velocity = XMVectorLerp(velocity.data, XMVectorZero(), deceleration * Time::Delta());
//		}
//		else
//		{
//			// ���� ��ġ ����
//			destPos = WorldPos();
//			SetAnimation(ATTACK);
//		}
//	}
//	else
//	{
//		velocity = destPos - position;
//
//		if (velocity.Length() > 0.1f)
//		{
//			position += velocity.Normal() * moveSpeed * Time::Delta();
//			SetAnimation(RUN);
//			velocity = XMVectorLerp(velocity.data, XMVectorZero(), deceleration * Time::Delta());
//		}
//		else
//		{
//			SetAnimation(IDLE);
//		}
//	}
//	
//}
//
//// ���콺�� �̿��� ȸ��
//void Vanguard::Rotation()
//{
//	if (velocity.Length() < 0.1f)
//		return;
//
//	// �޸��� +Z
//	Vector3 start = Forward() * -1;
//	Vector3 end = velocity.Normal();
//
//	// |A|,|B|�� ���� ���ʹϱ� 1
//	float cosValue = start.Dot(end);
//	float angle = acos(cosValue);
//
//	// �����̰� �� ������
//	if (angle < 0.1f)
//		return;
//
//	Vector3 cross = start.Cross(end);
//
//	if (cross.y > 0.0f)
//		rotation.y += rotSpeed * Time::Delta();
//	else
//		rotation.y -= rotSpeed * Time::Delta();
//}

void Vanguard::AttackEnd()
{
	// �ѹ��� ��������
	//enemy=nullptr;
	SetAnimation(IDLE);
}

void Vanguard::SetAnimation(AnimState state)
{
	if (this->state != state)
	{
		this->state = state;

		if (state == ATTACK)
			PlayClip(state, 1, 0);
		else
			PlayClip(state);
	}
}

void Vanguard::SetCollider()
{
	damagedCollider = new BoxCollider();
	damagedCollider->SetColor({ 1,0,0,1 });
	damagedCollider->position.y += 70.0f;
	damagedCollider->scale = { 100,200,100 };
	damagedCollider->SetParent(&world);
}

void Vanguard::SetWeapon()
{
	boneWorld = GetCurBoneMatrix(rightHand->index);

	boneWorld = rightHand->transform * boneWorld * world;
}

void Vanguard::Damaged(float damage)
{
	if (state != DIE)
	{
		hp -= damage;
		if (hp < 0)
			SetAnimation(DIE);
		else
			SetAnimation(HIT);
	}
}
