#include "Framework.h"
#include "CollisionScene.h"

CollisionScene::CollisionScene()
{
	colliders.emplace_back(new BoxCollider());
	colliders.emplace_back(new BoxCollider());
	SphereCollider* sc = new SphereCollider();
	sc->position = Vector3(0, 2, 0);
	colliders.emplace_back(sc);
}

CollisionScene::~CollisionScene()
{
	for (Collider* collider : colliders)
		delete collider;
}

void CollisionScene::Update()
{
	for (UINT i = 0; i < 3; i++)
	{
		Ray ray = Camera::Get()->ScreenPointToRay(Mouse::Get()->GetPosition());
		if (colliders[i]->IsCollision(ray, &contact))
		{
			colliders[i]->SetColor({ 0, 0, 1, 1 });
		}
		else
		{
			bool isCollision = false;
			for (UINT j = 0; j < 3; j++)
			{
				if (i == j)
					continue;
				if (colliders[i]->IsCollision(colliders[j]))
				{
					isCollision = true;
					break;
				}
			}
			if(isCollision)
				colliders[i]->SetColor({ 1, 0, 0, 1 });
			else
				colliders[i]->SetColor({ 0, 1, 0, 1 });
		}
	}
}

void CollisionScene::PreRender()
{
}

void CollisionScene::Render()
{
	for (Collider* collider : colliders)
		collider->Render();
}

void CollisionScene::PostRender()
{
	ImGui::SliderFloat3("ColPos0", (float*)&colliders[0]->position, -10, 10);
	ImGui::SliderFloat3("ColRot0", (float*)&colliders[0]->rotation, -10, 10);

	ImGui::SliderFloat3("ColPos1", (float*)&colliders[1]->position, -10, 10);
	ImGui::SliderFloat3("ColRot1", (float*)&colliders[1]->rotation, -10, 10);

	ImGui::SliderFloat3("ColPos2", (float*)&colliders[2]->position, -10, 10);
	ImGui::SliderFloat3("ColRot2", (float*)&colliders[2]->rotation, -10, 10);

	ImGui::SliderFloat("ContactDistance", &contact.distance, 0, 100);
	ImGui::SliderFloat3("ContactPoint", (float*)&contact.hitPoint, 0, 100);
}
