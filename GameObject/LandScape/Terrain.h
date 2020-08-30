#pragma once

class Terrain : Transform
{
	typedef VertexUVNormalTangent VertexType;

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
	Terrain();
	~Terrain();

	void Update();
	void Render();
	void PostRender();

	bool Picking(OUT Vector3* position);
	float GetAltitude(Vector3 position);

	// CS �̿��ؼ� Picking
	bool ComputePicking(OUT Vector3* position);

	Material* GetMaterial() { return material; }

private:
	void CreateData();
	void CreateNormal();
	void CreateTangent();

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
};