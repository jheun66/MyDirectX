#include "Framework.h"

RainFall::RainFall()
	:Particle(L"Textures/Particles/RainTexture.png", 1024)
{
	material->SetShader(L"VertexBillboard", L"PixelUV");

	geometryShader = Shader::AddGS(L"GeometryBillboard");

}

RainFall::~RainFall()
{
	delete[] vertices;
	delete[] endPos;
}

void RainFall::Create()
{
	vertices = new VertexSize[particleCount];
	endPos = new XMFLOAT3[particleCount];

	direction = { 0,-20,0 };

	for (UINT i = 0; i < particleCount; i++)
	{
		vertices[i].position.x = GameMath::Random(0.0f, 255.0f);
		vertices[i].position.y = GameMath::Random(150.0f, 250.0f);
		vertices[i].position.z = GameMath::Random(0.0f, 255.0f);

		Ray ray;
		ray.position = vertices[i].position;
		ray.direction = direction;

		Vector3 pos;
		if (terrain->ComputePicking(&pos, ray))
		{
			endPos[i] = pos;
		}
		else
		{
			endPos[i] = { 0,0,0 };
		}

		float size = GameMath::Random(0.1f, 0.5f);
		vertices[i].size = XMFLOAT2(size, size*10);
	}

	vertexBuffer = new VertexBuffer(vertices, sizeof(VertexSize), particleCount);
}

void RainFall::Update()
{
	for (UINT i = 0; i < particleCount; i++)
	{
		vertices[i].position.x += direction.x * Time::Delta();
		vertices[i].position.y += direction.y * Time::Delta();
		vertices[i].position.z += direction.z * Time::Delta();

		if (vertices[i].position.y < endPos[i].y)
		{
			// 함수포인터
			Explosion(endPos[i]);

			vertices[i].position.x = GameMath::Random(0.0f, 255.0f);
			vertices[i].position.y = GameMath::Random(150.0f, 250.0f);
			vertices[i].position.z = GameMath::Random(0.0f, 255.0f);

			Ray ray;
			ray.position = vertices[i].position;
			ray.direction = direction;

			Vector3 pos;
			if (terrain->ComputePicking(&pos, ray))
			{
				endPos[i] = pos;
			}
			else
			{
				endPos[i] = { 0,0,0 };
			}
		}
	}
	vertexBuffer->Update(vertices, particleCount);
	UpdateWorld();
}

void RainFall::Render()
{
	Particle::Render();
}

void RainFall::PostRender()
{
	ImGui::Begin("RainFall", 0, ImGuiWindowFlags_AlwaysAutoResize);
	{
		ImGui::SliderFloat3("Direction", (float*)&direction, -100, 100, "%.2f");
		ImGui::Text("RainFall particles counts : %d", particleCount);
	}
	ImGui::End();
}

void RainFall::SetTerrain(Terrain* terrain)
{
	this->terrain = terrain;
	this->terrain->Update();
	Create();
}
