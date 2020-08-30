#include "Framework.h"

TerrainEditor::TerrainEditor(UINT width, UINT height)
	:width(width), height(height), isRaise(1), adjustValue(50),
	mode(0), paintValue(5.0f), selectMap(0), objectRotation(0, 0, 0), objectScale(1, 1, 1)
{
	// default Material
	material = new Material(L"SplattingAdvanced");
	material->SetDiffuseMap(L"Terrain/brown_mud_leaves_01_diff_1k.png");
	material->SetSpecularMap(L"Terrain/brown_mud_leaves_01_spec_1k.png");
	material->SetNormalMap(L"Terrain/brown_mud_leaves_01_Nor_1k.png");

	CreateData();
	CreateNormal();
	CreateTangent();

	mesh = new Mesh(vertices.data(), sizeof(VertexType), vertices.size(),
		indices.data(), indices.size());

	CreateCompute();
	brushBuffer = new BrushBuffer();

	secondMap = Texture::Add(L"Terrain/brown_mud_dry_diff_1k.png");
	secondSMap = Texture::Add(L"Terrain/brown_mud_dry_spec_1k.png");
	secondNMap = Texture::Add(L"Terrain/brown_mud_dry_nor_1k.png");
	thirdMap = Texture::Add(L"Terrain/coral_mud_01_diff_1k.png");
	thirdSMap = Texture::Add(L"Terrain/coral_mud_01_spec_1k.png");
	thirdNMap = Texture::Add(L"Terrain/coral_mud_01_Nor_1k.png");
	fourthMap = Texture::Add(L"Terrain/snow_02_diff_1k.png");
	fourthSMap = Texture::Add(L"Terrain/snow_02_spec_1k.png");
	fourthNMap = Texture::Add(L"Terrain/snow_02_nor_1k.png");
	fifthMap = Texture::Add(L"Terrain/brown_mud_rocks_01_diff_1k.png");
	fifthSMap = Texture::Add(L"Terrain/brown_mud_rocks_01_spec_1k.png");
	fifthNMap = Texture::Add(L"Terrain/brown_mud_rocks_01_nor_1k.png");
}

TerrainEditor::~TerrainEditor()
{
	delete material;
	delete mesh;

	delete rayBuffer;
	delete structuredBuffer;

	delete[] input;
	delete[] output;

	delete brushBuffer;

	if (cubes.size())
	{
		for (auto cube : cubes)
			delete cube;
	}
	if (spheres.size())
	{
		for (auto sphere : spheres)
			delete sphere;
	}
}

void TerrainEditor::Update()
{
	Brushing();

	UpdateWorld();

	if (cubes.size())
	{
		for (auto cube : cubes)
			cube->Update();
	}
	if (spheres.size())
	{
		for (auto sphere : spheres)
			sphere->Update();
	}
}

void TerrainEditor::Render()
{
	mesh->Set();

	SetWorldBuffer();
	brushBuffer->SetBufferToVS(3);

	secondMap->PSSet(10);
	secondSMap->PSSet(11);
	secondNMap->PSSet(12);
	thirdMap->PSSet(20);
	thirdSMap->PSSet(21);
	thirdNMap->PSSet(22);
	fourthMap->PSSet(30);
	fourthSMap->PSSet(31);
	fourthNMap->PSSet(32);
	fifthMap->PSSet(40);
	fifthSMap->PSSet(41);
	fifthNMap->PSSet(42);

	material->Set();

	DC->DrawIndexed(indices.size(), 0, 0);

	if (cubes.size())
	{
		for (auto cube : cubes)
			cube->Render();
	}
	if (spheres.size())
	{
		for (auto sphere : spheres)
			sphere->Render();
	}

}


