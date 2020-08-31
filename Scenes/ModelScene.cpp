#include "framework.h"
#include "ModelScene.h"

ModelScene::ModelScene()
{
	string name = "Tree/Tree";
	modelReader = new ModelReader();
	modelReader->ReadFile("ModelData/Models/tree-05.fbx");
	modelReader->ExportMaterial(name);
	modelReader->ExportMesh(name);

	model = new Model(name);
	model->SetShader(L"Tree");

	settingBuffer = new MSBuffer();
}

ModelScene::~ModelScene()
{
	delete modelReader;
	delete model;
	delete settingBuffer;
}

void ModelScene::Update()
{
	model->Update();
}

void ModelScene::PreRender()
{
}

void ModelScene::Render()
{
	settingBuffer->SetBufferToPS(10);
	model->Render();
}

void ModelScene::PostRender()
{
	ImGui::SliderInt("SpecularMap", &settingBuffer->data.option[0], 0, 1);
	ImGui::SliderInt("NormalMap", &settingBuffer->data.option[1], 0, 1);
}
