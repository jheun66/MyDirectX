#include "framework.h"
#include "ModelScene.h"

ModelScene::ModelScene()
{
	string name = "Maria/Maria";
	modelReader = new ModelReader();
	modelReader->ReadFile("ModelData/Models/maria.fbx");
	modelReader->ExportMaterial(name);
	modelReader->ExportMesh(name);

	//애니메이션
	modelReader->ReadFile("ModelData/Animations/Maria/Idle.fbx");
	modelReader->ExportClip(0, "Maria/Idle");
	modelReader->ReadFile("ModelData/Animations/Maria/Run.fbx");
	modelReader->ExportClip(0, "Maria/Run");
	modelReader->ReadFile("ModelData/Animations/Maria/Slash.fbx");
	modelReader->ExportClip(0, "Maria/Slash");

	model = new ModelRender(name);
	model->SetShader(L"VertexModel", L"PixelNormalMapping");


	settingBuffer = new SettingBuffer();

	Camera::Get()->ChangeCameraMode(false);
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
	settingBuffer->SetPSBuffer(10);
	model->Render();
}

void ModelScene::PostRender()
{
	ImGui::SliderInt("SpecularMap", &settingBuffer->data.option[0], 0, 1);
	ImGui::SliderInt("NormalMap", &settingBuffer->data.option[1], 0, 1);
}
