#pragma once

class Tree : public ModelRender
{
private:
	Collider* collider;
	VertexBuffer* instanceBuffer = nullptr;
	UINT count;
public:
	Tree();
	~Tree();

	void Update() override;
	void Render() override;

	Collider* GetCollider() { return collider; }

	void SetInstanceBuffer(VertexBuffer* instanceBuffer, UINT count) { this->instanceBuffer = instanceBuffer; this->count = count; }
};