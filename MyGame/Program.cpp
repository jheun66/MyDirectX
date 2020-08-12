#include "Framework.h"

#include "Scenes/CubeScene.h"

Program::Program()
{
	scene = new CubeScene();
}

Program::~Program()
{
	delete scene;
}

void Program::Update()
{
	scene->Update();
}

void Program::PreRender()
{
	scene->PreRender();
}

void Program::Render()
{
	Camera::Get()->GetView()->SetBufferToVS(1);
	Camera::Get()->GetProjection()->SetBufferToVS(2);

	scene->Render();
}

void Program::PostRender()
{
	scene->PostRender();
}