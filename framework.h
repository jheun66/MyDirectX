// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN

// 화면 크기
#define WIN_WIDTH 1280
#define WIN_HEIGHT 720

// 윈도우 왼쪽 위 위치
#define WIN_START_X 100
#define WIN_START_Y 100

#define DEVICE Device::Get()->GetDevice()
#define DC Device::Get()->GetDeviceContext()

#define KEY_DOWN(k) Keyboard::Get()->Down(k)
#define KEY_PRESS(k) Keyboard::Get()->Press(k)
#define KEY_UP(k) Keyboard::Get()->Up(k)

// C/C++
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <assert.h>
#include <vector>
#include <map>
#include <string>

// direct 라이브러리 추가
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
// 충돌처리
#include <DirectXCollision.h>

// 프로젝트 속성 - 링커 - 입력 - 추가 종속성에서 추가해 줘도 된다.
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
// interface id 있는 라이브러리 
#pragma comment(lib, "dxguid.lib")

// ImGui
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <CustomFont.h>
#include <ImGuiFileDialog.h>

// DirectXTex
#include "../DirectXTex/DirectXTex.h"

//Assimp
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>

#pragma comment(lib, "Assimp/assimp-vc142-mtd.lib")

using namespace DirectX;
using namespace std;

// Framework Header
#include "Framework/Device/Device.h"

// Buffers
#include "Framework/Buffers/ConstantBuffer.h"
#include "Framework/Buffers/GlobalBuffer.h"
#include "Framework/Buffers/StructuredBuffer.h"
#include "Framework/Buffers/Mesh/VertexBuffer.h"
#include "Framework/Buffers/Mesh/IndexBuffer.h"
#include "Framework/Buffers/Mesh/Mesh.h"

// Material Shaders + Texture
#include "Framework/Material/Texture/Texture.h"
#include "Framework/Material/Shaders/Shader.h"
#include "Framework/Material/Shaders/VertexShader.h"
#include "Framework/Material/Shaders/PixelShader.h"
#include "Framework/Material/Shaders/ComputeShader.h"
#include "Framework/Material/Material.h"


#include "Framework/VertexLayout/VertexLayout.h"



// Math
#include "Framework/Math/Vector3.h"
#include "Framework/Math/Math.h"

// State
#include "Framework/State/SamplerState.h"
#include "Framework/State/RasterizerState.h"

// Environment
#include "Framework/Environment/Environment.h"

// Assimp
#include "Framework/Assimp/ModelTypes.h"
#include "Framework/Assimp/ModelReader.h"

// System
#include "Framework/Utility/Utility.h"
#include "Framework/Utility/Time.h"
#include "Framework/Utility/Keyboard.h"
#include "Framework/Utility/Mouse.h"
#include "Framework/Utility/FileSystem/BinaryWriter.h"
#include "Framework/Utility/FileSystem/BinaryReader.h"
#include "Framework/Utility/FileSystem/Xml.h"

// GameObject Header
#include "GameObject/Transform.h"
#include "GameObject/Camera/Camera.h"
#include "GameObject/Basic/Cube.h"
#include "GameObject/Basic/Quad.h"
#include "GameObject/Basic/Sphere.h"
#include "GameObject/LandScape/Terrain.h"
#include "GameObject/LandScape/TerrainEditor.h"
#include "GameObject/Custom/Orb.h"
#include "GameObject/Model/ModelMesh.h"
#include "GameObject/Model/Model.h"
// Custom Object
#include "GameObject/Custom/WarChief.h"
#include "GameObject/Custom/Tree.h"
#include "GameObject/Custom/Zombie.h"

// 게임 씬과 게임 프로세스
#include "Scenes/Scene.h"
#include "MyGame/Program.h"

// 외부 전역 변수
extern HWND hWnd;