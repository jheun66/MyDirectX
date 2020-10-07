#pragma once

class Shadow
{
private:
	UINT width, height;
	float radius;

	RenderTarget* renderTarget;
	DepthStencil* depthStencil;

	class Render2D* depthMap;

	MatrixBuffer* viewBuffer;
	MatrixBuffer* projectionBuffer;

	SettingBuffer* qualityBuffer;
	SizeBuffer* sizeBuffer;

	// 누구 중심으로 그림자를 그려줄지, 주로 주인공 근체에만 적용하게
	Vector3 offset;

public:
	Shadow(UINT width = 2000, UINT height = 2000);
	~Shadow();

	void PreRender(Vector3 offset);
	void Render();
	void PostRender();

	void SetViewProjection(Vector3 offset);

};