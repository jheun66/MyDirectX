#include "Framework.h"

ModelMeshPart::ModelMeshPart()
{
	boneBuffer = new BoneBuffer();
}

ModelMeshPart::~ModelMeshPart()
{
	delete boneBuffer;
}

void ModelMeshPart::Update()
{
	boneBuffer->data.index = mesh->BoneIndex();
}

void ModelMeshPart::Render()
{
	boneBuffer->SetBufferToVS(3);

	material->Set();

	DC->DrawIndexed(indexCount, startIndex, 0);
}

void ModelMeshPart::SetTransforms(XMMATRIX* transforms)
{
	// ������ MAX�� �����������
	memcpy(boneBuffer->data.transforms, transforms,
		sizeof(XMMATRIX) * MAX_MODEL_BONE);
}
