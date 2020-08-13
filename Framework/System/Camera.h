#pragma once

inline void IASetPT(D3D11_PRIMITIVE_TOPOLOGY type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
	DC->IASetPrimitiveTopology(type);
}

class Camera
{
private:
	Camera();
	~Camera();

	void CreateViewort();
	void CreatePerspective();
	void CreateSamplerState();

public:
	static Camera* Get() { return instance; }
	static void Create() { instance = new Camera(); }
	static void Delete() { delete instance; }

	MatrixBuffer* GetView() { return viewBuffer; }
	MatrixBuffer* GetProjection() { return projectionBuffer; }

private:
	MatrixBuffer* viewBuffer;
	MatrixBuffer* projectionBuffer;

	SamplerState* samplerState;

	static Camera* instance;
};