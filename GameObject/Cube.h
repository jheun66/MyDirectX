#pragma once

class Cube : public Transform
{
public:
	Cube();
	~Cube();

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

};