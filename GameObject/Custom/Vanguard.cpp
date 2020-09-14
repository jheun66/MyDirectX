#include "framework.h"
#include <algorithm>

Vanguard::Vanguard()
	:ModelAnimator("Vanguard/Vanguard"),
	moveSpeed(30.0f), rotSpeed(20.0f), state(IDLE),
	acceleration(10), deceleration(3), velocity(0, 0, 0) , characterCollider(nullptr), isAttack(false)
{
	scale = { 0.1f, 0.1f, 0.1f };
	ReadClip("Vanguard/Idle");
	ReadClip("Vanguard/Walk");
	ReadClip("Vanguard/Run");
	ReadClip("Vanguard/Attack");

	SetEndEvent(ATTACK, bind(&Vanguard::AttackEnd, this));

	PlayClip(0);

	// 미리 모델 읽어주고 나서 사용하기, export material 등
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
	delete characterCollider;
	delete weaponCollider;
}

void Vanguard::Update()
{
	////체크용
	//Ray ray = Camera::Get()->ScreenPointToRay(Mouse::Get()->GetPosition());
	//if (characterCollider->IsCollision(ray))
	//{
	//	characterCollider->SetColor({ 0,0,1,1 });
	//}
	//else
	//{
	//	characterCollider->SetColor({ 1,0,0,1 });
	//}
	//if (weaponCollider->IsCollision(ray))
	//{
	//	weaponCollider->SetColor({ 0,0,1,1 });
	//}
	//else
	//{
	//	weaponCollider->SetColor({ 1,0,0,1 });
	//}


	Move();
	Rotation();

	SetWeapon();
	sword->Update();

	ModelAnimator::Update();
	characterCollider->UpdateWorld();
	weaponCollider->UpdateWorld();
}

void Vanguard::Render()
{
	ModelAnimator::Render();
	characterCollider->Render();
	weaponCollider->Render();
	sword->Render();
}

void Vanguard::PostRender()
{
}

// 마우스를 이용한 이동
void Vanguard::Move()
{
	// 픽킹처리
	if (Mouse::Get()->Down(0))
	{
		Ray ray = Camera::Get()->ScreenPointToRay(Mouse::Get()->GetPosition());
		if (characterCollider->IsCollision(ray))
		{
			isPicked = true;
			characterCollider->SetColor({ 0,0,1,1 });
		}
		else
		{
			isPicked = false;
			characterCollider->SetColor({ 0,1,0,1 });
		}
	}	

	if (isPicked)
	{
		if (Mouse::Get()->Down(1))
		{
			Vector3 targetPos;
			terrain->ComputePicking(&targetPos);
			// 현재 코드는 쓸데없이 두번함
			Ray ray = Camera::Get()->ScreenPointToRay(Mouse::Get()->GetPosition());


			if (enemy->GetCollider()->IsCollision(ray) && !enemy->isDead)
			{
				isAttack = true;
				destPos = enemy->GetCollider()->WorldPos();
				destPos.y = 0;
			}
			else
			{
				isAttack = false;
				destPos = targetPos;
			}
			canMove = true;
		}
	}

	if (canMove)
	{
		velocity = destPos - position;
		if (isAttack)
		{
			if (velocity.Length() > 15)
			{
				position += velocity.Normal() * moveSpeed * Time::Delta();

				SetAnimation(RUN);

				velocity = XMVectorLerp(velocity.data, XMVectorZero(), deceleration * Time::Delta());
			}
			else
			{
				canMove = false;
				SetAnimation(ATTACK);
			}
		}
		else
		{
			if (velocity.Length() > 0.1f)
			{
				position += velocity.Normal() * moveSpeed * Time::Delta();

				SetAnimation(RUN);

				velocity = XMVectorLerp(velocity.data, XMVectorZero(), deceleration * Time::Delta());
			}
			else
			{
				canMove = false;
				SetAnimation(IDLE);
			}
		}
	}

	if (isAttack)
	{
		if (weaponCollider->IsSphereCollision((SphereCollider*)enemy->GetCollider()))
		{
			enemy->GetCollider()->SetColor({ 1,0,0,1 });
			// 데미지 10 받는걸 의미
			enemy->Damaged(10.0f);
		}
		else
		{
			enemy->GetCollider()->SetColor({ 0,1,0,1 });
		}
	}
	else
	{
		enemy->GetCollider()->SetColor({ 0,1,0,1 });
	}

}

// 마우스를 이용한 회전
void Vanguard::Rotation()
{
	if (velocity.Length() < 0.1f)
		return;

	// 뒷면이 +Z
	Vector3 start = Forward() * -1;
	Vector3 end = velocity.Normal();

	// |A|,|B|는 단위 벡터니까 1
	float cosValue = start.Dot(end);
	float angle = acos(cosValue);

	// 각차이가 얼마 없으면
	if (angle < 0.1f)
		return;

	Vector3 cross = start.Cross(end);

	if (cross.y > 0.0f)
		rotation.y += rotSpeed * Time::Delta();
	else
		rotation.y -= rotSpeed * Time::Delta();
}

void Vanguard::AttackEnd()
{
	isAttack = false;
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

void Vanguard::SetCollider(Collider* col)
{
	characterCollider = col;
	if (characterCollider != nullptr)
	{
		characterCollider->position.y += 60.0f;
		characterCollider->scale = { 10,10,10 };
		characterCollider->SetParent(&world);
	}
}

void Vanguard::SetWeapon()
{
	boneWorld = GetCurBoneMatrix(rightHand->index);

	boneWorld = rightHand->transform * boneWorld * world;
}

void Vanguard::SetEnemy(ModelAnimator* model)
{
	Zombie* zombie = dynamic_cast<Zombie*>(model);
	enemy = zombie;
}

