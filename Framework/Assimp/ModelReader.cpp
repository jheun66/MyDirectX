#include "Framework.h"

ModelReader::ModelReader()
{
	// importer를 이용하여 추출한다. 이를 위한 instance 생성
	importer = new Assimp::Importer();
}

ModelReader::~ModelReader()
{
	delete importer;
}

void ModelReader::ReadFile(string file)
{
	// file 이름 사용 안함
	//this->file = file;

	// 정보를 담고 있는 포인터를 scene에 넣어줌
	scene = importer->ReadFile(file,
		aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_MaxQuality);
	assert(scene != nullptr);
}

// Material을 읽고 해당 경로에 저장해줌 
void ModelReader::ExportMaterial(string savePath)
{
	ReadMaterial();
	savePath = "ModelData/Materials/" + savePath + ".mat";
	WriteMaterial(savePath);
}

void ModelReader::ReadMaterial()
{
	// 필요한 머테리얼 데이터들을 읽어온다.
	for (UINT i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* srcMaterial = scene->mMaterials[i];
		MaterialData* material = new MaterialData();

		material->name = srcMaterial->GetName().C_Str();

		aiColor3D color;

		srcMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
		material->ambient = XMFLOAT4(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		material->diffuse = XMFLOAT4(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
		material->specular = XMFLOAT4(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_SHININESS, material->specular.w);

		aiString file;
		srcMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file);
		material->diffuseFile = file.C_Str();

		file.Clear();
		srcMaterial->GetTexture(aiTextureType_SPECULAR, 0, &file);
		material->specularFile = file.C_Str();

		file.Clear();
		srcMaterial->GetTexture(aiTextureType_NORMALS, 0, &file);
		material->normalFile = file.C_Str();

		materials.emplace_back(material);
	}
}

// 읽어온 Material을 xml 형식으로 출력
void ModelReader::WriteMaterial(string savePath)
{
	Utility::CreateFolders(savePath);

	Xml::XMLDocument* document = new Xml::XMLDocument();

	Xml::XMLElement* root = document->NewElement("Materials");
	document->LinkEndChild(root);

	string folder = Utility::GetDirectoryName(savePath);

	for (MaterialData* material : materials)
	{
		Xml::XMLElement* node = document->NewElement("Material");
		root->LinkEndChild(node);

		Xml::XMLElement* element = document->NewElement("Name");
		element->SetText(material->name.c_str());
		node->LinkEndChild(element);

		element = document->NewElement("DiffuseFile");
		element->SetText(WriteTexture(folder, material->diffuseFile).c_str());
		node->LinkEndChild(element);

		element = document->NewElement("SpecularFile");
		element->SetText(WriteTexture(folder, material->specularFile).c_str());
		node->LinkEndChild(element);

		element = document->NewElement("NormalFile");
		element->SetText(WriteTexture(folder, material->normalFile).c_str());
		node->LinkEndChild(element);

		element = document->NewElement("Ambient");
		element->SetAttribute("R", material->ambient.x);
		element->SetAttribute("G", material->ambient.y);
		element->SetAttribute("B", material->ambient.z);
		element->SetAttribute("A", material->ambient.w);
		node->LinkEndChild(element);

		element = document->NewElement("Diffuse");
		element->SetAttribute("R", material->diffuse.x);
		element->SetAttribute("G", material->diffuse.y);
		element->SetAttribute("B", material->diffuse.z);
		element->SetAttribute("A", material->diffuse.w);
		node->LinkEndChild(element);

		element = document->NewElement("Specular");
		element->SetAttribute("R", material->specular.x);
		element->SetAttribute("G", material->specular.y);
		element->SetAttribute("B", material->specular.z);
		element->SetAttribute("A", material->specular.w);
		node->LinkEndChild(element);

		delete material;
	}
	materials.clear();

	document->SaveFile(savePath.c_str());

	delete document;
}

// 머테리얼에서 텍스처 파일 쓰기
string ModelReader::WriteTexture(string savePath, string file)
{
	if (file.length() == 0)
		return "";

	// png 파일에 대해서만 생각.. (다른 형식의 그림 파일이 들어 있으면 문제 있을 수도)
	string fileName = Utility::GetFileNameWithoutExtension(file) + ".png";
	const aiTexture* texture = scene->GetEmbeddedTexture(file.c_str());

	string path = "";

	if (texture)
	{
		path = savePath + fileName;

		// 높이가 없으면 binary로 쓰기
		if (texture->mHeight < 1)
		{
			BinaryWriter w(Utility::ToWString(path));
			w.Byte(texture->pcData, texture->mWidth);
		}
		else
		{
			Image image;

			image.width = texture->mWidth;
			image.height = texture->mHeight;
			image.pixels = reinterpret_cast<uint8_t*>(texture->pcData);
			image.format = DXGI_FORMAT_R8G8B8A8_UNORM;
			image.rowPitch = image.width * 4;

			image.slicePitch = image.width * image.height * 4;

			SaveToWICFile(image, WIC_FLAGS_NONE, GetWICCodec(WIC_CODEC_PNG),
				Utility::ToWString(path).c_str());
		}
	}

	return fileName;
}

// Mesh 데이터 추출하여 저장까지
void ModelReader::ExportMesh(string savePath)
{
	// Bone 마다 달려있는 Mesh를 읽어준다.
	ReadBone(scene->mRootNode, -1, -1);
	// bone weight를 이용해서 스킨 만들기(애니메이션에서 벌어지는 부분을 위해)
	ReadSkin();
	savePath = "ModelData/Meshes/" + savePath + ".mesh";
	WriteMesh(savePath);
}

void ModelReader::ReadBone(aiNode* node, int index, int parent)
{
	BoneData* bone = new BoneData();
	bone->index = index;
	bone->parent = parent;
	bone->name = node->mName.C_Str();

	// 주의) 전치 행렬로 넣어줌
	XMMATRIX transform(node->mTransformation[0]);
	bone->transform = XMMatrixTranspose(transform);

	XMMATRIX matParent;
	// 부모 0보다 작으면 == 루트
	if (parent < 0)
		matParent = XMMatrixIdentity();
	else
		matParent = bones[parent]->transform;

	// 부모 bone의 transform을 곱해줌
	bone->transform = bone->transform * matParent;
	bones.emplace_back(bone);

	// 해당 bone의 mesh를 읽어 meshes에 넣어줌
	ReadMesh(node, index);

	// 자식들에 대해 더이상 자식이 없을 때까지 재귀
	for (UINT i = 0; i < node->mNumChildren; i++)
		ReadBone(node->mChildren[i], bones.size(), index);

}

void ModelReader::ReadMesh(aiNode* node, int bone)
{
	if (node->mNumMeshes < 1)
		return;

	MeshData* mesh = new MeshData();
	mesh->name = node->mName.C_Str();
	mesh->boneIndex = bone;

	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		UINT index = node->mMeshes[i];
		aiMesh* srcMesh = scene->mMeshes[index];

		UINT startVertex = mesh->vertices.size();
		UINT startIndex = mesh->indices.size();

		for (UINT v = 0; v < srcMesh->mNumVertices; v++)
		{
			ModelVertex vertex;
			memcpy(&vertex.position, &srcMesh->mVertices[v], sizeof(XMFLOAT3));

			if (srcMesh->HasTextureCoords(0))
				memcpy(&vertex.uv, &srcMesh->mTextureCoords[0][v], sizeof(XMFLOAT2));

			if (srcMesh->HasNormals())
				memcpy(&vertex.normal, &srcMesh->mNormals[v], sizeof(XMFLOAT3));

			if (srcMesh->HasTangentsAndBitangents())
				memcpy(&vertex.tangent, &srcMesh->mTangents[v], sizeof(XMFLOAT3));

			mesh->vertices.emplace_back(vertex);
		}//Vertices

		for (UINT f = 0; f < srcMesh->mNumFaces; f++)
		{
			aiFace& face = srcMesh->mFaces[f];

			for (UINT k = 0; k < face.mNumIndices; k++)
			{
				mesh->indices.emplace_back(face.mIndices[k]);
				mesh->indices.back() += startVertex;
			}
		}//Indices

		aiMaterial* material = scene->mMaterials[srcMesh->mMaterialIndex];

		MeshPartData* meshPart = new MeshPartData();
		meshPart->name = srcMesh->mName.C_Str();
		meshPart->materialName = material->GetName().C_Str();
		meshPart->startVertex = startVertex;
		meshPart->startIndex = startIndex;
		meshPart->vertexCount = srcMesh->mNumVertices;
		meshPart->indexCount = srcMesh->mNumFaces * srcMesh->mFaces->mNumIndices;

		mesh->meshParts.emplace_back(meshPart);


	}//Mesh

	meshes.emplace_back(mesh);
}

