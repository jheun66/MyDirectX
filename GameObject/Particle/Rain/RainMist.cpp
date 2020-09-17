#include "Framework.h"

RainMist::RainMist()
	:Particle(L"Textures/Particles/RainTexture.png", 128)
{
	material->SetShader(L"VertexBillboard", L"PixelUV");
	//material->GetBuffer()->data.diffuse = {0.1f,0.1f,0.1f,1.0f};
	//구름은 위에서 바라봐도 제대로 보이도록
	geometryShader = Shader::AddGS(L"GeometrySpark");

	Create();
}

RainMist::~RainMist()
{
	delete[] vertices;
}

void RainMist::Create()
{
	vertices = new VertexSize[particleCount];

	for (UINT i = 0; i < particleCount; i++)
	{
		vertices[i].position.x = GameMath::Random(0.0f, 255.0f);
		vertices[i].position.y = GameMath::Random(150.0f, 250.0f);
		vertices[i].position.z = GameMath::Random(0.0f, 255.0f);

		float size = GameMath::Random(100.0f, 200.0f);
		vertices[i].size = XMFLOAT2(size, size);
	}

	vertexBuffer = new VertexBuffer(vertices, sizeof(VertexSize), particleCount);
}

void RainMist::Update()
{
	UpdateWorld();
}

void RainMist::Render()
{
	Particle::Render();
}
