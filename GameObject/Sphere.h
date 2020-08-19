#pragma once

class Sphere : public Transform
{
public:
	Sphere();
	~Sphere();

	virtual void Update();
	virtual void Render();
	virtual void PostRender();

	// ÀÚÀü
	virtual void Rotation();

	void Create();

	//void SetColor(XMFLOAT4 color) { colorBuffer->data.color = color; }

protected:
	vector<Vertex> vertices;
	vector<UINT> indices;

	VertexShader* vertexShader;
	PixelShader* pixelShader;
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

	//ColorBuffer* colorBuffer;

	UINT indexCnt;

	float rotSpeed = 10.0f;
};