#pragma once

class Tree;
class Zombie;
class WarChief;

class Terrain : Transform
{
	typedef VertexUVNormalTangentAlpha VertexType; 

	struct InputStruct
	{
		UINT index;
		XMFLOAT3 v0, v1, v2;
	};

	struct OutputStruct
	{
		UINT picked;
		float u, v, distance;
	};

private:

	Material* material;
	Mesh* mesh;

	vector<VertexType> vertices;
	vector<UINT> indices;

	UINT width, height;

	Texture* heightMap;

	ComputeShader* computeShader;
	RayBuffer* rayBuffer;

	StructuredBuffer* structuredBuffer;

	InputStruct* input;
	OutputStruct* output;

	// 폴리곤 수
	UINT size;

	Texture* secondMap;
	Texture* secondSMap;
	Texture* secondNMap;
	Texture* thirdMap;
	Texture* thirdSMap;
	Texture* thirdNMap;
	Texture* fourthMap;
	Texture* fourthSMap;
	Texture* fourthNMap;
	Texture* fifthMap;
	Texture* fifthSMap;
	Texture* fifthNMap;

	
	vector<Tree*> trees;

public:
	Terrain(UINT width, UINT height);
	~Terrain();

	void Update();
	void Render();
	void PostRender();

	// 고도(지형 높이) 측정
	float GetAltitude(Vector3 position);

	// CS 이용해서 Picking
	bool ComputePicking(OUT Vector3* position);
	bool ComputePicking(OUT Vector3* position, IN Ray ray);


	XMFLOAT2 GetSize() { return XMFLOAT2(width, height); }


	Material* GetMaterial() { return material; }

	void LoadHeightMap(wstring path);
	void LoadAlphaMap(wstring path);

	void LoadTree();
	vector<Tree*> GetTrees() { return trees; };
private:
	void CreateData();
	void CreateNormal();
	void CreateTangent();
	void CreateCompute();
};