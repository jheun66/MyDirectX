#include "Framework.h"
#include "GridScene.h"

GridScene::GridScene()
{
	grid = new Grid(L"Landscape/Dirt3.png", L"HeightMaps/HeightMap.png");
	planet = new Planet(L"Earth/earthmap1k.jpg", "sphere");

	planet->position = Vector3(32, 39, 0);

	dirLBuffer = new DirLightBuffer();
}

GridScene::~GridScene()
{
	delete dirLBuffer;
	delete planet;
	delete grid;
}

void GridScene::Update()
{
	planet->Update();
}

void GridScene::PreRender()
{
}

void GridScene::Render()
{
	dirLBuffer->SetBufferToVS(3);
	planet->Render();
	grid->Render();
}

void GridScene::PostRender()
{
	ImGui::Begin("Light", 0, ImGuiWindowFlags_AlwaysAutoResize);
	{
		ImGui::SliderFloat3("Dir", (float*)&dirLBuffer->data.direction, -100, 100);
	}
	ImGui::End();
	grid->PostRender();
	
}
