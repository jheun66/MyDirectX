#pragma once

struct Ray
{
	Vector3 position;
	Vector3 direction;
};

struct Contact
{
	Vector3 hitPoint;
	float distance;
};

class BoxCollider;
class SphereCollider;

class Collider : public Transform
{
public:
	Collider();
	virtual ~Collider();

	bool IsCollision(Collider* collider);
	virtual bool IsCollision(IN Ray ray, OUT Contact* contact = nullptr) = 0;
	virtual bool IsBoxCollision(BoxCollider* collider) = 0;
	virtual bool IsSphereCollision(SphereCollider* collider) = 0;

	virtual void CreateMesh() = 0;

	void Render();

	void SetColor(XMFLOAT4 color) { material->GetBuffer()->data.diffuse = color; }

protected:

	Material* material;
	Mesh* mesh;

	vector<Vertex> vertices;
	vector<UINT> indices;
};