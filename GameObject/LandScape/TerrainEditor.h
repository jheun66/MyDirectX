#pragma once
class TerrainEditor : public Transform
{
	typedef VertexUVNormalAlpha VertexType;

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

public:
	TerrainEditor(UINT width, UINT height);
	~TerrainEditor();

	void Update();
	void Render();
	void PostRender();

	bool ComputePicking(OUT Vector3* position);

	void AdjustY(Vector3 position, float value);
	void PaintBrush(Vector3 position, float value);

	void Save();
	void Load();

	void SaveHeightMap();

private:
	void CreateData();
	void CreateNormal();
	void CreateCompute();
	

private:
	Material* material;
	Mesh* mesh;

	vector<VertexType> vertices;
	vector<UINT> indices;

	UINT width, height;

	ComputeShader* computeShader;
	RayBuffer* rayBuffer;

	StructuredBuffer* structuredBuffer;

	InputStruct* input;
	OutputStruct* output;

	UINT size;

	BrushBuffer* brushBuffer;

	bool isRaise;
	float adjustValue;

	bool isPainting;
	float paintValue;

	int selectMap;

	// ≥Ù¿Ã∏  ¿˙¿Â¿ª ¿ß«— ∫§≈Õ
	vector<float> heights;

	//Texture* alphaMap;
	Texture* secondMap;
	Texture* thirdMap;


	Texture* heightMap;
};