#include "Framework.h"
#include "Terrain.h"

Terrain::Terrain(UINT width, UINT height)
	:width(width), height(height)
{
	material = new Material(L"SplattingAdvanced");
	material->SetDiffuseMap(L"Textures/Terrain/brown_mud_leaves_01_diff_1k.png");
	material->SetSpecularMap(L"Textures/Terrain/brown_mud_leaves_01_spec_1k.png");
	material->SetNormalMap(L"Textures/Terrain/brown_mud_leaves_01_Nor_1k.png");

	CreateData();
	CreateNormal();
	CreateTangent();

	mesh = new Mesh(vertices.data(), sizeof(VertexType), vertices.size(),
		indices.data(), indices.size());

	CreateCompute();
	
	secondMap = Texture::Add(L"Textures/Terrain/brown_mud_dry_diff_1k.png");
	secondSMap = Texture::Add(L"Textures/Terrain/brown_mud_dry_spec_1k.png");
	secondNMap = Texture::Add(L"Textures/Terrain/brown_mud_dry_nor_1k.png");
	thirdMap = Texture::Add(L"Textures/Terrain/coral_mud_01_diff_1k.png");
	thirdSMap = Texture::Add(L"Textures/Terrain/coral_mud_01_spec_1k.png");
	thirdNMap = Texture::Add(L"Textures/Terrain/coral_mud_01_Nor_1k.png");
	fourthMap = Texture::Add(L"Textures/Terrain/snow_02_diff_1k.png");
	fourthSMap = Texture::Add(L"Textures/Terrain/snow_02_spec_1k.png");
	fourthNMap = Texture::Add(L"Textures/Terrain/snow_02_nor_1k.png");
	fifthMap = Texture::Add(L"Textures/Terrain/brown_mud_rocks_01_diff_1k.png");
	fifthSMap = Texture::Add(L"Textures/Terrain/brown_mud_rocks_01_spec_1k.png");
	fifthNMap = Texture::Add(L"Textures/Terrain/brown_mud_rocks_01_nor_1k.png");
	
	LoadHeightMap(L"Textures/HeightMaps/TestHeightMap.png");
	LoadAlphaMap(L"Textures/HeightMaps/TestAlphaMap.png");
	LoadTree();
}

Terrain::~Terrain()
{
	delete material;
	delete mesh;

	delete rayBuffer;
	delete structuredBuffer;

	delete[] input;
	delete[] output;


	if (trees.size())
	{
		for (auto tree : trees)
			delete tree;
	}
}

void Terrain::Update()
{
	UpdateWorld();

	if (trees.size())
	{
		for (auto tree : trees)
			tree->Update();
	}
}

void Terrain::Render()
{
	mesh->Set();

	SetWorldBuffer();

	secondMap->PSSet(10);
	secondSMap->PSSet(11);
	secondNMap->PSSet(12);
	thirdMap->PSSet(20);
	thirdSMap->PSSet(21);
	thirdNMap->PSSet(22);
	fourthMap->PSSet(30);
	fourthSMap->PSSet(31);
	fourthNMap->PSSet(32);
	fifthMap->PSSet(40);
	fifthSMap->PSSet(41);
	fifthNMap->PSSet(42);

	material->Set();

	DC->DrawIndexed(indices.size(), 0, 0);

	if (trees.size())
	{
		for (auto tree : trees)
			tree->Render();
	}
}

void Terrain::PostRender()
{
	Vector3 pickingPos;
	ComputePicking(&pickingPos);
	ImGui::SliderFloat3("PickingPos", (float*)&pickingPos, 0, 500);

	float height = GetAltitude(pickingPos);
	ImGui::Text("Height : %f", height);
}

// 기존 방식 안씀
//bool Terrain::Picking(OUT Vector3* position)
//{
//	Ray ray = Environment::Get()->MainCamera()->ScreenPointToRay(Mouse::Get()->GetPosition());
//
//	for (UINT z = 0; z < height; z++)
//	{
//		for (UINT x = 0; x < width; x++)
//		{
//			UINT index[4];
//			index[0] = (width + 1) * z + x;
//			index[1] = (width + 1) * z + x + 1;
//			index[2] = (width + 1) * (z + 1) + x;
//			index[3] = (width + 1) * (z + 1) + x + 1;
//
//			Vector3 p[4];
//			for (UINT i = 0; i < 4; i++)
//				p[i] = vertices[index[i]].position;
//
//			float distance;
//			if (TriangleTests::Intersects(ray.position, ray.direction, p[0], p[1], p[2], distance))
//			{
//				*position = ray.position + ray.direction * distance;
//				return true;
//			}
//
//			if (TriangleTests::Intersects(ray.position, ray.direction, p[3], p[1], p[2], distance))
//			{
//				*position = ray.position + ray.direction * distance;
//				return true;
//			}
//		}
//	}
//
//	return false;
//}

