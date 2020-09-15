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

		Ray ray;
		ray.position = position;
		ray.direction = (destPos - position).Normal();

		float distance = GameMath::Distance(position, destPos);
		if (aStar->isCollisionObstacle(ray, distance))
		{
			int startIndex = aStar->FindCloseNode(position);
			int endIndex = aStar->FindCloseNode(destPos);

			aStar->Reset();

			path = aStar->FindPath(startIndex, endIndex);
		}

		// AStar���� �־��� �� ���� ��
		path.insert(path.begin(), destPos);
	}
}

void Medea::Move()
{
	MovePath();
	//velocity = destPos - position;

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

void Medea::MovePath()
{
	if (path.empty())
		return;

	Vector3 dest = path.back();

	velocity = dest - position;

	if (velocity.Length() < 0.1f)
		path.pop_back();
}


// ������ �̿��� ���� ��ȯ
void Medea::Rotate()
{
	if (velocity.Length() < 0.1f)
		return;

	// �޸��� +Z
	Vector3 start = Forward() * -1;
	Vector3 end = velocity.Normal();

	// |A|,|B|�� ���� ���ʹϱ� 1
	float cosValue = start.Dot(end);
	float angle = acos(cosValue);

	// �����̰� �� ������
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