void TerrainEditor::PostRender()
{
	ImGui::ShowDemoWindow();

	ImGui::Begin("TerrainEditor", 0, ImGuiWindowFlags_AlwaysAutoResize);
	{
		ImGui::RadioButton("SetHeight", &mode, 0); ImGui::SameLine();
		ImGui::RadioButton("Painting", &mode, 1); ImGui::SameLine();
		ImGui::RadioButton("Cube", &mode, 2); ImGui::SameLine();
		ImGui::RadioButton("Sphere", &mode, 3);

		ImGui::RadioButton("Up(Paint)", &isRaise, 1); ImGui::SameLine();
		ImGui::RadioButton("Down(Erase)", &isRaise, 0);

		ImGui::SliderFloat("AdjustValue", &adjustValue, 0, 300);
		ImGui::SliderFloat("PaintValue", &paintValue, 0, 10);
		ImGui::SliderInt("SelectMap", &selectMap, 0, 3);
		ImGui::SliderInt("Type", &brushBuffer->data.type, 1, 2);
		ImGui::SliderFloat("Range", &brushBuffer->data.range, 1, 20);

		ImGui::SliderFloat3("Rotation", (float*)&objectRotation, 0, XM_2PI);
		ImGui::SliderFloat3("Scale", (float*)&objectScale, 0.1f, 10.0f);

		if (ImGui::Button("Save"))
			Save();
		ImGui::SameLine();
		if (ImGui::Button("Load"))
			Load();

		igfd::ImGuiFileDialog::Instance()->SetExtentionInfos(".cpp", ImVec4(0, 0, 1, 0.9));
		igfd::ImGuiFileDialog::Instance()->SetExtentionInfos(".h", ImVec4(0, 1, 0, 0.9));
		igfd::ImGuiFileDialog::Instance()->SetExtentionInfos(".jpg", ImVec4(1, 1, 0, 0.9));
		igfd::ImGuiFileDialog::Instance()->SetExtentionInfos(".png", ImVec4(1, 0, 1, 0.9));
		const char* filters = "Source files (*.cpp *.h *.hpp){.cpp,.h,.hpp},Image files (*.png *.gif *.jpg *.jpeg){.png,.gif,.jpg,.jpeg},.md";
		if (ImGui::Button("SaveHeightMap"))
		{
			igfd::ImGuiFileDialog::Instance()->OpenDialog("SaveHeight", "Choose a File", filters, ".");
		}
		if (igfd::ImGuiFileDialog::Instance()->FileDialog("SaveHeight"))
		{
			if (igfd::ImGuiFileDialog::Instance()->IsOk == true)
			{
				string filePathName = igfd::ImGuiFileDialog::Instance()->GetFilepathName();
				string filePath = igfd::ImGuiFileDialog::Instance()->GetCurrentPath();
				
				wstring path;
				path = path.assign(filePathName.begin(), filePathName.end());
				SaveHeightMap(path);
			}
			igfd::ImGuiFileDialog::Instance()->CloseDialog("SaveHeight");
		}
		
		ImGui::SameLine();
		if (ImGui::Button("SaveAlphaMap"))
		{
			igfd::ImGuiFileDialog::Instance()->OpenDialog("SaveAlpha", "Choose a File", filters, ".");
		}
	
		if (igfd::ImGuiFileDialog::Instance()->FileDialog("SaveAlpha"))
		{
			if (igfd::ImGuiFileDialog::Instance()->IsOk == true)
			{
				string filePathName = igfd::ImGuiFileDialog::Instance()->GetFilepathName();
				string filePath = igfd::ImGuiFileDialog::Instance()->GetCurrentPath();

				wstring path;
				path = path.assign(filePathName.begin(), filePathName.end());
				SaveAlphaMap(path);
			}
			igfd::ImGuiFileDialog::Instance()->CloseDialog("SaveAlpha");
		}
		


		if (ImGui::Button("LoadHeightMap"))
		{
			igfd::ImGuiFileDialog::Instance()->OpenDialog("LoadHeight", "Choose a File", filters, ".");
		}
		if (igfd::ImGuiFileDialog::Instance()->FileDialog("LoadHeight"))
		{
			if (igfd::ImGuiFileDialog::Instance()->IsOk == true)
			{
				string filePathName = igfd::ImGuiFileDialog::Instance()->GetFilepathName();
				string filePath = igfd::ImGuiFileDialog::Instance()->GetCurrentPath();

				wstring path;
				path = path.assign(filePathName.begin(), filePathName.end());
				LoadHeightMap(path);
			}
			igfd::ImGuiFileDialog::Instance()->CloseDialog("LoadHeight");
		}
		ImGui::SameLine();
		if (ImGui::Button("LoadAlphaMap"))
		{
			igfd::ImGuiFileDialog::Instance()->OpenDialog("LoadAlpha", "Choose a File", filters, ".");
		}
		if (igfd::ImGuiFileDialog::Instance()->FileDialog("LoadAlpha"))
		{
			if (igfd::ImGuiFileDialog::Instance()->IsOk == true)
			{
				string filePathName = igfd::ImGuiFileDialog::Instance()->GetFilepathName();
				string filePath = igfd::ImGuiFileDialog::Instance()->GetCurrentPath();

				wstring path;
				path = path.assign(filePathName.begin(), filePathName.end());
				LoadAlphaMap(path);
			}
			igfd::ImGuiFileDialog::Instance()->CloseDialog("LoadAlpha");
		}
	}
	ImGui::End();

}

