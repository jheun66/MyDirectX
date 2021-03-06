#pragma once

class Mesh
{
public:
	Mesh(void* vertexData, UINT stride, UINT vertexCount,
		void* indexData, UINT indexCount);
	~Mesh();

	void Set(D3D11_PRIMITIVE_TOPOLOGY pt = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	void UpdateVertex(void* data, UINT count);
private:
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;
};