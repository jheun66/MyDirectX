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

	// ���� �߽����� �׸��ڸ� �׷�����, �ַ� ���ΰ� ��ü���� �����ϰ�
	Vector3 offset;

public:
	Shadow(UINT width = 2000, UINT height = 2000);
	~Shadow();

	void PreRender(Vector3 offset);
	void Render();
	void PostRender();

	void SetViewProjection(Vector3 offset);

};