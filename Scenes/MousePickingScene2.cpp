#include "framework.h"
#include "MousePickingScene2.h"

MousePickingScene2::MousePickingScene2()
{
	terrain = new Terrain(200, 200);
	vanguard = new Vanguard();
	vanguard->SetTerrain(terrain);

	for (int i = 0; i < 10; i++)
	{
		Zombie* zombie = new Zombie();
		zombie->offset.position = { (float)GameMath::Random(20, 100), 0, (float)GameMath::Random(20, 100) };

		zombie->SetTerrain(terrain);
		zombie->SetSpeed(GameMath::Random(7, 12));
		zombie->SetCollider(new SphereCollider(1.0f));

		zombies.push_back(zombie);
	}
	Camera::Get()->position = Vector3(0, 10, -30);
	Camera::Get()->SetTarget(vanguard);
}

MousePickingScene2::~MousePickingScene2()
{
	for (auto& zombie : zombies)
	{
		delete zombie;
	}
	delete terrain;
	delete vanguard;
}

void MousePickingScene2::Update()
{
	if (Mouse::Get()->Down(0))
	{
		Ray ray = Camera::Get()->ScreenPointToRay(Mouse::Get()->GetPosition());
		if (KEY_PRESS(VK_LCONTROL))
		{
			// 여러개 선택 가능
			for (Zombie* zombie : zombies)
			{
				if (zombie->isPicked)
					continue;
				if (zombie->CheckCollision(ray))
					zombie->Pick(true);
				else
					zombie->Pick(false);
			}
		}
		else
		{
			float minDis = FLT_MAX;
			UINT index = -1;
			// 하나만 선택하도록
			for (UINT i = 0; i < zombies.size();i++)
			{
				Contact contact;
				if (zombies[i]->CheckCollision(ray, &contact))
				{
					if (contact.distance < minDis)
					{
						minDis = contact.distance;
						index = i;
					}
				}
			}
			for (UINT i = 0; i < zombies.size(); i++)
			{
				if(i == index)
					zombies[i]->Pick(true);
				else
					zombies[i]->Pick(false);
			}
		}
	}
	if (Mouse::Get()->Down(1))
	{
		Ray ray = Camera::Get()->ScreenPointToRay(Mouse::Get()->GetPosition());

		Vector3 targetPos;
		terrain->ComputePicking(&targetPos);

		for (Zombie* zombie : zombies)
		{
			if (zombie->isPicked)
			{
				zombie->SetTarget(targetPos);
			}
		}
	}

	terrain->Update();
	vanguard->Update();
	for (auto& zombie : zombies)
	{
		zombie->Update();
	}
}

void MousePickingScene2::PreRender()
{
}

void MousePickingScene2::Render()
{
	terrain->Render();
	vanguard->Render();
	for (auto& zombie : zombies)
	{
		zombie->Render();
	}
}

void MousePickingScene2::PostRender()
{
	for (int i = 0; i < 10; i++)
	{
		ImGui::Text("Zombie %d: %f, %f, %f", i, zombies[i]->offset.position.x, zombies[i]->offset.position.y, zombies[i]->offset.position.z);
	}
}
