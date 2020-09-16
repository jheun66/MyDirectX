#include "Framework.h"
#include "ModelAnimationScene.h"

ModelAnimationScene::ModelAnimationScene()
{
	// 모아서 한꺼번에 해주기 한번만
	//ModelReader* reader = new ModelReader();
	//reader->ReadFile("ModelData/Animations/Zombie/Run.fbx");
	//reader->ExportClip(0, "Zombie/Run");
	//delete reader;

	//string name = "Vanguard/Vanguard";
	//ModelReader* reader = new ModelReader();
	//reader->ReadFile("ModelData/Models/Vanguard.fbx");
	//reader->ExportMaterial(name);
	//reader->ExportMesh(name);
	//delete reader;

	//string name = "Sword/Sword";
	//ModelReader* reader = new ModelReader();
	//reader->ReadFile("ModelData/Models/Sword.fbx");
	//reader->ExportMaterial(name);
	//reader->ExportMesh(name);
	//delete reader;


	terrain = new Terrain(200, 200);
	vanguard = new Vanguard();

	vanguard->SetTerrain(terrain);

	for (int i = 0; i < 10; i++)
	{
		Zombie* zombie = new Zombie();
		zombie->offset.position = { (float)GameMath::Random(20, 100), 0, (float)GameMath::Random(20, 100) };
		
		zombie->SetTerrain(terrain);
		//zombie->SetPlayer(vanguard);
		zombie->SetSpeed(GameMath::Random(7, 12));
		zombie->SetCollider(new SphereCollider(1.0f));
		zombies.push_back(zombie);
	}
	Camera::Get()->position = Vector3(0, 10, -30);
	Camera::Get()->SetTarget(vanguard);

	skybox = new Skybox();
}

ModelAnimationScene::~ModelAnimationScene()
{
	for (auto &zombie : zombies)
	{
		delete zombie;
	}
	delete terrain;
	delete vanguard;
	delete skybox;
}

void ModelAnimationScene::Update()
{
	terrain->Update();
	vanguard->Update();
	for (auto& zombie : zombies)
	{
		zombie->Update();
	}

}

void ModelAnimationScene::PreRender()
{
}

void ModelAnimationScene::Render()
{
	// 중요 skybox 맨위에
	skybox->Render();

	terrain->Render();
	vanguard->Render();
	for (auto& zombie : zombies)
	{
		zombie->Render();
	}
}

void ModelAnimationScene::PostRender()
{
	for (int i = 0; i < 10; i++)
	{
		ImGui::Text("Zombie %d: %f, %f, %f", i, zombies[i]->offset.position.x, zombies[i]->offset.position.y, zombies[i]->offset.position.z);
	}
	vanguard->PostRender();
}
