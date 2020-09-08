#pragma once

class FollowCamera : public Camera
{
	friend class Camera;
public:
	FollowCamera();
	~FollowCamera();

	void Update() override;
	void PostRender() override;

	void Move() override;
	// 1인칭
	void FPMove();

	void SetTarget(Transform* value) override { target = value; }

	void MouseControl();

private:
	// xz축 기준
	float distance;
	// y축 기준
	float height;

	XMMATRIX matRotation;

	Vector3 offset;

	Vector3 destPos;
	float destRot;

	float moveDamping;
	float rotDamping;

	float rotY;
	float rotSpeed;

	float zoomSpeed;

	Transform* target;

	// 1인칭
	bool isFP;
};