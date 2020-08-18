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

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

};