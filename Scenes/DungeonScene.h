#pragma once

class DungeonScene : public Scene
{
private:
	Terrain* terrain = nullptr;
	LightInfoBuffer* lightBuffer = nullptr;

	Maria* maria = nullptr;
	Vanguard* vanguard = nullptr;

	AStar* aStar = nullptr;
	Skybox* skybox = nullptr;



public:
	DungeonScene();
	~DungeonScene();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

	ModelAnimator* CheckEnemy();

	void CheckAstar();
	void LightPosUpdate();
};