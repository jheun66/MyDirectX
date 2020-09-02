#include "Framework.h"
#include "GridScene.h"

GridScene::GridScene()
{
	grid = new Terrain(100, 100);
	planet = new Sphere();

	planet->position = Vector3(32, 39, 0);

	dirLBuffer = new DirLightBuffer();
	useMapBuffer = new MSBuffer();
}

GridScene::~GridScene()
{
	delete useMapBuffer;
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
	// ºûÀÌ ÇÈ¼¿·Î °¨
	dirLBuffer->SetBufferToPS(0);
	useMapBuffer->SetBufferToPS(1);

	planet->Render();
	grid->Render();
}

void GridScene::PostRender()
{
	ImGui::Begin("Light", 0, ImGuiWindowFlags_AlwaysAutoResize);
	{
		ImGui::SliderFloat3("Dir", (float*)&dirLBuffer->data.direction, -100, 100);
		ImGui::SliderFloat("LightSpecularExp", (float*)&dirLBuffer->data.specularExp, 1, 100);
		ImGui::ColorEdit4("LightAmbient", (float*)&dirLBuffer->data.ambient);
	}
	ImGui::End();

	ImGui::Begin("Scene", 0, ImGuiWindowFlags_AlwaysAutoResize);
	{
		ImGui::Checkbox("SpecularMapping", (bool*)&useMapBuffer->data.option[0]);
		ImGui::Checkbox("NormalMapping", (bool*)&useMapBuffer->data.option[1]);
	}
	ImGui::End();
	grid->PostRender();
}
