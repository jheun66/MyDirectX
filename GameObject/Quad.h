#pragma once

class Quad : public Transform
{
public:
	Quad();
	~Quad();

	void Update();
	void Render();

	void Create();

	struct VertexUV
	{
		XMFLOAT3 position;
		XMFLOAT2 uv;

		VertexUV() :position(0, 0, 0), uv(0, 0)
		{

		}
	};

private:
	VertexShader* vertexShader;
	PixelShader* pixelShader;
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

	ID3D11ShaderResourceView* srv;

	RasterizerState* rasterizerState[2];

};