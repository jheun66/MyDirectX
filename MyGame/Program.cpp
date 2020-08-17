#include "Framework.h"

#include "Scenes/CubeScene.h"
#include "Scenes/SphereScene.h"
#include "Scenes/TextureScene.h"
#include "Scenes/PlaneScene.h"
#include "Scenes/GridScene.h"

Program::Program()
{
	// viewport¿Í samplerstate »ý¼º
	Enviroment::Create();
	Keyboard::Create();
	Mouse::Create();

	Time::Create();
	Time::Get()->Start();

	camera = new Camera("mainCamera");
	camera->position = { 128, 50, -2 };
	camera->rotation = { -30, 0, 0 };
	SetCursorPos((int)(WIN_WIDTH / 2), (int)(WIN_HEIGHT / 2));

	scene = new GridScene();
}

Program::~Program()
{
	delete scene;
	delete camera;
	Time::Delete();
	Keyboard::Delete();
	Mouse::Delete();
	Enviroment::Delete();
}

void Program::Update()
{
	Time::Get()->Update();
	Keyboard::Get()->Update();
	Mouse::Get()->Update();
	scene->Update();
	camera->Update();
}

void Program::PreRender()
{
	scene->PreRender();
}

void Program::Render()
{
	camera->GetView()->SetBufferToVS(1);
	camera->GetProjection()->SetBufferToVS(2);

	scene->Render();
}

void Program::PostRender()
{
	scene->PostRender();
	camera->PostRender();
}