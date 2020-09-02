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

public:
	Terrain(UINT width, UINT height);
	~Terrain();

	void Update();
	void Render();
	void PostRender();

	// ���� ��� �Ⱦ�
	//bool Picking(OUT Vector3* position);

	// ��(���� ����) ����
	float GetAltitude(Vector3 position);

	// CS �̿��ؼ� Picking
	bool ComputePicking(OUT Vector3* position);

	Material* GetMaterial() { return material; }
	
	// �ϴ� ������ �޾ƿ��µ� Save �Ҷ� �ٲ�����ҵ�
	void LoadTree();

	void LoadHeightMap(wstring path);
	void LoadAlphaMap(wstring path);

private:
	void CreateData();
	void CreateNormal();
	void CreateTangent();
	void CreateCompute();

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

	// ������ ��
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
};