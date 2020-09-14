#include "framework.h"

Skybox::Skybox()
{
	// shader���� ó���ϸ鼭 ũ�Ⱑ �ǹ̾���
	sphere = new Sphere(L"SkyBox");
	sphere->GetFillMode()->FrontCounterClockWise(true);

	cubeMap = Texture::Add(L"Textures/Landscape/NightMoon.dds");

	depthMode[0] = new DepthStencilState();
	depthMode[1] = new DepthStencilState();

	depthMode[1]->DepthEnable(false);
}

Skybox::~Skybox()
{
	delete sphere;
	delete depthMode[0];
	delete depthMode[1];
}

void Skybox::Render()
{
	cubeMap->PSSet(10);

	depthMode[1]->SetState();
	sphere->Render();
	depthMode[0]->SetState();
}
