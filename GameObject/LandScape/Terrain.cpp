#include "Framework.h"
#include "Terrain.h"

Terrain::Terrain()
{
	material = new Material(L"NormalMapping");
	material->SetDiffuseMap(L"Terrain/brown_mud_leaves_01_diff_1k.png");
	material->SetSpecularMap(L"Terrain/brown_mud_leaves_01_spec_1k.png");
	material->SetNormalMap(L"Terrain/brown_mud_leaves_01_Nor_1k.png");

	heightMap = Texture::Add(L"HeightMaps/HeightMap.png");

	CreateData();
	CreateNormal();
	CreateTangent();

	mesh = new Mesh(vertices.data(), sizeof(VertexType), vertices.size(),
		indices.data(), indices.size());

	computeShader = Shader::AddCS(L"Intersection");

	// ������ ����
	size = indices.size() / 3;

	structuredBuffer = new StructuredBuffer(input, sizeof(InputStruct), size,
		sizeof(OutputStruct), size);

	rayBuffer = new RayBuffer();
	output = new OutputStruct[size];
}

Terrain::~Terrain()
{
	delete material;
	delete mesh;

	delete rayBuffer;
	delete structuredBuffer;

	delete[] input;
	delete[] output;
}

void Terrain::Update()
{
	UpdateWorld();
}

void Terrain::Render()
{
	mesh->Set();

	SetWorldBuffer();

	material->Set();

	DC->DrawIndexed(indices.size(), 0, 0);
}

void Terrain::PostRender()
{

}

bool Terrain::Picking(OUT Vector3* position)
{
	Ray ray = Environment::Get()->MainCamera()->ScreenPointToRay(Mouse::Get()->GetPosition());

	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			UINT index[4];
			index[0] = (width + 1) * z + x;
			index[1] = (width + 1) * z + x + 1;
			index[2] = (width + 1) * (z + 1) + x;
			index[3] = (width + 1) * (z + 1) + x + 1;

			Vector3 p[4];
			for (UINT i = 0; i < 4; i++)
				p[i] = vertices[index[i]].position;

			float distance;
			if (TriangleTests::Intersects(ray.position, ray.direction, p[0], p[1], p[2], distance))
			{
				*position = ray.position + ray.direction * distance;
				return true;
			}

			if (TriangleTests::Intersects(ray.position, ray.direction, p[3], p[1], p[2], distance))
			{
				*position = ray.position + ray.direction * distance;
				return true;
			}
		}
	}

	return false;
}

float Terrain::GetAltitude(Vector3 position)
{
	UINT x = (UINT)position.x;
	UINT z = (UINT)position.z;

	if (x < 0 || x > width) return 0.0f;
	if (z < 0 || z > height) return 0.0f;

	UINT index[4];
	index[0] = (width + 1) * z + x;
	index[1] = (width + 1) * (z + 1) + x;
	index[2] = (width + 1) * z + x + 1;
	index[3] = (width + 1) * (z + 1) + x + 1;

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
	Ray ray = Environment::Get()->MainCamera()->ScreenPointToRay(Mouse::Get()->GetPosition());

	rayBuffer->data.position = ray.position;
	rayBuffer->data.direction = ray.direction;
	rayBuffer->data.size = size;
	computeShader->Set();

	rayBuffer->SetBufferToCS(0);

	DC->CSSetShaderResources(0, 1, &structuredBuffer->GetSRV());
	DC->CSSetUnorderedAccessViews(0, 1, &structuredBuffer->GetUAV(), nullptr);

	// ceil �ø�				1024 ������ ����
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

void Terrain::CreateData()
{
	width = heightMap->GetWidth() - 1;
	height = heightMap->GetHeight() - 1;

	vector<XMFLOAT4> pixels = heightMap->ReadPixels();

	for (UINT z = 0; z <= height; z++)
	{
		for (UINT x = 0; x <= width; x++)
		{
			VertexType vertex;
			vertex.position = XMFLOAT3(x, 0, z);
			vertex.uv = XMFLOAT2(x / (float)width, 1.0f - (z / (float)height));

			UINT index = (width + 1) * z + x;

			vertex.position.y += pixels[index].x * 20.0f;
			vertex.position.y += pixels[index].y * 20.0f;
			vertex.position.y += pixels[index].z * 20.0f;

			vertices.emplace_back(vertex);
		}
	}

	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			indices.emplace_back((width + 1) * z + x);
			indices.emplace_back((width + 1) * (z + 1) + x);
			indices.emplace_back((width + 1) * (z + 1) + x + 1);
			indices.emplace_back((width + 1) * z + x);
			indices.emplace_back((width + 1) * (z + 1) + x + 1);
			indices.emplace_back((width + 1) * z + x + 1);
		}
	}

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