#pragma once

#include "GameObject/Sphere.h"

class SphereScene : public Scene
{
private:
	Sphere* sun;
	Sphere* earth;
	Sphere* moon;

public:
	SphereScene();
	~SphereScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

};