#include "framework.h"
#include "MoveModelScene.h"

MoveModelScene::MoveModelScene()
{
	terrain = new Terrain(200, 200);
	warHawk = new WarHawk();

	settingBuffer = new MSBuffer();
}

MoveModelScene::~MoveModelScene()
{
	delete settingBuffer;
	delete terrain;
}

void MoveModelScene::Update()
{
	terrain->Update();
	float altitude = terrain->GetAltitude(Vector3(warHawk->position.x, 0, warHawk->position.z));
	warHawk->Update(altitude);
	UpdateCamera();
}

void MoveModelScene::PreRender()
{
}

void MoveModelScene::Render()
{
	settingBuffer->SetPSBuffer(1);
	terrain->Render();
	warHawk->Render();
}

void MoveModelScene::PostRender()
{
	terrain->PostRender();
	warHawk->PostRender();
}

void MoveModelScene::UpdateCamera()
{
	Vector3 offset = { 0,1500, -2500 };
	offset = XMVector3TransformNormal(offset, *warHawk->GetWorld());
	Camera::Get()->position = warHawk->position + offset;
	Camera::Get()->rotation = warHawk->rotation + Vector3(XM_PIDIV4 / 2, 0, 0);
}
