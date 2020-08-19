#include "framework.h"
#include "Sphere.h"
#include "Planet.h"

Planet::Planet(wstring texturePath, string tag)
	:tag(tag)
{
	vertexShader = Shader::AddVS(L"VertexSpecular");
	pixelShader = Shader::AddPS(L"PixelSpecular");

	if (texturePath != L"")
	{
		texture = Texture::Add(texturePath);
	}


	if (texture != nullptr)
	{
		sector = 50;
		stack = 50;
	}


	CreateUVNormal();
}

Planet::~Planet()
{
}

void Planet::Update()
{
	Rotation();
	Revolution();

	UpdateWorld();
}

void Planet::Render()
{
	vertexBuffer->Set();
	indexBuffer->Set();
	IASetPT();

	worldBuffer->SetBufferToVS(0);

	if (texture != nullptr)
		texture->PSSet(0);

	vertexShader->Set();
	pixelShader->Set();

	DC->DrawIndexed(indices.size(), 0, 0);
}

void Planet::Rotation()
{
	__super::Rotation();
}

void Planet::Revolution()
{
	if (pivot != nullptr)
	{
		degree += Time::Delta() * revSpeed;
		float theta = XMConvertToRadians(degree);
		position.x = pivot->x + distance * XMScalarCos(theta);
		position.y = pivot->y;
		position.z = pivot->z + distance * XMScalarSin(theta);
	}
}

void Planet::PostRender()
{
	ImGui::Begin(("Planet " + tag).c_str(), 0, ImGuiWindowFlags_AlwaysAutoResize);
	{
		ImGui::SliderFloat3("position",(float*)&position, -100, 100);
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

			vt.normal.x = (radius * XMScalarCos(pi)) * XMScalarCos(theta);
			vt.normal.y = (radius * XMScalarSin(pi));
			vt.normal.z = (radius * XMScalarCos(pi)) * XMScalarSin(theta);

			vt.position = Vector3(vt.normal) * radius;

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

	vertexBuffer = new VertexBuffer(UVVertices.data(), sizeof(VertexUVNormal), UVVertices.size());
	
}

void Planet::SetPivot(Vector3* value)
{
	pivot = value;

	Vector3 tmp = *pivot - position;
	distance = tmp.Length();
}