bool TerrainEditor::ComputePicking(OUT Vector3* position)
{
	Ray ray = Environment::Get()->MainCamera()->ScreenPointToRay(Mouse::Get()->GetPosition());
	
	rayBuffer->data.position = ray.position;
	rayBuffer->data.direction = ray.direction;
	rayBuffer->data.size = size;
	computeShader->Set();

	rayBuffer->SetBufferToCS(0);

	DC->CSSetShaderResources(0, 1, &structuredBuffer->GetSRV());
	DC->CSSetUnorderedAccessViews(0, 1, &structuredBuffer->GetUAV(), nullptr);

	// ceil �ø�				1024 ������ ����
	UINT x = ceil((float)size / 1024.0f);

	DC->Dispatch(x, 1, 1);

	structuredBuffer->Copy(output, sizeof(OutputStruct) * size);

	float minDistance = FLT_MAX;
	int minIndex = -1;

	for (UINT i = 0; i < size; i++)
	{
		OutputStruct temp = output[i];
		if (temp.picked)
		{
			if (minDistance > temp.distance)
			{
				minDistance = temp.distance;
				minIndex = i;
			}
		}
	}

	if (minIndex >= 0)
	{
		*position = ray.position + ray.direction * minDistance;
		return true;
	}


	return false;
}

void TerrainEditor::AdjustY(Vector3 position, float value)
{
	switch (brushBuffer->data.type)
	{
	case 1:
	{
		for (VertexType& vertex : vertices)
		{
			Vector3 p1 = Vector3(vertex.position.x, 0, vertex.position.z);
			Vector3 p2 = Vector3(position.x, 0, position.z);

			float dist = (p2 - p1).Length();

			float temp = value * max(0, cos(XM_PIDIV2 * dist / brushBuffer->data.range));

			if (dist <= brushBuffer->data.range)
			{
				vertex.position.y += temp * Time::Delta();

				if (vertex.position.y < 0)
					vertex.position.y = 0.0f;

				float maxHeight = 60;

				if (vertex.position.y > maxHeight)
					vertex.position.y = maxHeight;
			}
		}
	}
	break;
	case 2:
	{
		for (VertexType& vertex : vertices)
		{
			if (position.x - brushBuffer->data.range <= vertex.position.x
				&& position.x + brushBuffer->data.range >= vertex.position.x
				&& position.z - brushBuffer->data.range <= vertex.position.z
				&& position.z + brushBuffer->data.range >= vertex.position.z)
			{
				vertex.position.y += value * Time::Delta();

				if (vertex.position.y < 0)
					vertex.position.y = 0.0f;

				float maxHeight = 60;

				if (vertex.position.y > maxHeight)
					vertex.position.y = maxHeight;
			}
		}
	}

	break;
	default:
		break;

	}

	mesh->UpdateVertex(vertices.data(), vertices.size());
}

void TerrainEditor::PaintBrush(Vector3 position, float value)
{
	switch (brushBuffer->data.type)
	{
	case 1:
	{
		for (VertexType& vertex : vertices)
		{
			Vector3 p1 = Vector3(vertex.position.x, 0, vertex.position.z);
			Vector3 p2 = Vector3(position.x, 0, position.z);

			float dist = (p2 - p1).Length();

			float temp = value * max(0, cos(XM_PIDIV2 * dist / brushBuffer->data.range));

			if (dist <= brushBuffer->data.range)
			{
				vertex.alpha[selectMap] += temp * Time::Delta();
				vertex.alpha[selectMap] = GameMath::Saturate(vertex.alpha[selectMap]);
			}
		}
	}
	default:
		break;
	}

	mesh->UpdateVertex(vertices.data(), vertices.size());
}

