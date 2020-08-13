#pragma once

class Plane : public Transform
{
public:
	Plane(UINT, UINT);
	~Plane();

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
	bool isFrame = false;

private:
	VertexShader* vertexShader;
	PixelShader* pixelShader;
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

	ID3D11ShaderResourceView* srv;

	RasterizerState* rasterizerState[2];
	
	UINT indexCount = 0;
	UINT row = 0;
	UINT col = 0;
};