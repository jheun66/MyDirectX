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
	info.position.y = 30.0f;
	info.outer = 80.0f;
	info.inner = 60.0f;
	info.range = 100.0f;
	lightBuffer->Add(info);


	RenderTarget* playerRenderTarget = new RenderTarget();
	RenderTarget* enemyRenderTarget = new RenderTarget();
	renderTargets.push_back(playerRenderTarget);
	renderTargets.push_back(enemyRenderTarget);

	depthStencil = new DepthStencil();

	{
		Render2D* render2D = new Render2D(L"UV");
		render2D->SetSRV(playerRenderTarget->GetSRV());

		render2D->position = { WIN_WIDTH * 0.25f, WIN_HEIGHT * 0.5f, 0.0f };
		render2D->scale = { WIN_WIDTH * 0.5f, WIN_HEIGHT, 1.0f };

		render2Ds.emplace_back(render2D);
	}

	{
		Render2D* render2D = new Render2D(L"UV");
		render2D->SetSRV(enemyRenderTarget->GetSRV());

		render2D->position = { WIN_WIDTH * 0.75f, WIN_HEIGHT * 0.5f, 0.0f };
		render2D->scale = { WIN_WIDTH * 0.5f, WIN_HEIGHT, 1.0f };

		render2Ds.emplace_back(render2D);
	}

	enemyCamera = new FollowCamera();
	enemyCamera->tag = "enemy";
	enemyCamera->SetTarget(maria);


	blendState[0] = new BlendState();
	blendState[1] = new BlendState();
	blendState[1]->Alpha(true);

}

DungeonScene::~DungeonScene()
{
	delete terrain;
	delete vanguard;
	delete skybox;
	delete aStar;
	delete maria;

	delete lightBuffer;

	for (RenderTarget* renderTarget : renderTargets)
		delete renderTarget;

	delete depthStencil;
	delete blendState[0];
	delete blendState[1];

	for (Render2D* render2D : render2Ds)
		delete render2D;

	delete enemyCamera;
}

void DungeonScene::Update()
{
	LightPosUpdate();

	terrain->Update();
	vanguard->Update();
	maria->Update();
	aStar->Update();

	for (Render2D* render2D : render2Ds)
		render2D->Update();

	enemyCamera->Update();
}

void DungeonScene::PreRender()
{
	lightBuffer->SetPSBuffer(2);

	// player
	renderTargets[0]->Set(depthStencil);
	// 항상 먼저 렌더링하기
	skybox->Render();
	vanguard->Render();
	terrain->Render();
	maria->Render();

	// 아웃라인 그려줌
	maria->PreRender();

	// enemy(카메라 다른거 세팅해서)
	enemyCamera->GetViewBuffer()->SetVSBuffer(1);
	enemyCamera->GetProjection()->SetVSBuffer(2);

	renderTargets[1]->Set(depthStencil);
	// 항상 먼저 렌더링하기
	skybox->Render();
	vanguard->Render();
	maria->Render();
	terrain->Render();
	aStar->Render();

	
}

void DungeonScene::Render()
{
	for (Render2D* render2D : render2Ds)
		render2D->Render();

	blendState[1]->SetState();
	maria->OutlineRender();
	blendState[0]->SetState();
}

void DungeonScene::PostRender()
{
	terrain->PostRender();
	

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

	enemyCamera->PostRender();
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
