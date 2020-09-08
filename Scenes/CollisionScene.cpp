#include "Framework.h"
#include "CollisionScene.h"

CollisionScene::CollisionScene()
{
	colliders.emplace_back(new SphereCollider());
	colliders.emplace_back(new SphereCollider());
}

CollisionScene::~CollisionScene()
{
	for (Collider* collider : colliders)
		delete collider;
}

void CollisionScene::Update()
{
	//if (colliders[0]->IsCollision(colliders[1]))
	//	colliders[0]->SetColor({ 1, 0, 0, 1 });
	//else
	//	colliders[0]->SetColor({ 0, 1, 0, 1 });


	if (Mouse::Get()->Press(0))
	{
		Ray ray = Camera::Get()->ScreenPointToRay(Mouse::Get()->GetPosition());

		for (Collider* collider : colliders)
		{
			if (collider->IsCollision(ray, &contact))
			{
				collider->SetColor({ 0,0,1,1 });
			}
			else
			{
				collider->SetColor({ 0,1,0,1 });
			}
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
	ImGui::SliderFloat("ContactDistance", &contact.distance, 0, 100);
	ImGui::SliderFloat3("ContactPoint", (float*)&contact.hitPoint, 0, 100);
}