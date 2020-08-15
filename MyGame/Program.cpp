#include "Framework.h"

#include "Scenes/CubeScene.h"
#include "Scenes/SphereScene.h"
//#include "Scenes/RobotScene.h"
#include "Scenes/TextureScene.h"
#include "Scenes/PlaneScene.h"
#include "Scenes/GridScene.h"

Program::Program()
{
	Time::Get()->Start();
	scene = new GridScene();
}

Program::~Program()
{
	delete scene;
}

void Program::Update()
{
	Time::Get()->Update();
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