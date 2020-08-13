#include "Framework.h"
#include "PlaneScene.h"

PlaneScene::PlaneScene()
{
	plane = new Plane(3,3);
}

PlaneScene::~PlaneScene()
{
	delete plane;
}

void PlaneScene::Update()
{
	
	plane->Update();
}

void PlaneScene::PreRender()
{
}

void PlaneScene::Render()
{
	plane->Render();
}

void PlaneScene::PostRender()
{
}