void TerrainEditor::Save()
{
	BinaryWriter* writer = new BinaryWriter(L"TextData/SaveObject.object");

	writer->String("Cube");
	writer->UInt(cubes.size());
	if (cubes.size())
	{
		for (auto cube : cubes)
		{
			writer->Byte(&cube->position,sizeof(Vector3));
			writer->Byte(&cube->rotation, sizeof(Vector3));
			writer->Byte(&cube->scale, sizeof(Vector3));
		}
	}

	writer->String("Sphere");
	writer->UInt(spheres.size());
	if (spheres.size())
	{
		for (auto sphere : spheres)
		{
			writer->Byte(&sphere->position, sizeof(Vector3));
			writer->Byte(&sphere->rotation, sizeof(Vector3));
			writer->Byte(&sphere->scale, sizeof(Vector3));
		}
	}

	delete writer;
}

void TerrainEditor::Load()
{
	BinaryReader* reader = new BinaryReader(L"TextData/SaveObject.object");

	string objName = reader->String();
	UINT size = reader->UInt();
	if (size)
	{
		cubes.resize(size);
		for (int i = 0; i < size; i++)
		{
			Cube* tmp = new Cube();
			void* data = &tmp->position;
			reader->Byte(&data, sizeof(Vector3));
			void* data2 = &tmp->rotation;
			reader->Byte(&data2, sizeof(Vector3));
			void* data3 = &tmp->scale;
			reader->Byte(&data3, sizeof(Vector3));
			cubes[i]= tmp;
			cubes[i]->UpdateWorld();
		}
	}

	string objName2 = reader->String();
	UINT size2 = reader->UInt();
	if (size2)
	{
		spheres.resize(size2);
		for (int i = 0; i < size2; i++)
		{
			Sphere* tmp = new Sphere(5, 50, 50);
			void* data = &tmp->position;
			reader->Byte(&data, sizeof(Vector3));
			void* data2 = &tmp->rotation;
			reader->Byte(&data2, sizeof(Vector3));
			void* data3 = &tmp->scale;
			reader->Byte(&data3, sizeof(Vector3));
			spheres[i] = tmp;
			spheres[i]->UpdateWorld();
		}
	}

	delete reader;
}

void TerrainEditor::SaveHeightMap(wstring path)
{
	UINT size = width * height * 4;
	uint8_t* pixels = new uint8_t[size];

	for (UINT i = 0; i < size / 4; i++)
	{
		float y = (vertices[i].position.y) / 20.0f * 255.0f;

		for (UINT j = 0; j < 3; j++)
		{
			pixels[i * 4 + j] = y >= 255 ? 255 : y;

			y -= 255;

			if (y < 0)
				y = 0;
		}

		pixels[i * 4 + 3] = 255;
	}

	Image image;

	image.width = width;
	image.height = height;
	image.pixels = pixels;
	image.format = DXGI_FORMAT_R8G8B8A8_UNORM;
	image.rowPitch = width * 4;

	image.slicePitch = image.width * image.height * 4;

	SaveToWICFile(image, WIC_FLAGS_FORCE_RGB, GetWICCodec(WIC_CODEC_PNG), path.c_str());

}

void TerrainEditor::SaveAlphaMap(wstring path)
{
	UINT size = width * height * 4;
	uint8_t* pixels = new uint8_t[size];

	for (UINT i = 0; i < size / 4; i++)
	{
		for (UINT j = 0; j < 4; j++)
		{
			pixels[i * 4 + j] = (uint8_t)(vertices[i].alpha[j] * 255);
		}
		//pixels[i * 4 + 3] = 255;
	}

	Image image;

	image.width = width;
	image.height = height;
	image.pixels = pixels;
	image.format = DXGI_FORMAT_R8G8B8A8_UNORM;
	image.rowPitch = (size_t)width * 4;

	image.slicePitch = image.width * image.height * 4;

	SaveToWICFile(image, WIC_FLAGS_FORCE_RGB, GetWICCodec(WIC_CODEC_PNG), path.c_str());
}

