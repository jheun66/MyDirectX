#include "Framework.h"
#include "ModelInstancingScene.h"

ModelInstancingScene::ModelInstancingScene()
{
	string name = "maria";

	Export(name);

	model = new ModelAnimator(name + "/" + name);
	model->SetShader(L"VertexModelAnimationInstancing", L"PixelNormalMapping");
	model->ReadClip(name + "/Idle");
	model->ReadClip(name + "/Run");

	UINT instanceNum = 0;

	for (float z = -5; z < 5; z++)
	{
		for (float x = -5; x < 5; x++)
		{
			Transform* transform = model->AddTransform();
			transform->scale = { 0.1f, 0.1f, 0.1f };
			transform->position = { x * 3, 0, z * 3 };
			transform->UpdateWorld();

			model->PlayClip(instanceNum, GameMath::Random(0, 2), GameMath::Random(1.0f, 10.0f));
			instanceNum++;
		}
	}
}

ModelInstancingScene::~ModelInstancingScene()
{
	delete model;
}

void ModelInstancingScene::Update()
{
	model->Update();
}

void ModelInstancingScene::PreRender()
{
}

void ModelInstancingScene::Render()
{
	model->Render();
}

void ModelInstancingScene::PostRender()
{
}

void ModelInstancingScene::Export(string name)
{
	ModelReader* reader = new ModelReader();
	reader->ReadFile("ModelData/Models/" + name + ".fbx");
	reader->ExportMaterial(name + "/" + name);
	reader->ExportMesh(name + "/" + name);
	delete reader;
}
