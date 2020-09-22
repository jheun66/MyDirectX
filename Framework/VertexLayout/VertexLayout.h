#pragma once

// VertexLayouts

struct Vertex
{
	XMFLOAT3 position;

	Vertex() : position(0, 0, 0)
	{

	}
	Vertex(float x, float y, float z) : position(x, y, z)
	{

	}
};

struct VertexColor
{
	XMFLOAT3 position;
	XMFLOAT4 color;

	VertexColor() :position(0, 0, 0), color(1, 1, 1, 1)
	{

	}
};


struct VertexUV
{
	XMFLOAT3 position;
	XMFLOAT2 uv;

	VertexUV() :position(0, 0, 0), uv(0, 0)
	{

	}
};

struct VertexSize
{
	XMFLOAT3 position;
	XMFLOAT2 size;

	VertexSize() :position(0, 0, 0), size(0, 0)
	{

	}
};

struct VertexUVNormal
{
	XMFLOAT3 position;
	XMFLOAT2 uv;
	XMFLOAT3 normal;

	VertexUVNormal() : position(0, 0, 0), uv(0, 0), normal(0, 0, 0)
	{

	}
};

struct VertexUVNormalTangent
{
	XMFLOAT3 position;
	XMFLOAT2 uv;
	XMFLOAT3 normal;
	XMFLOAT3 tangent;

	VertexUVNormalTangent() : position(0, 0, 0), uv(0, 0), normal(0, 0, 0), tangent(0, 0, 0)
	{

	}
};

struct VertexUVNormalTangentBlend
{
	XMFLOAT3 position;
	XMFLOAT2 uv;
	XMFLOAT3 normal;
	XMFLOAT3 tangent;
	XMFLOAT4 blendIndices;
	XMFLOAT4 blendWeights;

	VertexUVNormalTangentBlend() : position(0, 0, 0), uv(0, 0), normal(0, 0, 0), tangent(0, 0, 0),
		blendIndices(0, 0, 0, 0), blendWeights(0, 0, 0, 0)
	{

	}
};

struct VertexUVNormalAlpha
{
	XMFLOAT3 position;
	XMFLOAT2 uv;
	XMFLOAT3 normal;
	float alpha[4];

	VertexUVNormalAlpha() : position(0, 0, 0), uv(0, 0), normal(0, 0, 0), alpha{}
	{

	}
};

struct VertexUVNormalTangentAlpha
{
	XMFLOAT3 position;
	XMFLOAT2 uv;
	XMFLOAT3 normal;
	XMFLOAT3 tangent;
	float alpha[4];

	VertexUVNormalTangentAlpha() : position(0, 0, 0), uv(0, 0), normal(0, 0, 0), tangent(0, 0, 0), alpha{}
	{

	}
};

struct VertexParticle
{
	XMFLOAT3 position;
	XMFLOAT2 size;
	XMFLOAT3 velocity;

	UINT isActive;
	float lifeTime;

	VertexParticle() : position(0, 0, 0), size(0, 0), velocity(0, 0, 0), isActive(0), lifeTime(5.0f)
	{

	}
};