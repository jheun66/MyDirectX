#include "framework.h"
#include "RainScene.h"

RainScene::RainScene()
{
	terrain = new Terrain(256, 256);

	player = new Vanguard();
	player->SetTerrain(terrain);

	rainMist = new RainMist();
	rainFall = new RainFall();
	rainFall->SetTerrain(terrain);
	rainFall->Explosion = bind(&RainScene::ExplosionFunc, this, placeholders::_1);

}

RainScene::~RainScene()
{
	delete terrain;
	delete player;

	delete rainMist;
	delete rainFall;
	for (RainExplosion* explosion : explosions)
		delete explosion;
}

void RainScene::Update()
{
	terrain->Update();
	player->Update();

	rainMist->Update();
	rainFall->Update();
	for (UINT i = 0; i < explosions.size(); i++)
	{
		explosions[i]->Update();
		if (!explosions[i]->IsPlay())
		{
			delete explosions[i];
			explosions.erase(explosions.begin() + i);
		}
	}
}

void RainScene::PreRender()
{
}

void RainScene::Render()
{
	terrain->Render();
	player->Render();

	rainMist->Render();
	rainFall->Render();
	for (RainExplosion* explosion : explosions)
		explosion->Render();
}

void RainScene::PostRender()
{
	rainFall->PostRender();
	ImGui::Text("Explosion Total particles counts : %d", explosions.size() * 32);

}

void RainScene::ExplosionFunc(Vector3 pos)
{
	RainExplosion* explosion = new RainExplosion();
	explosion->Play(pos);
	explosions.emplace_back(explosion);
}
