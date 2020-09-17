#include "Framework.h"

RainExplosion::RainExplosion()
	:Particle(L"Textures/Particles/RainTexture.png", 32)
{
	material->SetShader(L"VertexSpark", L"PixelUV");
	geometryShader = Shader::AddGS(L"GeometrySpark");

	sparkBuffer = new SparkBuffer();

	Create();
}

RainExplosion::~RainExplosion()
{
	delete sparkBuffer;
	delete[] vertices;
}

void RainExplosion::Create()
{
	vertices = new VertexParticle[particleCount];

	for (UINT i = 0; i < particleCount; i++)
	{
		vertices[i].position = position;
		float size = GameMath::Random(0.1f, 0.5f);
		vertices[i].size = XMFLOAT2(size, size);

		float radius = GameMath::Random(5.0f, 10.0f);

		Vector3 velocity = Vector3(0, 0, radius);

		// 위쪽을 향하도록
		XMFLOAT3 rot;
		rot.x = GameMath::Random(-XM_PI, 0.0f);;
		rot.y = GameMath::Random(0.0f, XM_2PI);
		rot.z = GameMath::Random(0.0f, XM_2PI);

		XMMATRIX matRot = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);

		XMStoreFloat3(&vertices[i].velocity, XMVector3TransformCoord(velocity.data, matRot));
	}

	vertexBuffer = new VertexBuffer(vertices, sizeof(VertexParticle), particleCount);

	sparkBuffer->data.duration = 0.1f;
	sparkBuffer->data.gravity = 5.0f;
}

void RainExplosion::Update()
{
	if (!isPlay)
		return;

	sparkBuffer->data.time += Time::Delta();

	if (sparkBuffer->data.time > sparkBuffer->data.duration)
		Stop();

	UpdateWorld();
}

void RainExplosion::Render()
{
	if (!isPlay)
		return;

	sparkBuffer->SetVSBuffer(10);

	Particle::Render();
}

void RainExplosion::Play(Vector3 position)
{
	sparkBuffer->data.time = 0.0f;
	Particle::Play(position);
}
