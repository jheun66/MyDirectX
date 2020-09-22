#include "Framework.h"

Snow::Snow()
	:Particle(L"Textures/Landscape/Snow.png", 1000)
{
	material->SetShader(L"Snow");
	geometryShader = Shader::AddGS(L"GeometrySnow");

	Create();

	blendState[1]->Alpha(true);
}

Snow::~Snow()
{
	delete snowBuffer;
}

void Snow::Create()
{
	vertices = new VertexSnow[MAX_COUNT];

	XMFLOAT3 scale = XMFLOAT3(50, 50, 50);

	for (UINT i = 0; i < MAX_COUNT; i++)
	{
		float size;
		size = GameMath::Random(0.1f, 0.4f);

		XMFLOAT3 position;
		position.x = GameMath::Random(-scale.x, scale.x);
		position.y = GameMath::Random(-scale.y, scale.y);
		position.z = GameMath::Random(-scale.z, scale.z);

		XMFLOAT2 random;
		random.x = GameMath::Random(0.0f, 1.0f);
		random.y = GameMath::Random(0.0f, 1.0f);

		vertices[i].position = position;
		vertices[i].scale = size;
		vertices[i].random = random;
	}

	vertexBuffer = new VertexBuffer(vertices, sizeof(VertexSnow), MAX_COUNT);

	snowBuffer = new SnowBuffer();
	snowBuffer->data.size = scale;
}

void Snow::Update()
{
	snowBuffer->data.time += Time::Delta();
}

void Snow::Render()
{
	snowBuffer->SetVSBuffer(10);
	Particle::Render();
}

void Snow::PostRender()
{
	ImGui::SliderFloat3("Velocity", (float*)&snowBuffer->data.velocity, -50, 50);
	ImGui::ColorEdit4("Color", (float*)&snowBuffer->data.color);
	ImGui::SliderFloat("Distance", &snowBuffer->data.distance, 0, 2000);
	ImGui::SliderFloat3("Origin", (float*)&snowBuffer->data.origin, -50, 50);
	ImGui::SliderFloat("Turbulence", &snowBuffer->data.turbulence, 0, 20.0f);
}
