#include "framework.h"
#include "TerrainScene.h"

TerrainScene::TerrainScene()
{
	terrain = new Terrain();
	settingBuffer = new MSBuffer();
}

TerrainScene::~TerrainScene()
{
	delete settingBuffer;
	delete terrain;
}

void TerrainScene::Update()
{
	terrain->Update();
}

void TerrainScene::PreRender()
{
}

void TerrainScene::Render()
{
	settingBuffer->SetBufferToPS(1);
	terrain->Render();
}

void TerrainScene::PostRender()
{
	ImGui::Checkbox("SpecularMapping", (bool*)&settingBuffer->data.option[0]);
	ImGui::Checkbox("NormalMapping", (bool*)&settingBuffer->data.option[1]);

	terrain->PostRender();
}
