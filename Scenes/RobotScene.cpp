#include "Framework.h"

#include "RobotScene.h"

RobotScene::RobotScene()
{
	robot = new Robot();
	robot2 = new Robot();
	robot3 = new Robot();

	robot2->position = { 20,0,0 };
	robot3->position = { -20,0,0 };
}

RobotScene::~RobotScene()
{
	delete robot;
	delete robot2;
	delete robot3;
}

void RobotScene::Update()
{
	robot->Update();
	robot2->Update();
	robot3->Update();
}

void RobotScene::PreRender()
{
}

void RobotScene::Render()
{
	robot->Render();
	robot2->Render();
	robot3->Render();
}

void RobotScene::PostRender()
{
}
