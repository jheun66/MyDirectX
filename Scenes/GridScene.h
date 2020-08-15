#pragma once

class GridScene : public Scene
{
public:
	GridScene();
	~GridScene();


	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

private:
	Grid* grid = nullptr;

};