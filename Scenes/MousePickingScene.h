#pragma once

class MousePickingScene : public Scene
{
public:
	MousePickingScene();
	~MousePickingScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

private:
	void MoveToPickingPos();

private:
	Terrain* terrain;
	Sphere* sphere;

	// 마우스 피킹
	Vector3 pickingPos;
	float altitude;
	Vector3 curPos;

	MSBuffer* settingBuffer;
};