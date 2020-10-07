#include "Framework.h"
#include "ShadowScene.h"

ShadowScene::ShadowScene()
{
	//plane = new ModelRender("Basic/Plane");
	terrain = new Terrain(256, 256);
	terrain->LoadHeightMap(L"Textures/HeightMaps/HeightMap.png");

	vanguard = new Vanguard();
	vanguard->SetTerrain(terrain);

	settingBuffer = new SettingBuffer();
	
	shadow = new Shadow();
}

ShadowScene::~ShadowScene()
{
	//delete plane;
	delete terrain;
	delete vanguard;

	delete settingBuffer;
	delete shadow;
}

void ShadowScene::Update()
{
	//plane->Update();
	terrain->Update();
	vanguard->Update();
}

void ShadowScene::PreRender()
{
	shadow->PreRender(vanguard->WorldPos());

	//plane->SetShader(L"DepthMap");
	terrain->SetShader(L"DepthMap");
	vanguard->SetShader(L"DepthMap");

	// 葛胆 坊歹 可记
	settingBuffer->data.option[0] = 0;
	settingBuffer->SetVSBuffer(10);
	//plane->Render();
	terrain->Render();

	// 局聪皋捞记 可记
	settingBuffer->data.option[0] = 2;
	settingBuffer->SetVSBuffer(10);
	vanguard->Render();
}

void ShadowScene::Render()
{
	shadow->Render();

	//plane->SetShader(L"Shadow");
	terrain->SetShader(L"Shadow");
	vanguard->SetShader(L"Shadow");

	settingBuffer->data.option[0] = 0;
	settingBuffer->SetVSBuffer(10);
	//plane->Render();
	terrain->Render();

	settingBuffer->data.option[0] = 2;
	settingBuffer->SetVSBuffer(10);
	vanguard->Render();
}

void ShadowScene::PostRender()
{
	shadow->PostRender();
}