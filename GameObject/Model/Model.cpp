#include "Framework.h"

Model::Model()
{
}

Model::Model(string file)
{
	ReadMaterial(file);
	ReadMesh(file);
}

Model::~Model()
{
}

void Model::Update()
{
	UpdateWorld();
}

void Model::Render()
{
	worldBuffer->SetBufferToVS(0);

	for (ModelMesh* mesh : meshes)
		mesh->Render();
}

void Model::ReadMaterial(string file)
{
	file = "ModelData/Materials/" + file + ".mat";

	wstring folder = Utility::ToWString(Utility::GetDirectoryName(file));

	Xml::XMLDocument* document = new Xml::XMLDocument();
	document->LoadFile(file.c_str());

	Xml::XMLElement* root = document->FirstChildElement();

	Xml::XMLElement* matNode = root->FirstChildElement();

	do
	{
		Xml::XMLElement* node = matNode->FirstChildElement();

		Material* material = new Material();
		material->name = node->GetText();

		node = node->NextSiblingElement();

		if (node->GetText())
		{
			wstring file = Utility::ToWString(node->GetText());
			material->SetDiffuseMap(folder + file);
		}

		node = node->NextSiblingElement();

		if (node->GetText())
		{
			wstring file = Utility::ToWString(node->GetText());
			material->SetSpecularMap(folder + file);
		}

		node = node->NextSiblingElement();

		if (node->GetText())
		{
			wstring file = Utility::ToWString(node->GetText());
			material->SetNormalMap(folder + file);
		}

		XMFLOAT4 color;

		node = node->NextSiblingElement();
		color.x = node->FloatAttribute("R");
		color.y = node->FloatAttribute("G");
		color.z = node->FloatAttribute("B");
		color.w = node->FloatAttribute("A");
		material->GetBuffer()->data.ambient = color;

		node = node->NextSiblingElement();
		color.x = node->FloatAttribute("R");
		color.y = node->FloatAttribute("G");
		color.z = node->FloatAttribute("B");
		color.w = node->FloatAttribute("A");
		material->GetBuffer()->data.diffuse = color;

		node = node->NextSiblingElement();
		color.x = node->FloatAttribute("R");
		color.y = node->FloatAttribute("G");
		color.z = node->FloatAttribute("B");
		color.w = node->FloatAttribute("A");
		material->GetBuffer()->data.specular = color;

		materials[material->name] = material;

		matNode = matNode->NextSiblingElement();
	} while (matNode != nullptr);

	delete document;
}

void Model::ReadMesh(string file)
{
	file = "ModelData/Meshes/" + file + ".mesh";
	BinaryReader* r = new BinaryReader(Utility::ToWString(file));

	UINT count = r->UInt();

	for (UINT i = 0; i < count; i++)
	{
		ModelMesh* mesh = new ModelMesh();
		mesh->name = r->String();
		mesh->materialName = r->String();

		mesh->material = materials[mesh->materialName];
		{//Vertices
			UINT count = r->UInt();

			mesh->vertexCount = count;
			mesh->vertices = new ModelVertex[count];

			void* ptr = (void*)mesh->vertices;
			r->Byte(&ptr, sizeof(ModelVertex) * count);
		}

		{//Indices
			UINT count = r->UInt();

			mesh->indexCount = count;
			mesh->indices = new UINT[count];

			void* ptr = (void*)mesh->indices;
			r->Byte(&ptr, sizeof(UINT) * count);
		}

		mesh->CreateMesh();

		meshes.emplace_back(mesh);
	}

	delete r;
}

void Model::SetShader(wstring file)
{
	for (auto material : materials)
		material.second->SetShader(file);
}