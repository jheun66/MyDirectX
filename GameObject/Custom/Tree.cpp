#include "framework.h"

Tree::Tree()
	:ModelRender("Tree/Tree")
{
	SetShader(L"Model");

	collider = new BoxCollider();
	collider->position = { -1,5,-5 };
	collider->scale = { 15,25,15 };
	collider->SetParent(&world);
}

Tree::~Tree()
{
}

void Tree::Update()
{
	////Ã¼Å©¿ë
	//Ray ray = Camera::Get()->ScreenPointToRay(Mouse::Get()->GetPosition());
	//if (collider->IsCollision(ray))
	//{
	//	collider->SetColor({ 0,0,1,1 });
	//}
	//else
	//{
	//	collider->SetColor({ 1,0,0,1 });
	//}
	collider->UpdateWorld();
	__super::Update();
}

void Tree::Render()
{
	collider->Render();
	__super::Render();
}
