#pragma once

class WarHawk : public Model
{
public:
	WarHawk();
	~WarHawk();

	void Render() override;
	void Update(float altitude);
	void PostRender();

	void Move(float altitude);
	void Rotate();
private:
	SettingBuffer* settingBuffer;

	// 회전을 위해
	POINT mCurPos;
	POINT mOldPos;

	float moveSpeed;

	XMMATRIX matRotation;
	Vector3 forward;
	Vector3 right;
	Vector3 up;

	Vector3 origin;
};