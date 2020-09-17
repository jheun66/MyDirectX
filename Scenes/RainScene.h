#pragma once
#include "GameObject/LandScape/Billboard.h"

class RainScene : public Scene
{
private:
	Terrain* terrain;
	Vanguard* player;
	
	RainMist* rainMist;
	vector<RainExplosion*> explosions;
	RainFall* rainFall;

public:
	RainScene();
	~RainScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

	void ExplosionFunc(Vector3 pos);
};