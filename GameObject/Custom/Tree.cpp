#include "framework.h"

Tree::Tree()
	:ModelRender("Tree/Tree")
{
	SetShader(L"VertexModelInstancing", L"PixelNormalMapping");

	scale = { 0.1f,0.1f,0.1f };

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
	//collider->UpdateWorld();
	__super::Update();
}

void Tree::Render()
{
	__super::Render();
}