float Terrain::GetAltitude(Vector3 position)
{
	// 버림
	UINT x = (UINT)position.x;
	UINT z = (UINT)position.z;

	// Vertex 기준으로 생각하기 (0 ~ width-1)(0 ~ height-1)까지 있으면 index는 그보다 1 작음
	if (x < 0 || x >= width - 1) return 0.0f;
	if (z < 0 || z >= height - 1) return 0.0f;

	UINT index[4];
	index[0] = (width) * z + x;
	index[1] = (width) * (z + 1) + x;
	index[2] = (width) * z + x + 1;
	index[3] = (width) * (z + 1) + x + 1;

	Vector3 p[4];
	for (UINT i = 0; i < 4; i++)
		p[i] = vertices[index[i]].position;

	float u = position.x - p[0].x;
	float v = position.z - p[0].z;

	Vector3 result;
	if (u + v <= 1)
	{
		result = p[0] + (p[2] - p[0]) * u + (p[1] - p[0]) * v;
	}
	else
	{
		u = 1.0f - u;
		v = 1.0f - v;

		result = p[3] + (p[1] - p[3]) * u + (p[2] - p[3]) * v;
	}

	return result.y;
}

bool Terrain::ComputePicking(OUT Vector3* position)
{
	Ray ray = Camera::Get()->ScreenPointToRay(Mouse::Get()->GetPosition());

	rayBuffer->data.position = ray.position;
	rayBuffer->data.direction = ray.direction;
	rayBuffer->data.size = size;
	computeShader->Set();

	rayBuffer->SetCSBuffer(0);

	DC->CSSetShaderResources(0, 1, &structuredBuffer->GetSRV());
	DC->CSSetUnorderedAccessViews(0, 1, &structuredBuffer->GetUAV(), nullptr);

	// ceil 올림				1024 쓰레드 개수
	UINT x = ceil((float)size / 1024.0f);

	DC->Dispatch(x, 1, 1);

	structuredBuffer->Copy(output, sizeof(OutputStruct) * size);

	float minDistance = FLT_MAX;
	int minIndex = -1;

	for (UINT i = 0; i < size; i++)
	{
		OutputStruct temp = output[i];
		if (temp.picked)
		{
			if (minDistance > temp.distance)
			{
				minDistance = temp.distance;
				minIndex = i;
			}
		}
	}

	if (minIndex >= 0)
	{

		*position = ray.position + ray.direction * minDistance;
		return true;
	}


	return false;
}

void Terrain::LoadTree()
{
	BinaryReader* reader = new BinaryReader(L"TextData/SaveObject.object");

	string objName = reader->String();
	UINT size = reader->UInt();
	if (size)
	{
		trees.resize(size);
		for (int i = 0; i < size; i++)
		{
			Tree* tmp = new Tree();
			void* data = &tmp->position;
			reader->Byte(&data, sizeof(Vector3));
			void* data2 = &tmp->rotation;
			reader->Byte(&data2, sizeof(Vector3));
			void* data3 = &tmp->scale;
			reader->Byte(&data3, sizeof(Vector3));
			trees[i] = tmp;
			trees[i]->UpdateWorld();
		}
	}

	delete reader;
}

void Terrain::LoadHeightMap(wstring path)
{
	ScratchImage image;

	HRESULT hr = LoadFromWICFile(path.c_str(), WIC_FLAGS_FORCE_RGB, nullptr, image);
	assert(SUCCEEDED(hr));


	ID3D11ShaderResourceView* srv;

	hr = CreateShaderResourceView(DEVICE, image.GetImages(), image.GetImageCount(),
		image.GetMetadata(), &srv);
	assert(SUCCEEDED(hr));


	UINT width = image.GetMetadata().width - 1;
	UINT height = image.GetMetadata().height - 1;

	vector<XMFLOAT4> pixels;

	uint8_t* colors = image.GetPixels();
	UINT size = image.GetPixelsSize();

	float scale = 1.0f / 255.0f;

	for (int i = 0; i < size; i += 4)
	{
		XMFLOAT4 color;

		color.x = colors[i + 0] * scale;
		color.y = colors[i + 1] * scale;
		color.z = colors[i + 2] * scale;
		color.w = colors[i + 3] * scale;

		pixels.emplace_back(color);
	}


	for (UINT i = 0; i < pixels.size(); i++)
	{
		vertices[i].position.y = 0;
		vertices[i].position.y += pixels[i].x * 20.0f;
		vertices[i].position.y += pixels[i].y * 20.0f;
		vertices[i].position.y += pixels[i].z * 20.0f;
	}

	CreateNormal();
	CreateTangent();
	CreateCompute();

	srv->Release();
	srv = nullptr;

	mesh->UpdateVertex(vertices.data(), vertices.size());
}

