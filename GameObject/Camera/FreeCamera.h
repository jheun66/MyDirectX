#pragma once

class FreeCamera : public Camera
{
	friend class Camera;
private:
	FreeCamera();
	~FreeCamera();

	void Move() override;
	void Rotation() override;
public:
	void Update() override;
	void PostRender() override;

private:
	float moveSpeed;
	float rotSpeed;
};
