#include "Framework.h"
#include "DungeonScene.h"

DungeonScene::DungeonScene()
{
	terrain = new Terrain(256, 256);
	terrain->LoadHeightMap(L"Textures/HeightMaps/Dungeon.png");


	vanguard = new Vanguard();
	vanguard->position = { -25,0,-25 };

	skybox = new Skybox();
	aStar = new AStar();
	maria = new Maria();

	aStar->Setup(terrain);

	maria->SetAStar(aStar);
	maria->SetTerrain(terrain);
	maria->SetPlayer(vanguard);

	vanguard->SetTerrain(terrain);

	vanguard->Check = bind(&DungeonScene::CheckEnemy, this);

	Camera::Get()->ChangeCameraMode(true);
	Camera::Get()->SetTarget(vanguard);

	CheckAstar();

	lightBuffer = new LightInfoBuffer();
	LightInfo info;
	info.type = LightInfo::SPOT;
	info.color = XMFLOAT4(1, 1, 1, 1);
	info.direction = { 0,0,1 };
	info.position = vanguard->WorldPos();

	lightBuffer->Add(info);
}

DungeonScene::~DungeonScene()
{
	delete terrain;
	delete vanguard;
	delete skybox;
	delete aStar;
	delete maria;

	delete lightBuffer;
}

void DungeonScene::Update()
{
	LightPosUpdate();

	terrain->Update();
	vanguard->Update();
	maria->Update();
	aStar->Update();

}

void DungeonScene::PreRender()
{

}

void DungeonScene::Render()
{
	lightBuffer->SetPSBuffer(2);

	// 항상 먼저 렌더링하기
	skybox->Render();

	vanguard->Render();
	maria->Render();
	terrain->Render();
	aStar->Render();
}

void DungeonScene::PostRender()
{
	terrain->PostRender();
	maria->PostRender();

	ImGui::Text("LightInfo");

	if (ImGui::Button("AddLight"))
	{
		lightBuffer->Add();
	}

	for (UINT i = 0; i < lightBuffer->data.lightCount; i++)
	{
		string menuName = "Light" + to_string(i);
		if (ImGui::BeginMenu(menuName.c_str()))
		{
			ImGui::SliderInt("Type", (int*)&lightBuffer->data.lights[i].type, 0, 3);
			ImGui::SliderFloat3("Position", (float*)&lightBuffer->data.lights[i].position, -100, 100);
			ImGui::SliderFloat("Range", &lightBuffer->data.lights[i].range, 0, 100);
			ImGui::ColorEdit4("Color", (float*)&lightBuffer->data.lights[i].color);
			ImGui::SliderFloat3("Direction", (float*)&lightBuffer->data.lights[i].direction, -1, 1);
			ImGui::SliderFloat("Outer", &lightBuffer->data.lights[i].outer, 0, 180);
			// 90 넘으면 반전
			ImGui::SliderFloat("Inner", &lightBuffer->data.lights[i].inner, 0, 90);
			ImGui::SliderFloat("Length", &lightBuffer->data.lights[i].length, 0, 100);
			ImGui::Checkbox("Active", (bool*)&lightBuffer->data.lights[i].active);

			ImGui::EndMenu();
		}
	}

}

ModelAnimator* DungeonScene::CheckEnemy()
{
	if (KEY_DOWN(VK_LBUTTON))
	{
		Ray ray = Camera::Get()->ScreenPointToRay(Mouse::Get()->GetPosition());
		if (maria->GetDamagedCollider()->IsCollision(ray))
			return maria;
		// 추가 체크
	}
	return nullptr;
}

void DungeonScene::CheckAstar()
{
	aStar->isDungeon();
}

void DungeonScene::LightPosUpdate()
{
	lightBuffer->data.lights[0].position.x = vanguard->WorldPos().x;
	lightBuffer->data.lights[0].position.z = vanguard->WorldPos().z;
	lightBuffer->data.lights[0].direction = Vector3(XMVector3TransformNormal({ 0,0,1 }, XMMatrixRotationRollPitchYawFromVector(vanguard->rotation)));
}
