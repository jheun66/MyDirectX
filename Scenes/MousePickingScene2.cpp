#include "framework.h"
#include "MousePickingScene2.h"

MousePickingScene2::MousePickingScene2()
{
	terrain = new Terrain(200, 200);
	vanguard = new Vanguard();
	vanguard->SetCollider(new SphereCollider(10.0f));
	vanguard->SetTerrain(terrain);

	zombie = new Zombie();
	zombie->offset.position = { (float)GameMath::Random(20, 100), 0, (float)GameMath::Random(20, 100) };

	zombie->SetTerrain(terrain);
	zombie->SetSpeed(GameMath::Random(7, 12));
	Collider* zombieCollider = new SphereCollider(10.0f);
	zombie->SetCollider(zombieCollider);
	vanguard->SetEnemy(zombie);

	Camera::Get()->ChangeCameraMode(false);
	//Camera::Get()->SetTarget(vanguard);
}

MousePickingScene2::~MousePickingScene2()
{
	
	delete zombie;
	
	delete terrain;
	delete vanguard;
}

void MousePickingScene2::Update()
{
	terrain->Update();
	vanguard->Update();
	zombie->Update();
}

void MousePickingScene2::PreRender()
{
}

void MousePickingScene2::Render()
{
	terrain->Render();
	vanguard->Render();
	zombie->Render();
}

void MousePickingScene2::PostRender()
{

}
