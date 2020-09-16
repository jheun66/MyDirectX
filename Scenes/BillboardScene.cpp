#include "Framework.h"
#include "BillboardScene.h"

BillboardScene::BillboardScene()
{
	terrain = new Terrain(100, 100);

	for (UINT i = 0; i < 1000; i++)
	{
		Billboard* tree = new Billboard(L"Textures/Landscape/Tree.png");

		Vector3 pos;
		pos.x = GameMath::Random(0.0f, 255.0f);
		pos.z = GameMath::Random(0.0f, 255.0f);

		Vector3 scale;
		scale.x = GameMath::Random(3.0f, 10.0f);
		scale.y = GameMath::Random(3.0f, 10.0f);
		scale.z = 1.0f;

		pos.y = (scale.y * 0.5f + terrain->GetAltitude(pos));

		tree->position = pos;
		tree->scale = scale;

		trees.emplace_back(tree);
	}
}

BillboardScene::~BillboardScene()
{
	delete terrain;

	for (Billboard* tree : trees)
		delete tree;
}

void BillboardScene::Update()
{
	terrain->Update();

	for (Billboard* tree : trees)
		tree->Update();
}

void BillboardScene::PreRender()
{
}

void BillboardScene::Render()
{
	terrain->Render();

	for (Billboard* tree : trees)
		tree->Render();
}

void BillboardScene::PostRender()
{
}
