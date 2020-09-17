#include "Framework.h"

#include "Scenes/TerrainScene.h"
#include "Scenes/MousePickingScene.h"
#include "Scenes/TerrainEditorScene.h"
#include "Scenes/ModelScene.h"
#include "Scenes/ModelAnimationScene.h"
#include "Scenes/MoveModelScene.h"
#include "Scenes/CollisionScene.h"
#include "Scenes/AStarScene.h"
#include "Scenes/MonsterAStarScene.h"
#include "Scenes/BillboardScene.h"
#include "Scenes/RainScene.h"

Program::Program()
{
	// viewport, samplerstate, mainCamera »ý¼º
	Environment::Create();
	Keyboard::Create();
	Mouse::Create();
	Camera::Create();

	Time::Create();
	Time::Get()->Start();

	SetCursorPos((int)(WIN_WIDTH / 2), (int)(WIN_HEIGHT / 2));

	scene = new RainScene();
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
	
	if (ImGui::GetIO().WantCaptureMouse == false)
	{
		Keyboard::Get()->Update();
		Mouse::Get()->Update();
	}

	scene->Update();
	Camera::Get()->Update();
}

void Program::PreRender()
{
	scene->PreRender();
}

void Program::Render()
{
	Camera::Get()->GetViewBuffer()->SetVSBuffer(1);
	Camera::Get()->GetProjection()->SetVSBuffer(2);
	Environment::Get()->GetLight()->SetPSBuffer(0);

	scene->Render();
}

void Program::PostRender()
{
	Environment::Get()->PostRender();
	Camera::Get()->PostRender();
	scene->PostRender();
}