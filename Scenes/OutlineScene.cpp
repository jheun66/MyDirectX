#include "Framework.h"
#include "OutlineScene.h"

OutlineScene::OutlineScene()
{
	Camera::Get()->ChangeCameraMode(true);

	zombie = new Zombie();
	terrain = new Terrain(100,100);

	zombie->SetTerrain(terrain);
	Camera::Get()->SetTarget(zombie);

	renderTarget = new RenderTarget();
	depthStencil = new DepthStencil();

	outline = new Outline();
	outline->SetSRV(renderTarget->GetSRV());
}

OutlineScene::~OutlineScene()
{
	delete zombie;
	delete terrain;

	delete renderTarget;
	delete depthStencil;

	delete outline;

}

void OutlineScene::Update()
{
	zombie->Update();
	terrain->Update();

	outline->Update();
}

void OutlineScene::PreRender()
{
	renderTarget->Set(depthStencil);

	zombie->Render();
}

void OutlineScene::Render()
{
	zombie->Render();
	terrain->Render();
}

void OutlineScene::PostRender()
{
	outline->Render();
}
