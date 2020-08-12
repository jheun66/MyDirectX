#pragma once

#include "GameObject/Robot.h"

class RobotScene : public Scene
{

public:
	RobotScene();
	~RobotScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

private:
	Robot* robot = nullptr;
	Robot* robot2 = nullptr;
	Robot* robot3 = nullptr;
};