void ModelReader::ReadSkin()
{
	for (UINT i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* srcMesh = scene->mMeshes[i];
		if (!srcMesh->HasBones())
			continue;

		MeshData* mesh = meshes[i];
		vector<BoneWeights> boneWeights;
		boneWeights.resize(mesh->vertices.size());

		for (UINT b = 0; b < srcMesh->mNumBones; b++)
		{
			aiBone* srcBone = srcMesh->mBones[b];

			UINT boneIndex = 0;
			for (BoneData* bone : bones)
			{
				if (bone->name == srcBone->mName.C_Str())
				{
					boneIndex = bone->index;
					break;
				}
			}// BoneData

			for (UINT w = 0; w < srcBone->mNumWeights; w++)
			{
				UINT index = srcBone->mWeights[w].mVertexId;
				float weight = srcBone->mWeights[w].mWeight;

				boneWeights[index].AddWeights(boneIndex, weight);
			}// Weight
		}// SrcBone

		for (UINT w = 0; w < boneWeights.size(); w++)
		{
			boneWeights[w].Normalize();

			BlendWeight blendWeight;
			boneWeights[w].GetBlendWeights(blendWeight);

			mesh->vertices[w].blendIndices = blendWeight.indices;
			mesh->vertices[w].blendWeights = blendWeight.weights;
		}// BoneWeights
	}// Mesh
}