void TerrainEditor::LoadHeightMap(wstring path)
{
	ScratchImage image;

	HRESULT hr = LoadFromWICFile(path.c_str(), WIC_FLAGS_FORCE_RGB, nullptr, image);
	assert(SUCCEEDED(hr));
	

	ID3D11ShaderResourceView* srv;

	hr = CreateShaderResourceView(DEVICE, image.GetImages(), image.GetImageCount(),
		image.GetMetadata(), &srv);
	assert(SUCCEEDED(hr));


	UINT width = image.GetMetadata().width - 1;
	UINT height = image.GetMetadata().height - 1;

	vector<XMFLOAT4> pixels;

	uint8_t* colors = image.GetPixels();
	UINT size = image.GetPixelsSize();

	float scale = 1.0f / 255.0f;

	for (int i = 0; i < size; i += 4)
	{
		XMFLOAT4 color;

		color.x = colors[i + 0] * scale;
		color.y = colors[i + 1] * scale;
		color.z = colors[i + 2] * scale;
		color.w = colors[i + 3] * scale;

		pixels.emplace_back(color);
	}
	

	for (UINT i = 0; i < pixels.size(); i++)
	{
		vertices[i].position.y = 0;
		vertices[i].position.y += pixels[i].x * 20.0f;
		vertices[i].position.y += pixels[i].y * 20.0f;
		vertices[i].position.y += pixels[i].z * 20.0f;
	}


	CreateNormal();
	CreateTangent();
	CreateCompute();

	srv->Release();
	srv = nullptr;

	mesh->UpdateVertex(vertices.data(), vertices.size());
}

void TerrainEditor::LoadAlphaMap(wstring path)
{
	ScratchImage image;

	HRESULT hr = LoadFromWICFile(path.c_str(), WIC_FLAGS_FORCE_RGB, nullptr, image);
	assert(SUCCEEDED(hr));


	ID3D11ShaderResourceView* srv;

	hr = CreateShaderResourceView(DEVICE, image.GetImages(), image.GetImageCount(),
		image.GetMetadata(), &srv);
	assert(SUCCEEDED(hr));


	UINT width = image.GetMetadata().width - 1;
	UINT height = image.GetMetadata().height - 1;

	vector<XMFLOAT4> pixels;

	uint8_t* colors = image.GetPixels();
	UINT size = image.GetPixelsSize();

	float scale = 1.0f / 255.0f;

	for (int i = 0; i < size; i += 4)
	{
		XMFLOAT4 color;

		color.x = colors[i + 0] * scale;
		color.y = colors[i + 1] * scale;
		color.z = colors[i + 2] * scale;
		color.w = colors[i + 3] * scale;

		pixels.emplace_back(color);
	}


	for (UINT i = 0; i < pixels.size(); i++)
	{
		vertices[i].alpha[0] = pixels[i].x;
		vertices[i].alpha[1] = pixels[i].y;
		vertices[i].alpha[2] = pixels[i].z;
		vertices[i].alpha[3] = pixels[i].w;
	}

	srv->Release();
	srv = nullptr;

	mesh->UpdateVertex(vertices.data(), vertices.size());
}

void TerrainEditor::Brushing()
{
	Vector3 temp;
	ComputePicking(&temp);
	brushBuffer->data.location = temp;

	if (KEY_PRESS(VK_LCONTROL))
	{
		if (KEY_PRESS(VK_LBUTTON))
		{
			// set height
			if (mode == 0)
			{
				if (isRaise)
					AdjustY(temp, adjustValue);
				else
					AdjustY(temp, -adjustValue);
			}
			// painting
			else if (mode == 1)
			{
				if (isRaise)
					PaintBrush(brushBuffer->data.location, paintValue);
				else
					PaintBrush(brushBuffer->data.location, -paintValue);
			}
		}

		if (KEY_DOWN(VK_LBUTTON) && mode == 2)
		{
			if (isRaise)
				CreateCube(brushBuffer->data.location, objectRotation, objectScale);
		}

		if (KEY_DOWN(VK_LBUTTON) && mode == 3)
		{
			if (isRaise)
				CreateSphere(brushBuffer->data.location, objectRotation, objectScale);
		}

		if (KEY_UP(VK_LBUTTON) && mode == 0)
		{
			CreateNormal();
			CreateTangent();
			CreateCompute();
			mesh->UpdateVertex(vertices.data(), vertices.size());
		}

	}
}

