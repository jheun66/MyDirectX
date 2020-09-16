#pragma once

class MonsterAStarScene : public Scene
{
private:
	Maria* maria = nullptr;
	Vanguard* vanguard = nullptr;

	AStar* aStar = nullptr;
	Terrain* terrain = nullptr;
	Skybox* skybox = nullptr;

public:
	MonsterAStarScene();
	~MonsterAStarScene();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

	ModelAnimator* CheckEnemy();
	void CheckTree();
};