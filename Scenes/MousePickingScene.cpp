#include "framework.h"
#include "MousePickingScene.h"

MousePickingScene::MousePickingScene()
{
	terrain = new Terrain();
	sphere = new Sphere();

	terrain->GetMaterial()->SetDiffuseMap(L"rocks_ground_01_diff_1k.png");
	terrain->GetMaterial()->SetNormalMap(L"rocks_ground_01_nor_1k.png");

	sphere->GetMaterial()->SetDiffuseMap(L"aerial_grass_rock_diff_1k.png");
	sphere->GetMaterial()->SetNormalMap(L"aerial_grass_rock_nor_1k.png");
	sphere->position = { 0,1,0 };


	settingBuffer = new MSBuffer();
}

MousePickingScene::~MousePickingScene()
{
	delete settingBuffer;
	delete sphere;
	delete terrain;
}

void MousePickingScene::Update()
{
	if (Mouse::Get()->Down(0))
	{
		terrain->Picking(&pickingPos);
		curPos = sphere->position;
	}

	MoveToPickingPos();
	

	terrain->Update();
	sphere->Update();
}

void MousePickingScene::PreRender()
{
}

void MousePickingScene::Render()
{
	settingBuffer->SetBufferToPS(1);
	terrain->Render();
	sphere->Render();
}

void MousePickingScene::PostRender()
{
	ImGui::SliderFloat3("PickingPos", (float*)&pickingPos, 0, 500);

	altitude = terrain->GetAltitude(pickingPos);
	ImGui::Text("Altitude : %f", altitude);
}

void MousePickingScene::MoveToPickingPos()
{
	static float sinWave = 0.0f;
	sinWave += Time::Delta() * 10;
	if (!Vector3::Approximation(sphere->position, pickingPos, 0.001f))
	{
		Vector3 tmpPos1 = sphere->position;
		Vector3 tmpPos2 = pickingPos;

		Vector3 tmp3 = tmpPos2 - tmpPos1;

		XMMATRIX mat = XMMatrixTranslationFromVector((tmp3).Normal() / 50.0f);

		sphere->position = XMVector3TransformCoord(tmpPos1, mat);
	}
	sphere->position.y = terrain->GetAltitude(sphere->position) + 2 + sin(sinWave);
}