void TerrainEditor::CreateData()
{
	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			VertexType vertex;
			vertex.position = XMFLOAT3(x, 0, z);
			vertex.uv = XMFLOAT2((x / (float)width)*3, (1.0f - (z / (float)height))*3);


			vertices.emplace_back(vertex);
		}
	}

	for (UINT z = 0; z < height - 1; z++)
	{
		for (UINT x = 0; x < width - 1; x++)
		{
			indices.emplace_back(width * z + x);
			indices.emplace_back(width * (z + 1) + x);
			indices.emplace_back(width * (z + 1) + x + 1);
			indices.emplace_back(width * z + x);
			indices.emplace_back(width * (z + 1) + x + 1);
			indices.emplace_back(width * z + x + 1);
		}
	}

	
}

void TerrainEditor::CreateNormal()
{
	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		Vector3 v0 = vertices[index0].position;
		Vector3 v1 = vertices[index1].position;
		Vector3 v2 = vertices[index2].position;

		Vector3 A = v1 - v0;
		Vector3 B = v2 - v0;

		Vector3 normal = Vector3::Cross(A, B).Normal();

		vertices[index0].normal = (vertices[index0].normal + normal).Normal();
		vertices[index1].normal = (vertices[index1].normal + normal).Normal();
		vertices[index2].normal = (vertices[index2].normal + normal).Normal();
	}
}

void TerrainEditor::CreateTangent()
{
	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		VertexType vertex0 = vertices[index0];
		VertexType vertex1 = vertices[index1];
		VertexType vertex2 = vertices[index2];

		Vector3 p0 = vertex0.position;
		Vector3 p1 = vertex1.position;
		Vector3 p2 = vertex2.position;

		XMFLOAT2 uv0 = vertex0.uv;
		XMFLOAT2 uv1 = vertex1.uv;
		XMFLOAT2 uv2 = vertex2.uv;

		Vector3 e0 = p1 - p0;
		Vector3 e1 = p2 - p0;

		float u0 = uv1.x - uv0.x;
		float u1 = uv2.x - uv0.x;
		float v0 = uv1.y - uv0.y;
		float v1 = uv2.y - uv0.y;

		float d = 1.0f / (u0 * v1 - v0 * u1);

		Vector3 tangent;
		tangent = (v1 * e0 - v0 * e1) * d;

		vertices[index0].tangent = tangent + vertices[index0].tangent;
		vertices[index1].tangent = tangent + vertices[index1].tangent;
		vertices[index2].tangent = tangent + vertices[index2].tangent;

	}

	for (VertexType& vertex : vertices)
	{
		Vector3 t = vertex.tangent;
		Vector3 n = vertex.normal;

		Vector3 temp = (t - n * Vector3::Dot(n, t)).Normal();

		vertex.tangent = temp;
	}
}

void TerrainEditor::CreateCompute()
{
	input = new InputStruct[indices.size() / 3];
	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		input[i].v0 = vertices[index0].position;
		input[i].v1 = vertices[index1].position;
		input[i].v2 = vertices[index2].position;

		input[i].index = i;
	}

	computeShader = Shader::AddCS(L"Intersection");

	// ������ ����
	size = indices.size() / 3;

	if (structuredBuffer)
	{
		delete structuredBuffer;
		structuredBuffer = nullptr;
	}

	structuredBuffer = new StructuredBuffer(input, sizeof(InputStruct), size,
		sizeof(OutputStruct), size);

	rayBuffer = new RayBuffer();
	output = new OutputStruct[size];
}

void TerrainEditor::CreateCube(Vector3 position, Vector3 rotation, Vector3 scale)
{
	Cube* tmp = new Cube();
	tmp->position = position;
	tmp->rotation = rotation;
	tmp->scale = scale;
	tmp->UpdateWorld();
	cubes.push_back(tmp);
}

void TerrainEditor::CreateSphere(Vector3 position, Vector3 rotation, Vector3 scale)
{
	Sphere* tmp = new Sphere(5, 50, 50);
	tmp->position = position + scale.y;
	tmp->rotation = rotation;
	tmp->scale = scale;
	tmp->UpdateWorld();
	spheres.push_back(tmp);
}
