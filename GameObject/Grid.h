#pragma once

class Grid : public Transform
{

public:
	Grid();
	~Grid();

	void CreateData();
	void CreateNormal();

	void PostRender();
	void Render();

private:
	vector<VertexUVNormal> vertices;
	vector<UINT> indices;
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;
	UINT width;
	UINT height;

	VertexShader* vertexShader;
	PixelShader* pixelShader;

	MatrixBuffer* worldBuffer;
	LightBuffer* lightBuffer;

	RasterizerState* rasterizerState[2];
	Texture* texture;
	Texture* heightMap;

};