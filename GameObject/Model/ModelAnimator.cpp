#include "framework.h"

ModelAnimator::ModelAnimator(string file)
	:texture(nullptr), srv(nullptr)
{
	ReadMaterial(file);
	ReadMesh(file);

	SetShader(L"VertexModelAnimation", L"PixelNormalMapping");

	frameBuffer = new FrameBuffer();
}

ModelAnimator::~ModelAnimator()
{
	delete frameBuffer;
	delete[] clipTransform;

	texture->Release();
	srv->Release();

	for (ModelClip* clip : clips)
		delete clip;
}

void ModelAnimator::Update()
{
	KeyFrameDesc& desc = frameBuffer->data.keyFrame;
	ModelClip* clip = clips[desc.clip];

	float time = 1.0f / clip->frameRate / desc.speed;
	desc.runningTime += Time::Delta();

	if (desc.time >= 1.0f)	// 다음 프레임으로 넘기는 조건
	{
		desc.runningTime = 0.0f;

		//									루프 돌게
		desc.curFrame = (desc.curFrame + 1) % clip->frameCount;
		desc.nextFrame = (desc.curFrame + 1) % clip->frameCount;
	}

	desc.time = desc.runningTime / time;

	Model::Update();
}

void ModelAnimator::Render()
{
	if (texture == nullptr)
		CreateTexture();

	frameBuffer->SetBufferToVS(10);
	DC->VSSetShaderResources(10, 1, &srv);

	Model::Render();
}

void ModelAnimator::PlayClip(UINT clip, float speed)
{
	frameBuffer->data.keyFrame.clip = clip;
	frameBuffer->data.keyFrame.speed = speed;
}

void ModelAnimator::ReadClip(string file)
{
	file = "ModelData/Clips/" + file + ".clip";

	BinaryReader* r = new BinaryReader(Utility::ToWString(file));

	ModelClip* clip = new ModelClip();

	clip->name = r->String();
	clip->duration = r->Float();
	clip->frameRate = r->Float();
	clip->frameCount = r->UInt();

	UINT keyFrameCount = r->UInt();
	for (UINT i = 0; i < keyFrameCount; i++)
	{
		KeyFrame* keyFrame = new KeyFrame();
		keyFrame->boneName = r->String();

		UINT size = r->UInt();
		if (size > 0)
		{
			keyFrame->transforms.resize(size);

			void* ptr = (void*)keyFrame->transforms.data();
			r->Byte(&ptr, sizeof(KeyTransform) * size);
		}
		clip->keyFrameMap[keyFrame->boneName] = keyFrame;
	}
	clips.emplace_back(clip);

	delete r;
}

void ModelAnimator::CreateTexture()
{
	UINT clipCount = clips.size();

	clipTransform = new ClipTransform[clipCount];

	for (UINT i = 0; i < clipCount; i++)
		CreateClipTransform(i);

	{//Craete Texture
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = MAX_MODEL_BONE * 4;
		desc.Height = MAX_ANIM_KEY;
		desc.ArraySize = clipCount;
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;

		UINT pageSize = MAX_MODEL_BONE * 4 * 16 * MAX_ANIM_KEY;

		void* p = VirtualAlloc(nullptr, pageSize * clipCount, MEM_RESERVE, PAGE_READWRITE);

		for (UINT c = 0; c < clipCount; c++)
		{
			UINT start = c * pageSize;

			for (UINT y = 0; y < MAX_ANIM_KEY; y++)
			{
				void* temp = (BYTE*)p + MAX_MODEL_BONE * y * sizeof(XMMATRIX) + start;

				VirtualAlloc(temp, MAX_MODEL_BONE * sizeof(XMMATRIX), MEM_COMMIT, PAGE_READWRITE);
				memcpy(temp, clipTransform[c].transform[y], MAX_MODEL_BONE * sizeof(XMMATRIX));
			}
		}

		D3D11_SUBRESOURCE_DATA* subResource = new D3D11_SUBRESOURCE_DATA[clipCount];
		for (UINT c = 0; c < clipCount; c++)
		{
			void* temp = (BYTE*)p + c * pageSize;

			subResource[c].pSysMem = temp;
			subResource[c].SysMemPitch = MAX_MODEL_BONE * sizeof(XMMATRIX);
			subResource[c].SysMemSlicePitch = pageSize;
		}
		HRESULT hr = DEVICE->CreateTexture2D(&desc, subResource, &texture);
		assert(SUCCEEDED(hr));

		delete[] subResource;
		VirtualFree(p, 0, MEM_RELEASE);
	}


	{// Create SRV
		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		desc.Texture2DArray.MipLevels = 1;
		desc.Texture2DArray.ArraySize = clipCount;

		HRESULT hr = DEVICE->CreateShaderResourceView(texture, &desc, &srv);
		assert(SUCCEEDED(hr));
	}
}

void ModelAnimator::CreateClipTransform(UINT index)
{
	XMMATRIX* boneTransforms = new XMMATRIX[MAX_MODEL_BONE];

	ModelClip* clip = clips[index];
	for (UINT f = 0; f < clip->frameCount; f++)
	{
		for (UINT b = 0; b < bones.size(); b++)
		{
			ModelBone* bone = bones[b];

			XMMATRIX parent;
			XMMATRIX invGlobal = XMMatrixInverse(nullptr, bone->transform);

			int parentIndex = bone->parentIndex;
			if (parentIndex < 0)
				parent = XMMatrixIdentity();
			else
				parent = boneTransforms[parentIndex];

			XMMATRIX animation;
			KeyFrame* frame = clip->GetKeyFrame(bone->name);
			if (frame != nullptr)
			{
				KeyTransform& transform = frame->transforms[f];

				XMMATRIX S = XMMatrixScaling(transform.scale.x, transform.scale.y, transform.scale.z);
				XMMATRIX R = XMMatrixRotationQuaternion(XMLoadFloat4(&transform.rotation));
				XMMATRIX T = XMMatrixTranslation(transform.position.x, transform.position.y, transform.position.z);

				animation = S * R * T;
			}
			else
			{
				animation = XMMatrixIdentity();
			}

			boneTransforms[b] = animation * parent;
			clipTransform[index].transform[f][b] = invGlobal * boneTransforms[b];
		}
	}
}
