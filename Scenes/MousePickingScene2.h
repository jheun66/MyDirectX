#pragma once

class MousePickingScene2 : public Scene
{
public:
	MousePickingScene2();
	~MousePickingScene2();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

private:
	Terrain* terrain;
	Vanguard* vanguard;
	vector<Zombie*> zombies;
};