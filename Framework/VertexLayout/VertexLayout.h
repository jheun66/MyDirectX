#pragma once

// VertexLayouts

struct Vertex
{
	XMFLOAT3 position;

	Vertex() : position(0, 0, 0)
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