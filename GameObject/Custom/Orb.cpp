#include "framework.h"
#include "Orb.h"

Orb::Orb(float radius, UINT sliceCount, UINT stackCount)
	:Sphere(radius, sliceCount, stackCount)
{
	// material µ¤¾î¾²±â
	delete material;
	material = new Material(L"Diffuse");
}

Orb::~Orb()
{
}

void Orb::Update()
{
	Rotation();
	Revolution();

	Sphere::Update();
}

void Orb::Render()
{
	Sphere::Render();
}

void Orb::Rotation()
{
	rotation.y += XMConvertToRadians(rotSpeed * Time::Delta());
}

void Orb::Revolution()
{
	if (pivot != nullptr)
	{
		degree += Time::Delta() * revSpeed;
		float theta = XMConvertToRadians(degree);
		position.x = pivot->x + distance * XMScalarCos(theta);
		position.y = pivot->y;
		position.z = pivot->z + distance * XMScalarSin(theta);
	}
}
