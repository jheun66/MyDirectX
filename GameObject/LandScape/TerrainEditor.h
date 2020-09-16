#pragma once

class Tree;

class TerrainEditor : public Transform
{
private:
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

	class BrushBuffer : public ConstBuffer
	{
	public:
		struct Data
		{
			int type;
			XMFLOAT3 location;

			float range;
			XMFLOAT3 color;
		}data;
		BrushBuffer() : ConstBuffer(&data, sizeof(Data))
		{
			data.type = 1;
			data.location = XMFLOAT3(0, 0, 0);

			data.range = 10.0f;
			data.color = XMFLOAT3(0, 1, 0);
		}

	};

	/// <summary>
	/// ComputeAdjustY 용
	struct InputVertices
	{
		UINT index;
		XMFLOAT3 pos;
	};

	struct OutputVertices
	{
		UINT inside;
		float distance;
	};

	class HeightBrushBuffer : public ConstBuffer
	{
	public:
		struct Data
		{
			int type;
			XMFLOAT3 location;

			float range;
			UINT outputSize;
			XMFLOAT2 dummy;
		}data;
		HeightBrushBuffer() : ConstBuffer(&data, sizeof(Data))
		{
			data.type = 1;
			data.location = XMFLOAT3(0, 0, 0);

			data.range = 10.0f;
			data.outputSize = 0;
		}
	};
	/// </summary>

private:
	Material* material;
	Mesh* mesh;

	vector<VertexType> vertices;
	vector<UINT> indices;

	UINT width, height;

	ComputeShader* computeShader[2];
	RayBuffer* rayBuffer;

	StructuredBuffer* structuredBuffer[2];

	InputStruct* input;
	OutputStruct* output;

	UINT size;

	BrushBuffer* brushBuffer;

	int isRaise;
	float adjustValue;

	int mode;
	float paintValue;

	int selectMap;

	// 높이맵 저장을 위한 벡터
	vector<float> heights;


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

	XMFLOAT3 objectRotation;
	XMFLOAT3 objectScale;

	InputVertices* inputVertices;
	OutputVertices* outputVertices;
	HeightBrushBuffer* heightBrushBuffer;

public:
	TerrainEditor(UINT width, UINT height);
	~TerrainEditor();

	void Update();
	void Render();
	void PostRender();

	bool ComputePicking(OUT Vector3* position);

	void AdjustY(Vector3 position, float value);
	void ComputeAdjustY(Vector3 position, float value);

	void ComputePaintBrush(Vector3 position, float value);
	void CreateTree(Vector3 position, Vector3 rotation, Vector3 scale);

	void SaveTree();
	void LoadTree();

	void SaveHeightMap(wstring path);
	void SaveAlphaMap(wstring path);

	void LoadHeightMap(wstring path);
	void LoadAlphaMap(wstring path);

	void Brushing();

private:
	void CreateData();
	void CreateNormal();
	void CreateTangent();
	void CreateCompute();
	
	// AdjustY에서 사용할 input, output   한번만 호출하면 됨
	void CreateComputeForY();


};