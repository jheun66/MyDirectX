#pragma once

#include "GameObject/Sphere.h"
#include "GameObject/Star.h"
#include "GameObject/Planet.h"

class SolarSystemScene : public Scene
{
private:
	Star* sun;
	Planet* earth;
	Planet* moon;

	Planet* mercury;
	Planet* venus;
	Planet* mars;
	Planet* jupiter;

public:
	SolarSystemScene();
	~SolarSystemScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

};
