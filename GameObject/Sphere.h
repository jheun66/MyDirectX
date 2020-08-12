#pragma once

class Sphere : public Transform
{
public:
	Sphere();
	~Sphere();

	void Update();
	void Render();

	void Create();

	void SetColor(XMFLOAT4 color) { colorBuffer->data.color = color; }

	struct Vertex
	{
		XMFLOAT3 position;

		Vertex() : position(0, 0, 0)
		{

		}

	};

private:
	VertexShader* vertexShader;
	PixelShader* pixelShader;
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

	ColorBuffer* colorBuffer;

	UINT indexCnt;
};