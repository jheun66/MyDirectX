#include "framework.h"
#include "MonsterAStarScene.h"

MonsterAStarScene::MonsterAStarScene()
{
	terrain = new Terrain(200, 200);
	vanguard = new Vanguard();
	skybox = new Skybox();
	aStar = new AStar();
	maria = new Maria();

	aStar->Setup(terrain);

	maria->SetAStar(aStar);
	maria->SetTerrain(terrain);
	maria->SetPlayer(vanguard);
	
	vanguard->SetTerrain(terrain);

	vanguard->Check = bind(&MonsterAStarScene::CheckEnemy, this);
	
	Camera::Get()->ChangeCameraMode(true);
	Camera::Get()->SetTarget(vanguard);

	CheckTree();
}

MonsterAStarScene::~MonsterAStarScene()
{
	delete terrain;
	delete vanguard;
	delete skybox;
	delete aStar;
	delete maria;
}

void MonsterAStarScene::Update()
{
	terrain->Update();
	vanguard->Update();
	maria->Update();
	aStar->Update();
}

void MonsterAStarScene::PreRender()
{
}

void MonsterAStarScene::Render()
{
	// �׻� ���� �������ϱ�
	skybox->Render();

	vanguard->Render();
	maria->Render();
	terrain->Render();
	aStar->Render();
}

void MonsterAStarScene::PostRender()
{
	maria->PostRender();
}

ModelAnimator* MonsterAStarScene::CheckEnemy()
{
	if (KEY_DOWN(VK_LBUTTON))
	{
		Ray ray = Camera::Get()->ScreenPointToRay(Mouse::Get()->GetPosition());
		if(maria->GetDamagedCollider()->IsCollision(ray))
			return maria;
		// �߰� üũ
	}
	return nullptr;
}

// astar�� ���� tree ������Ʈ �� �Ŀ� �ؾ���
void MonsterAStarScene::CheckTree()
{
	aStar->Update();
	terrain->Update();
	vector<Tree*> trees = terrain->GetTrees();
	for (Tree* tree : trees)
	{
		aStar->isObstacle(tree->GetCollider());
	}

}
