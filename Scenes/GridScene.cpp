#include "Framework.h"
#include "GridScene.h"

GridScene::GridScene()
{
	grid = new Grid();
}

GridScene::~GridScene()
{
	delete grid;
}

void GridScene::Update()
{
}

void GridScene::PreRender()
{
}

void GridScene::Render()
{
	grid->Render();
}

void GridScene::PostRender()
{
	grid->PostRender();
}
