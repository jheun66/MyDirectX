#include "Framework.h"

//#include "Scenes/CubeScene.h"
//#include "Scenes/SphereScene.h"
//#include "Scenes/TextureScene.h"
//#include "Scenes/PlaneScene.h"
//#include "Scenes/GridScene.h"
#include "Scenes/SolarSystemScene.h"
#include "Scenes/TerrainScene.h"

Program::Program()
{
	// viewport, samplerstate, mainCamera »ý¼º
	Enviroment::Create();
	Keyboard::Create();
	Mouse::Create();

	Time::Create();
	Time::Get()->Start();

	SetCursorPos((int)(WIN_WIDTH / 2), (int)(WIN_HEIGHT / 2));

	scene = new SolarSystemScene();
}

Program::~Program()
{
	delete scene;

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
	Enviroment::Get()->MainCamera()->Update();
}

void Program::PreRender()
{
	scene->PreRender();
}

void Program::Render()
{
	Enviroment::Get()->MainCamera()->GetViewBuffer()->SetBufferToVS(1);
	Enviroment::Get()->MainCamera()->GetProjection()->SetBufferToVS(2);
	Enviroment::Get()->GetLight()->SetBufferToPS(0);

	scene->Render();
}

void Program::PostRender()
{
	Enviroment::Get()->PostRender();
	scene->PostRender();
}