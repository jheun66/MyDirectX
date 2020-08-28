#include "framework.h"
#include "TerrainEditorScene.h"

TerrainEditorScene::TerrainEditorScene()
{
	terrainEditor = new TerrainEditor(200, 200);
	settingBuffer = new MSBuffer();
}

TerrainEditorScene::~TerrainEditorScene()
{
	delete terrainEditor;
}

void TerrainEditorScene::Update()
{
	terrainEditor->Update();
}

void TerrainEditorScene::PreRender()
{
}

void TerrainEditorScene::Render()
{
	settingBuffer->SetBufferToPS(1);
	terrainEditor->Render();
}

void TerrainEditorScene::PostRender()
{
	terrainEditor->PostRender();
}
