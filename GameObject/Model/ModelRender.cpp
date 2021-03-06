#include "framework.h"

ModelRender::ModelRender(string file)
{
	ReadMaterial(file);
	ReadMesh(file);

	SetShader(L"VertexModel", L"PixelNormalMapping");

	for (UINT i = 0; i < MAX_MODEL_INSTANCE; i++)
		worlds[i] = XMMatrixIdentity();

	instanceBuffer = new VertexBuffer(worlds, sizeof(XMMATRIX), MAX_MODEL_INSTANCE);
}

ModelRender::~ModelRender()
{
	for (Transform* transform : transforms)
		delete transform;

	delete instanceBuffer;
}

void ModelRender::Update()
{
	UpdateTransforms();

	Model::Update();
}

void ModelRender::Render()
{
	instanceBuffer->Set(1);

	for (ModelMesh* mesh : meshes)
		mesh->Render(transforms.size());
}

void ModelRender::UpdateTransforms()
{
	for (UINT i = 0; i < transforms.size(); i++)
	{
		transforms[i]->UpdateWorld();
		XMMATRIX temp = XMMatrixTranspose(*transforms[i]->GetWorld());
		memcpy(&worlds[i], &temp, sizeof(XMMATRIX));
	}

	instanceBuffer->Update(worlds, MAX_MODEL_INSTANCE);
}

Transform* ModelRender::AddTransform()
{
	Transform* transform = new Transform();
	transforms.emplace_back(transform);

	return transform;
}