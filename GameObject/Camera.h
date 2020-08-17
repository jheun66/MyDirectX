#pragma once

class Camera : public Transform
{
public:
	Camera(string tag);
	~Camera();

	void Update();
	void PostRender();
	void CreatePerspective();

	MatrixBuffer* GetView() { return viewBuffer; }
	MatrixBuffer* GetProjection() { return projectionBuffer; }

private:
	MatrixBuffer* viewBuffer;
	MatrixBuffer* projectionBuffer;

	POINT  cursorPos;
	bool isDrag = false;
	XMFLOAT2 dragedPosition;


};