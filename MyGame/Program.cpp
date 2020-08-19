#include "Framework.h"

#include "Scenes/CubeScene.h"
#include "Scenes/SphereScene.h"
#include "Scenes/TextureScene.h"
#include "Scenes/PlaneScene.h"
#include "Scenes/GridScene.h"
#include "Scenes/SolarSystemScene.h"

Program::Program()
{
	// viewport와 samplerstate 생성
	Enviroment::Create();
	Keyboard::Create();
	Mouse::Create();

	Time::Create();
	Time::Get()->Start();

	camera = new Camera("mainCamera");
	camera->position = Vector3(32, 39, -10);
	// 현제 라디안 단위
	camera->rotation = Vector3(0, 0, 0);

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

	Texture::Delete();
	Shader::Delete();
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