void Terrain::LoadAlphaMap(wstring path)
{
	ScratchImage image;

	HRESULT hr = LoadFromWICFile(path.c_str(), WIC_FLAGS_FORCE_RGB, nullptr, image);
	assert(SUCCEEDED(hr));


	ID3D11ShaderResourceView* srv;

	hr = CreateShaderResourceView(DEVICE, image.GetImages(), image.GetImageCount(),
		image.GetMetadata(), &srv);
	assert(SUCCEEDED(hr));


	UINT width = image.GetMetadata().width - 1;
	UINT height = image.GetMetadata().height - 1;

	vector<XMFLOAT4> pixels;

	uint8_t* colors = image.GetPixels();
	UINT size = image.GetPixelsSize();

	float scale = 1.0f / 255.0f;

	for (int i = 0; i < size; i += 4)
	{
		XMFLOAT4 color;

		color.x = colors[i + 0] * scale;
		color.y = colors[i + 1] * scale;
		color.z = colors[i + 2] * scale;
		color.w = colors[i + 3] * scale;

		pixels.emplace_back(color);
	}


	for (UINT i = 0; i < pixels.size(); i++)
	{
		vertices[i].alpha[0] = pixels[i].x;
		vertices[i].alpha[1] = pixels[i].y;
		vertices[i].alpha[2] = pixels[i].z;
		vertices[i].alpha[3] = pixels[i].w;
	}

	srv->Release();
	srv = nullptr;

	mesh->UpdateVertex(vertices.data(), vertices.size());
}

void Terrain::CreateData()
{
	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			VertexType vertex;
			vertex.position = XMFLOAT3(x, 0, z);
			vertex.uv = XMFLOAT2((x / (float)width) * 3, (1.0f - (z / (float)height)) * 3);


			vertices.emplace_back(vertex);
		}
	}

	for (UINT z = 0; z < height - 1; z++)
	{
		for (UINT x = 0; x < width - 1; x++)
		{
			indices.emplace_back(width * z + x);
			indices.emplace_back(width * (z + 1) + x);
			indices.emplace_back(width * (z + 1) + x + 1);
			indices.emplace_back(width * z + x);
			indices.emplace_back(width * (z + 1) + x + 1);
			indices.emplace_back(width * z + x + 1);
		}
	}
}

void Terrain::CreateNormal()
{
	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		Vector3 v0 = vertices[index0].position;
		Vector3 v1 = vertices[index1].position;
		Vector3 v2 = vertices[index2].position;

		Vector3 A = v1 - v0;
		Vector3 B = v2 - v0;

		Vector3 normal = Vector3::Cross(A, B).Normal();

		vertices[index0].normal = (vertices[index0].normal + normal).Normal();
		vertices[index1].normal = (vertices[index1].normal + normal).Normal();
		vertices[index2].normal = (vertices[index2].normal + normal).Normal();
	}
}

void Terrain::CreateTangent()
{
	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		VertexType vertex0 = vertices[index0];
		VertexType vertex1 = vertices[index1];
		VertexType vertex2 = vertices[index2];

		Vector3 p0 = vertex0.position;
		Vector3 p1 = vertex1.position;
		Vector3 p2 = vertex2.position;

		XMFLOAT2 uv0 = vertex0.uv;
		XMFLOAT2 uv1 = vertex1.uv;
		XMFLOAT2 uv2 = vertex2.uv;

		Vector3 e0 = p1 - p0;
		Vector3 e1 = p2 - p0;

		float u0 = uv1.x - uv0.x;
		float u1 = uv2.x - uv0.x;
		float v0 = uv1.y - uv0.y;
		float v1 = uv2.y - uv0.y;

		float d = 1.0f / (u0 * v1 - v0 * u1);

		Vector3 tangent;
		tangent = (v1 * e0 - v0 * e1) * d;

		vertices[index0].tangent = tangent + vertices[index0].tangent;
		vertices[index1].tangent = tangent + vertices[index1].tangent;
		vertices[index2].tangent = tangent + vertices[index2].tangent;

	}

	for (VertexType& vertex : vertices)
	{
		Vector3 t = vertex.tangent;
		Vector3 n = vertex.normal;

		Vector3 temp = (t - n * Vector3::Dot(n, t)).Normal();

		vertex.tangent = temp;
	}
}

void Terrain::CreateCompute()
{
	input = new InputStruct[indices.size() / 3];
	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		input[i].v0 = vertices[index0].position;
		input[i].v1 = vertices[index1].position;
		input[i].v2 = vertices[index2].position;

		input[i].index = i;
	}

	computeShader = Shader::AddCS(L"Intersection");

	// 폴리곤 개수
	size = indices.size() / 3;

	if (structuredBuffer)
	{
		delete structuredBuffer;
		structuredBuffer = nullptr;
	}

	structuredBuffer = new StructuredBuffer(input, sizeof(InputStruct), size,
		sizeof(OutputStruct), size);

	rayBuffer = new RayBuffer();
	output = new OutputStruct[size];
}
