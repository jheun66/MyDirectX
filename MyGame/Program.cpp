#include "Framework.h"

#include "Scenes/SolarSystemScene.h"
#include "Scenes/TerrainScene.h"
#include "Scenes/MousePickingScene.h"
#include "Scenes/TerrainEditorScene.h"

Program::Program()
{
	// viewport, samplerstate, mainCamera »ý¼º
	Environment::Create();
	Keyboard::Create();
	Mouse::Create();

	Time::Create();
	Time::Get()->Start();

	SetCursorPos((int)(WIN_WIDTH / 2), (int)(WIN_HEIGHT / 2));

	scene = new TerrainScene();
}

Program::~Program()
{
	delete scene;

	Time::Delete();
	Keyboard::Delete();
	Mouse::Delete();
	Environment::Delete();

	Texture::Delete();
	Shader::Delete();
}

void Program::Update()
{
	Time::Get()->Update();
	Keyboard::Get()->Update();
	Mouse::Get()->Update();
	scene->Update();
	Environment::Get()->MainCamera()->Update();
}

void Program::PreRender()
{
	scene->PreRender();
}

void Program::Render()
{
	Environment::Get()->MainCamera()->GetViewBuffer()->SetBufferToVS(1);
	Environment::Get()->MainCamera()->GetProjection()->SetBufferToVS(2);
	Environment::Get()->GetLight()->SetBufferToPS(0);

	scene->Render();
}

void Program::PostRender()
{
	Environment::Get()->PostRender();
	scene->PostRender();
}