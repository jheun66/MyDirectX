#include "Framework.h"
#include "ParticleSystemScene.h"

ParticleSystemScene::ParticleSystemScene()
{
	terrain = new Terrain(256, 256);
	ps = new ParticleSystem();
}

ParticleSystemScene::~ParticleSystemScene()
{
	delete terrain;
	delete ps;
}

void ParticleSystemScene::Update()
{
	if (KEY_DOWN(VK_LBUTTON))
	{
		Vector3 pickPos;
		terrain->ComputePicking(&pickPos);

		// Nothing
	}
	terrain->Update();
	ps->Update();
}

void ParticleSystemScene::PreRender()
{
}

void ParticleSystemScene::Render()
{
	terrain->Render();
	ps->Render();
}

void ParticleSystemScene::PostRender()
{
	ps->PostRender();
}
