#include "framework.h"
#include "Sphere.h"
#include "Planet.h"

Planet::Planet(wstring texturePath, string tag)
	:tag(tag)
{
	vertexShader = new VertexShader(L"Shaders/VertexShaders/VertexDiffuse.hlsl");
	pixelShader = new PixelShader(L"Shaders/PixelShaders/PixelDiffuse.hlsl");

	if (texturePath != L"")
	{
		texture = Texture::Add(texturePath);
	}

	lightBuffer = new LightBuffer();

	if (texture != nullptr)
	{
		sector = 50;
		stack = 50;
	}


	CreateUVNormal();
}

Planet::~Planet()
{
	delete vertexShader;
	delete pixelShader;
	delete lightBuffer;
}

void Planet::Update()
{
	degree += Time::Delta() * revSpeed;
	float theta = XMConvertToRadians(degree);
	position.x = pivot->x + distance * XMScalarCos(theta);
	position.y = pivot->y;
	position.z = pivot->z + distance * XMScalarSin(theta);

	Sphere::Update();
}

void Planet::Render()
{
	vertexBuffer->Set();
	indexBuffer->Set();
	IASetPT();

	worldBuffer->SetBufferToVS(0);
	lightBuffer->SetBufferToVS(3);

	if (texture != nullptr)
		texture->PSSet(0);

	vertexShader->Set();
	pixelShader->Set();

	DC->DrawIndexed(indices.size(), 0, 0);
}

void Planet::PostRender()
{
	ImGui::Begin(("Planet " + tag).c_str(), 0, ImGuiWindowFlags_AlwaysAutoResize);
	{
		ImGui::SliderFloat3("position",(float*)&position, -100, 100);
		ImGui::SliderFloat3("LightPos", (float*)&lightBuffer->data.position, -100, 100);
		ImGui::SliderFloat("Rotation Speed", &rotSpeed, -2, 2);
	}
	ImGui::End();

}

void Planet::CreateUVNormal()
{
	// y 축(스택)은 반원만 하면 됨
	float stackStep = (float)XM_PI / stack;
	float sectorStep = (float)XM_2PI / sector;

	float radius = 1.0f;

	for (int i = 0; i <= stack; i++)
	{
		for (int j = 0; j <= sector; j++)
		{
			float pi = XM_PIDIV2 - stackStep * i;
			float theta = sectorStep * j;

			VertexUVNormal vt;

			vt.position.x = (radius * XMScalarCos(pi)) * XMScalarCos(theta);
			vt.position.y = (radius * XMScalarSin(pi));
			vt.position.z = (radius * XMScalarCos(pi)) * XMScalarSin(theta);

			vt.uv = XMFLOAT2(j / (float)sector, (i / (float)stack));
			UVVertices.push_back(vt);

		}
	}

	for (int i = 0; i < stack; i++)
	{
		int k1 = i * (sector + 1);
		int k2 = k1 + sector + 1;

		for (int j = 0; j < sector; j++, k1++, k2++)
		{
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k1 + 1);
				indices.push_back(k2);
			}

			if (i != stack - 1)
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2 + 1);
				indices.push_back(k2);
			}
		}
	}
	indexCnt = indices.size();
	indexBuffer = new IndexBuffer(indices.data(), indexCnt);

	// normal vector 구해주기
	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		XMVECTOR v0 = XMLoadFloat3(&UVVertices[index0].position);
		XMVECTOR v1 = XMLoadFloat3(&UVVertices[index1].position);
		XMVECTOR v2 = XMLoadFloat3(&UVVertices[index2].position);

		XMVECTOR A = v1 - v0;
		XMVECTOR B = v2 - v0;

		XMVECTOR normal = XMVector3Normalize(XMVector3Cross(A, B));

		XMStoreFloat3(&UVVertices[index0].normal, normal);
		XMStoreFloat3(&UVVertices[index1].normal, normal);
		XMStoreFloat3(&UVVertices[index2].normal, normal);
	}

	vertexBuffer = new VertexBuffer(UVVertices.data(), sizeof(VertexUVNormal), UVVertices.size());
	
}

void Planet::SetPivot(XMFLOAT3 *value)
{
	pivot = value;
	XMVECTOR pivotVec = XMLoadFloat3(pivot);
	XMVECTOR posVec = XMLoadFloat3(&position);

	distance = XMVectorGetX(XMVector3Length(pivotVec - posVec));
}