void ModelReader::WriteMesh(string savePath)
{
	Utility::CreateFolders(savePath);

	BinaryWriter* w = new BinaryWriter(Utility::ToWString(savePath));

	w->UInt(bones.size());
	for (BoneData* bone : bones)
	{
		w->Int(bone->index);
		w->String(bone->name);
		w->Int(bone->parent);

		// xmmatrix와 형변환
		XMFLOAT4X4 temp;
		XMStoreFloat4x4(&temp, bone->transform);
		w->Float4x4(temp);

		delete bone;
	}
	bones.clear();

	w->UInt(meshes.size());
	for (MeshData* mesh : meshes)
	{
		w->String(mesh->name);
		w->Int(mesh->boneIndex);

		w->UInt(mesh->vertices.size());
		w->Byte(mesh->vertices.data(), sizeof(ModelVertex) * mesh->vertices.size());

		w->UInt(mesh->indices.size());
		w->Byte(mesh->indices.data(), sizeof(UINT) * mesh->indices.size());

		w->UInt(mesh->meshParts.size());
		for (MeshPartData* part : mesh->meshParts)
		{
			w->String(part->name);
			w->String(part->materialName);

			w->UInt(part->startVertex);
			w->UInt(part->vertexCount);

			w->UInt(part->startIndex);
			w->UInt(part->indexCount);

			delete part;
		}
		mesh->meshParts.clear();

		delete mesh;
	}
	meshes.clear();

	delete w;
}


void ModelReader::ExportClip(UINT index, string savePath)
{
	Clip* clip = ReadClip(scene->mAnimations[index]);
	savePath = "ModelData/Clips/" + savePath + ".clip";
	WriteClip(clip, savePath);
}

