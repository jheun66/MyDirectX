#pragma once

class Camera : public Transform
{
public:
	Camera(string tag);
	~Camera();

	void Rotation();
	void Move();

	void Update();
	void PostRender();
	void CreatePerspective();
	void CreateView();

	ViewBuffer* GetView() { return viewBuffer; }
	MatrixBuffer* GetProjection() { return projectionBuffer; }

private:	
	Vector3 forward;
	Vector3 right;
	Vector3 up;

	XMMATRIX matRotation;
	XMMATRIX matView;

	ViewBuffer* viewBuffer;
	MatrixBuffer* projectionBuffer;

	POINT curPos;
	POINT oldPos;

	float FOV;

	float moveSpeed;
	float rotSpeed;
};