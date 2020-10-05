#include "framework.h"
#include "MousePickingScene.h"

MousePickingScene::MousePickingScene()
{
	terrain = new Terrain(100, 100);
	sphere = new Sphere();

	terrain->GetMaterial()->SetDiffuseMap(L"rocks_ground_01_diff_1k.png");
	terrain->GetMaterial()->SetNormalMap(L"rocks_ground_01_nor_1k.png");

	sphere->GetMaterial()->SetDiffuseMap(L"aerial_grass_rock_diff_1k.png");
	sphere->GetMaterial()->SetNormalMap(L"aerial_grass_rock_nor_1k.png");
	sphere->position = { 0,1,0 };


	settingBuffer = new SettingBuffer();

	GetCursorPos(&mOldPos);
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
		terrain->ComputePicking(&pickingPos);
		curPos = sphere->position;
	}

	MoveToPickingPos();
	RotateSphere();

	terrain->Update();
	sphere->Update();
}

void MousePickingScene::PreRender()
{
}

void MousePickingScene::Render()
{
	settingBuffer->SetPSBuffer(1);
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

	Vector3 offset = { 0,10,-10 };
	offset = XMVector3TransformNormal(offset, *sphere->GetWorld());
	Camera::Get()->position = Vector3(sphere->position.x, terrain->GetAltitude(sphere->position), sphere->position.z) + offset;
	Camera::Get()->rotation = sphere->rotation;
}

void MousePickingScene::RotateSphere()
{
	// 모니터의 좌표를 구함(마우스가 화면 밖으로 나가도 상관 x)
	GetCursorPos(&mCurPos);
	Vector3 curVec = { (float)mCurPos.x, (float)mCurPos.y, 0 };
	Vector3 oldVec = { (float)mOldPos.x, (float)mOldPos.y, 0 };

	Vector3 dif = curVec - oldVec;

	if (Mouse::Get()->Press(1))
	{
		sphere->rotation.x += dif.y * 5.0f * Time::Delta();
		sphere->rotation.y += dif.x * 5.0f * Time::Delta();

		XMMATRIX rotY = XMMatrixRotationY(sphere->rotation.y);
		XMMATRIX rotX = XMMatrixRotationX(sphere->rotation.x);
		
	}

	mOldPos = mCurPos;
}