Clip* ModelReader::ReadClip(aiAnimation* animation)
{
	Clip* clip = new Clip();
	clip->name = animation->mName.C_Str();
	clip->frameRate = (float)animation->mTicksPerSecond;
	clip->frameCount = (UINT)animation->mDuration + 1;

	vector<ClipNode> nodeInfos;
	for (UINT i = 0; i < animation->mNumChannels; i++)
	{
		aiNodeAnim* aniNode = animation->mChannels[i];

		ClipNode nodeInfo;
		nodeInfo.name = aniNode->mNodeName;

		UINT keyCount = max(aniNode->mNumPositionKeys, aniNode->mNumRotationKeys);
		keyCount = max(keyCount, aniNode->mNumScalingKeys);

		KeyTransform transform;
		for (UINT k = 0; k < keyCount; k++)
		{
			bool isFound = false;
			UINT t = nodeInfo.keyFrame.size();

			// 가끔 요기서 터짐
			if (abs((float)aniNode->mPositionKeys[k].mTime - (float)t) <= FLT_EPSILON)
			{
				aiVectorKey key = aniNode->mPositionKeys[k];

				memcpy_s(&transform.position, sizeof(XMFLOAT3),
					&key.mValue, sizeof(aiVector3D));

				transform.time = (float)aniNode->mPositionKeys[k].mTime;
				isFound = true;
			}

			if (abs((float)aniNode->mRotationKeys[k].mTime - (float)t) <= FLT_EPSILON)
			{
				aiQuatKey key = aniNode->mRotationKeys[k];

				transform.rotation.x = (float)key.mValue.x;
				transform.rotation.y = (float)key.mValue.y;
				transform.rotation.z = (float)key.mValue.z;
				transform.rotation.w = (float)key.mValue.w;

				transform.time = (float)aniNode->mRotationKeys[k].mTime;

				isFound = true;
			}

			if (abs((float)aniNode->mScalingKeys[k].mTime - (float)t) <= FLT_EPSILON)
			{
				aiVectorKey key = aniNode->mScalingKeys[k];

				memcpy_s(&transform.scale, sizeof(XMFLOAT3),
					&key.mValue, sizeof(aiVector3D));

				transform.time = (float)aniNode->mScalingKeys[k].mTime;
				isFound = true;
			}

			if (isFound)
				nodeInfo.keyFrame.emplace_back(transform);
		}// KeyTransform

		if (nodeInfo.keyFrame.size() < clip->frameCount)
		{
			UINT count = clip->frameCount - nodeInfo.keyFrame.size();

			KeyTransform keyTransform = nodeInfo.keyFrame.back();

			for (UINT n = 0; n < count; n++)
				nodeInfo.keyFrame.emplace_back(keyTransform);
		}
		clip->duration = max(clip->duration, nodeInfo.keyFrame.back().time);

		nodeInfos.emplace_back(nodeInfo);
	}// KeyChannel

	ReadKeyFrame(clip, scene->mRootNode, nodeInfos);

	return clip;
}

void ModelReader::ReadKeyFrame(Clip* clip, aiNode* node, vector<ClipNode>& nodeInfos)
{
	KeyFrame* keyFrame = new KeyFrame();
	keyFrame->boneName = node->mName.C_Str();

	for (UINT i = 0; i < clip->frameCount; i++)
	{
		ClipNode* clipNode = nullptr;
		for (UINT n = 0; n < nodeInfos.size(); n++)
		{
			if (nodeInfos[n].name == node->mName)
			{
				clipNode = &nodeInfos[n];
				break;
			}
		}

		KeyTransform keyTransform;
		if (clipNode == nullptr)
		{
			XMMATRIX transform(node->mTransformation[0]);
			transform = XMMatrixTranspose(transform);

			XMVECTOR scale;
			XMVECTOR rotation;
			XMVECTOR position;
			
			// 계속 움직이니까 벡터의 주소값을 받아준다. 중요!
			// Decompose : Matrix 분해!!
			XMMatrixDecompose(&scale, &rotation, &position, transform);
			XMStoreFloat3(&keyTransform.scale, scale);
			XMStoreFloat4(&keyTransform.rotation, rotation);
			XMStoreFloat3(&keyTransform.position, position);

			keyTransform.time = (float)i;
		}
		else
		{
			keyTransform = clipNode->keyFrame[i];
		}
		keyFrame->transforms.emplace_back(keyTransform);
	}
	clip->keyFrame.emplace_back(keyFrame);

	for (UINT i = 0; i < node->mNumChildren; i++)
		ReadKeyFrame(clip, node->mChildren[i], nodeInfos);

}

void ModelReader::WriteClip(Clip* clip, string savePath)
{
	Utility::CreateFolders(savePath);

	BinaryWriter* w = new BinaryWriter(Utility::ToWString(savePath));

	w->String(clip->name);
	w->Float(clip->duration);
	w->Float(clip->frameRate);
	w->UInt(clip->frameCount);

	w->UInt(clip->keyFrame.size());
	for (KeyFrame* keyFrame : clip->keyFrame)
	{
		w->String(keyFrame->boneName);

		w->UInt(keyFrame->transforms.size());
		w->Byte(keyFrame->transforms.data(), sizeof(KeyTransform) * keyFrame->transforms.size());

		delete keyFrame;
	}
	clip->keyFrame.clear();

	delete clip;

	delete w;
}
