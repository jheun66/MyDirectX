#include "framework.h"

Maria::Maria()
	:ModelAnimator("Maria/Maria"), state(IDLE)
{
	scale = { 0.05f, 0.05f, 0.05f };
	position = { 100.0f, 0, 100.0f };

	ReadClip("Maria/Idle");
	ReadClip("Maria/Run");
	ReadClip("Maria/Slash");
	ReadClip("Maria/Hit");
	ReadClip("Maria/Die");

	SetEndEvent(SLASH, bind(&Maria::SlashEnd, this));

	// IDLE
	PlayClip(0);

	SetColliders();

	renderTarget = new RenderTarget();
	depthStencil = new DepthStencil();

	sizeBuffer = new SizeBuffer();
	sizeBuffer->data.size = { WIN_WIDTH * 0.5f, WIN_HEIGHT};

	render2D = new Render2D(L"VertexUV", L"PixelOutline");
	render2D->SetSRV(renderTarget->GetSRV());
	render2D->position = { WIN_WIDTH * 0.25f, WIN_HEIGHT * 0.5f, 0.0f };
	render2D->scale = { WIN_WIDTH * 0.5f, WIN_HEIGHT, 1.0f };

}

Maria::~Maria()
{
	delete renderTarget;
	delete depthStencil;
	delete render2D;

	if(damagedCollider != nullptr)
		delete damagedCollider;
	if (attackCollider != nullptr)
		delete attackCollider;
	if (detectCollider != nullptr)
		delete detectCollider;
}

void Maria::Update()
{	
	////체크용
	//Ray ray = Camera::Get()->ScreenPointToRay(Mouse::Get()->GetPosition());
	//if (attackCollider->IsCollision(ray))
	//{
	//	attackCollider->SetColor({ 0,0,1,1 });
	//}
	//else
	//{
	//	attackCollider->SetColor({ 1,0,0,1 });
	//}


	Input();
	Rotate();

	position.y = terrain->GetAltitude(position);
	
	ModelAnimator::Update();

	if (damagedCollider != nullptr)
		damagedCollider->UpdateWorld();
	if (attackCollider != nullptr)
		attackCollider->UpdateWorld();
	if (detectCollider != nullptr)
		detectCollider->UpdateWorld();

	render2D->Update();
}

void Maria::PreRender()
{
	if (detectCollider->IsBoxCollision((BoxCollider*)player->GetDamagedCollider()))
	{
		renderTarget->Set(depthStencil);
		ModelAnimator::Render();
	}
}

void Maria::Render()
{
	if (damagedCollider != nullptr)
		damagedCollider->Render();
	if (attackCollider != nullptr)
		attackCollider->Render();
	if (detectCollider != nullptr)
		detectCollider->Render();
	ModelAnimator::Render();
}

void Maria::PostRender()
{

}

void Maria::OutlineRender()
{
	if (detectCollider->IsBoxCollision((BoxCollider*)player->GetDamagedCollider()))
	{
		sizeBuffer->SetPSBuffer(11);
		render2D->Render();
	}
}

void Maria::Damaged(float damage)
{
	hp -= damage;
	if (hp < 0)
		SetAnimation(DIE);
	else
		SetAnimation(HIT);
}

void Maria::Input()
{
	// 인풋 변경하기 .. , 새로운 방향이 업데이트 되면 수정하기
	if (detectCollider->IsBoxCollision((BoxCollider*)player->GetDamagedCollider()))
	{
		// 동작 끝나고 진행하도록
		if (state != SLASH)
		{
			if (attackCollider->IsBoxCollision((BoxCollider*)player->GetDamagedCollider()))
			{
				path.clear();
				velocity = { 0,0,0 };
				Slash();
			}
			else
			{
				//path.clear(); // 계속 갱신
				if (path.empty())
				{
					destPos = player->WorldPos();
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

					// 장애물이 아닐때만 넣어주도록 해야할 듯..
					path.insert(path.begin(), destPos);
				}
				Move();
			}
		}
	}
	else
	{
		path.clear();
		velocity = { 0,0,0 };
		SetAnimation(IDLE);
	}
}

void Maria::Move()
{
	MovePath();

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

void Maria::MovePath()
{
	if (path.empty())
		return;

	Vector3 dest = path.back();

	velocity = dest - position;

	if (velocity.Length() < 0.1f)
		path.pop_back();
}

void Maria::Rotate()
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

void Maria::Slash()
{
	SetAnimation(SLASH);
}

void Maria::SlashEnd()
{
	// 끝날때 있으면 데미지 주기
	if (attackCollider->IsBoxCollision((BoxCollider*)player->GetDamagedCollider()))
	{
		//player->Damaged(10.0f);
	}

	SetAnimation(IDLE);
}

void Maria::SetAnimation(AnimState value)
{
	if (state != value)
	{
		state = value;
		PlayClip(value);
	}
}

void Maria::SetColliders()
{
	damagedCollider = new BoxCollider();
	damagedCollider->SetColor({ 1,0,0,1 });
	damagedCollider->position.y += 70.0f;
	damagedCollider->scale = { 100,200,100 };
	damagedCollider->SetParent(&world);

	attackCollider = new BoxCollider();
	attackCollider->SetColor({ 0,1,0,1 });
	attackCollider->position.y += 70.0f;
	attackCollider->position.z -= 90.0f;
	attackCollider->scale = { 100,200,100 };
	attackCollider->SetParent(&world);

	// radius 값으로 판단, 크기값은 Scale로 맞춰주기 (부모에서 줄인만큼 늘리면 기본 값)
	detectCollider = new SphereCollider(50);
	detectCollider->position.y += 120.0f;
	detectCollider->scale = { 20,20,20 };
	detectCollider->SetColor({ 0,0,1,1 });
	detectCollider->SetParent(&world);
}
