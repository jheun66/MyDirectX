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

	

	MatrixBuffer* worldBuffer;

	RasterizerState* rasterizerState[2];

	Material* material;
	Texture* heightMap;

	//중심점 버퍼
	vector<VertexColor> pivotVertices;
	VertexBuffer* vertexBuffer2;
	VertexShader* vertexShader2;
	PixelShader* pixelShader2;

	bool viewFrame = false;
	bool viewNormal = false;
};