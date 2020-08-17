#include "Framework.h"

Transform::Transform(string tag)
	: tag(tag), position(0, 0, 0), rotation(0, 0, 0), scale(1, 1, 1),
	pivot(0, 0, 0), parent(nullptr)
{
	world = XMMatrixIdentity();
	worldBuffer = new MatrixBuffer();
}

Transform::~Transform()
{
	delete worldBuffer;
}

void Transform::UpdateWorld()
{
	XMVECTOR v_pivot = XMLoadFloat3(&pivot);
	XMVECTOR v_position = XMLoadFloat3(&position);
	XMVECTOR v_sclae = XMLoadFloat3(&scale);

	world = XMMatrixTransformation(v_pivot, XMQuaternionIdentity(),
		v_sclae, v_pivot, XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z)
		, v_position);

	if (parent != nullptr)
		world *= *parent;

	worldBuffer->Set(world);
}

void Transform::SetWorldBuffer(UINT slot)
{
	worldBuffer->SetBufferToVS(slot);
}
