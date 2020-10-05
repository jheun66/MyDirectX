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
	void RotateSphere();
private:
	Terrain* terrain;
	Sphere* sphere;

	// ���콺 ��ŷ
	Vector3 pickingPos;
	float altitude;
	Vector3 curPos;

	SettingBuffer* settingBuffer;

	// ȸ���� ����
	POINT mCurPos;
	POINT mOldPos;
};