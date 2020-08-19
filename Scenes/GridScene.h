#pragma once

#include "GameObject/Grid.h"
#include "GameObject/Sphere.h"
#include "GameObject/Planet.h"

class GridScene : public Scene
{
public:
	GridScene();
	~GridScene();


	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

private:
	Grid* grid = nullptr;
	Planet* planet = nullptr;

	DirLightBuffer* dirLBuffer;
};