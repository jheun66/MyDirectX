#pragma once

class Grid : public Transform
{

public:
	Grid(wstring texturePath = L"", wstring heightMapPath = L"");
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

	//�߽��� ����
	vector<VertexColor> pivotVertices;
	VertexBuffer* vertexBuffer2;
	VertexShader* vertexShader2;
	PixelShader* pixelShader2;

	bool viewFrame = false;
	bool viewNormal = false;
};