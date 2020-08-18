#pragma once

#include "GameObject/Sphere.h"
#include "GameObject/Star.h"

class SphereScene : public Scene
{
private:
	Star* sun;
	Star* earth;
	Star* moon;

public:
	SphereScene();
	~SphereScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

};