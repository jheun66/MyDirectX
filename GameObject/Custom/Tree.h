#pragma once

class Tree : public ModelRender
{
private:
	Collider* collider;

public:
	Tree();
	~Tree();

	void Update() override;
	void Render() override;

	Collider* GetCollider() { return collider; }
};