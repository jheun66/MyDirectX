#include "Framework.h"

TerrainEditor::TerrainEditor(UINT width, UINT height)
	:width(width), height(height), isRaise(true), adjustValue(50)
{
	material = new Material(L"BrushWithMapping");
	material->SetDiffuseMap(L"Terrain/brown_mud_leaves_01_diff_1k.png");
	material->SetSpecularMap(L"Terrain/brown_mud_leaves_01_spec_1k.png");
	material->SetNormalMap(L"Terrain/brown_mud_leaves_01_Nor_1k.png");

	CreateData();
	CreateNormal();
	CreateTangent();

	mesh = new Mesh(vertices.data(), sizeof(VertexType), vertices.size(),
		indices.data(), indices.size());

	CreateCompute();
	brushBuffer = new BrushBuffer();
}

TerrainEditor::~TerrainEditor()
{
	delete material;
	delete mesh;

	delete rayBuffer;
	delete structuredBuffer;

	delete[] input;
	delete[] output;

	delete brushBuffer;
}

void TerrainEditor::Update()
{
	Vector3 temp;
	ComputePicking(&temp);
	brushBuffer->data.location = temp;

	if (KEY_PRESS(VK_LCONTROL))
	{
		if (KEY_PRESS(VK_LBUTTON))
		{
			if (isRaise)
				AdjustY(temp, adjustValue);
			else
				AdjustY(temp, -adjustValue);
		}

		if (KEY_UP(VK_LBUTTON))
		{
			CreateNormal();
			CreateTangent();
			mesh->UpdateVertex(vertices.data(), vertices.size());
		}

	}

	UpdateWorld();
}

void TerrainEditor::Render()
{
	mesh->Set();

	SetWorldBuffer();
	brushBuffer->SetBufferToVS(3);

	material->Set();

	DC->DrawIndexed(indices.size(), 0, 0);
}

void TerrainEditor::PostRender()
{
	ImGui::Text("TerrainEditor");
	ImGui::SliderInt("Type", &brushBuffer->data.type, 1, 3);
	ImGui::Checkbox("Raise", &isRaise);
	ImGui::SliderFloat("AdjustValue", &adjustValue, 0, 300);
}

bool TerrainEditor::ComputePicking(OUT Vector3* position)
{
	Ray ray = Environment::Get()->MainCamera()->ScreenPointToRay(Mouse::Get()->GetPosition());
	
	rayBuffer->data.position = ray.position;
	rayBuffer->data.direction = ray.direction;
	rayBuffer->data.size = size;
	computeShader->Set();

	rayBuffer->SetBufferToCS(0);

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

void TerrainEditor::AdjustY(Vector3 position, float value)
{
	switch (brushBuffer->data.type)
	{
	case 1:
	{
		for (VertexType& vertex : vertices)
		{
			Vector3 p1 = Vector3(vertex.position.x, 0, vertex.position.z);
			Vector3 p2 = Vector3(position.x, 0, position.z);

			float dist = (p2 - p1).Length();

			float temp = value * max(0, cos(XM_PIDIV2 * dist / brushBuffer->data.range));

			if (dist <= brushBuffer->data.range)
			{
				vertex.position.y += temp * Time::Delta();
			}
		}
	}
	break;
	case 2:
	{
		for (VertexType& vertex : vertices)
		{
			if (position.x - brushBuffer->data.range <= vertex.position.x
				&& position.x + brushBuffer->data.range >= vertex.position.x
				&& position.z - brushBuffer->data.range <= vertex.position.z
				&& position.z + brushBuffer->data.range >= vertex.position.z)
			{
				vertex.position.y += value * Time::Delta();
			}
		}
	}

	break;
	default:
		break;

	}

	mesh->UpdateVertex(vertices.data(), vertices.size());
}

void TerrainEditor::CreateData()
{
	for (UINT z = 0; z <= height; z++)
	{
		for (UINT x = 0; x <= width; x++)
		{
			VertexType vertex;
			vertex.position = XMFLOAT3(x, 0, z);
			vertex.uv = XMFLOAT2((x / (float)width)*2 , (1.0f - (z / (float)height))*2);

			UINT index = width * z + x;

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

void TerrainEditor::CreateNormal()
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

void TerrainEditor::CreateTangent()
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

void TerrainEditor::CreateCompute()
{
	computeShader = Shader::AddCS(L"Intersection");

	// 폴리곤 개수
	size = indices.size() / 3;

	structuredBuffer = new StructuredBuffer(input, sizeof(InputStruct), size,
		sizeof(OutputStruct), size);

	rayBuffer = new RayBuffer();
	output = new OutputStruct[size];
}
