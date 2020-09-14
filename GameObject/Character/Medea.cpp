#include "Framework.h"

Medea::Medea()
	:ModelAnimator("Medea/Medea"),
	moveSpeed(30.0f), rotSpeed(20.0f), state(IDLE),
	accelation(10), deceleration(3), velocity(0, 0, 0)
{
	ReadClip("Medea/Idle");
	ReadClip("Medea/Running");

	PlayClip(0);
}

Medea::~Medea()
{
}

void Medea::Update()
{
	Input();
	Move();
	Rotate();

	position.y = terrain->GetAltitude(position);

	ModelAnimator::Update();
}

void Medea::Render()
{
	ModelAnimator::Render();
}

void Medea::Input()
{
	if (KEY_DOWN(VK_LBUTTON))
	{
		terrain->ComputePicking(&destPos);
	}
}

void Medea::Move()
{
	velocity = destPos - position;

	if (velocity.Length() > 0.1f)
	{
		position += velocity.Normal() * moveSpeed * Time::Delta();

		SetAnimation(RUN);

		velocity = XMVectorLerp(velocity.data, XMVectorZero(), deceleration * Time::Delta());
	}
	else
	{
		SetAnimation(IDLE);
	}
}

// 외적을 이용한 방향 전환
void Medea::Rotate()
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

void Medea::SetAnimation(AnimState value)
{
	if (state != value)
	{
		state = value;
		PlayClip(value);
	}
}
