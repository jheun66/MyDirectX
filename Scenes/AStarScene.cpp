#include "Framework.h"
#include "AStarScene.h"

AStarScene::AStarScene()
{
	ModelReader* reader = new ModelReader();
	reader->ReadFile("ModelData/Animations/Medea/Idle.fbx");
	reader->ExportClip(0, "Medea/Idle");
	reader->ReadFile("ModelData/Animations/Medea/Running.fbx");
	reader->ExportClip(0, "Medea/Running");
	delete reader;

	terrain = new Terrain(200,200);
	medea = new Medea();
	skybox = new Skybox();
	aStar = new AStar();

	medea->SetTerrain(terrain);
	aStar->Setup(terrain);

	Camera::Get()->ChangeCameraMode(true);
	Camera::Get()->SetTarget(medea);
}

AStarScene::~AStarScene()
{
	delete skybox;
	delete medea;
	delete terrain;
	delete aStar;
}

void AStarScene::Update()
{
	terrain->Update();
	medea->Update();
	aStar->Update();
}

void AStarScene::PreRender()
{
}

void AStarScene::Render()
{
	skybox->Render();

	terrain->Render();
	medea->Render();
	aStar->Render();
}

void AStarScene::PostRender()
{
}
