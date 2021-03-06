#include "Framework.h"
#include "BillboardScene.h"

BillboardScene::BillboardScene()
	:count(1000)
{
	terrain = new Terrain(256, 256);

	//for (UINT i = 0; i < 1000; i++)
	//{
	//	Billboard* tree = new Billboard(L"Textures/Landscape/Tree.png");

	//	Vector3 pos;
	//	pos.x = GameMath::Random(0.0f, 255.0f);
	//	pos.z = GameMath::Random(0.0f, 255.0f);

	//	Vector3 scale;
	//	scale.x = GameMath::Random(3.0f, 10.0f);
	//	scale.y = GameMath::Random(3.0f, 10.0f);
	//	scale.z = 1.0f;

	//	pos.y = (scale.y * 0.5f + terrain->GetAltitude(pos));

	//	tree->position = pos;
	//	tree->scale = scale;

	//	trees.emplace_back(tree);
	//}

	VertexSize* vertices = new VertexSize[count];
	for (UINT i = 0; i < count; i++)
	{

		Vector3 pos;
		pos.x = GameMath::Random(0.0f, 255.0f);
		pos.z = GameMath::Random(0.0f, 255.0f);

		Vector3 scale;
		scale.x = GameMath::Random(3.0f, 10.0f);
		scale.y = GameMath::Random(3.0f, 10.0f);
		scale.z = 1.0f;

		pos.y = (scale.y * 0.5f + terrain->GetAltitude(pos));

		vertices[i].position = pos;
		vertices[i].size = { scale.x, scale.y };
	}

	vertexBuffer = new VertexBuffer(vertices, sizeof(VertexSize), count);

	delete[] vertices;

	material = new Material(L"VertexBillboard", L"PixelUV");
	material->SetDiffuseMap(L"Textures/Landscape/Tree.png");

	geometryShader = Shader::AddGS(L"GeometryBillboard");

	blendState[0] = new BlendState();
	blendState[1] = new BlendState();
	//blendState[1]->Alpha(true);
	blendState[1]->AlphaToCoverage(true);
	//blendState[1]->Additvie();

	snow = new Snow();

	ParticleManager::Create();
}

BillboardScene::~BillboardScene()
{
	delete terrain;

	delete vertexBuffer;
	delete material;

	delete snow;

	ParticleManager::Delete();
}

void BillboardScene::Update()
{
	if (KEY_DOWN(VK_LBUTTON) && !ImGui::GetIO().WantCaptureMouse)
	{
		Vector3 pickPos;
		terrain->ComputePicking(&pickPos);

		ParticleManager::Get()->Play("spark", pickPos);
	}
	if (KEY_DOWN(VK_RBUTTON) && !ImGui::GetIO().WantCaptureMouse)
	{
		Vector3 pickPos;
		terrain->ComputePicking(&pickPos);

		ParticleManager::Get()->Play("breath", pickPos, Vector3(XM_PI, 0, 0));
	}

	terrain->Update();

	ParticleManager::Get()->Update();
	snow->Update();
	//for (Billboard* tree : trees)
	//	tree->Update();
}

void BillboardScene::PreRender()
{
}

void BillboardScene::Render()
{
	terrain->Render();

	//for (Billboard* tree : trees)
	//	tree->Render();

	blendState[1]->SetState();

	vertexBuffer->Set();
	DC->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	Camera::Get()->GetViewBuffer()->SetGSBuffer(1);
	Camera::Get()->GetProjection()->SetGSBuffer(2);

	material->Set();
	geometryShader->Set();

	DC->Draw(count, 0);

	DC->GSSetShader(nullptr, nullptr, 0);

	blendState[0]->SetState();

	ParticleManager::Get()->Render();

	snow->Render();
}

void BillboardScene::PostRender()
{
	snow->PostRender();
}
