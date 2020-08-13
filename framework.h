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

// 프로젝트 속성 - 링커 - 입력 - 추가 종속성에서 추가해 줘도 된다.
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
// interface id 있는 라이브러리 
#pragma comment(lib, "dxguid.lib")

// ImGui
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_dx11.h"
#include "../ImGui/imgui_impl_win32.h"

// DirectXTex
#include "../DirectXTex/DirectXTex.h"

using namespace DirectX;
using namespace std;

// Framework Header
#include "Framework/Device/Device.h"

// Buffers
#include "Framework/Buffers/VertexBuffer.h"
#include "Framework/Buffers/IndexBuffer.h"
#include "Framework/Buffers/ConstantBuffer.h"
#include "Framework/Buffers/GlobalBuffer.h"

// Shaders
#include "Framework/Shaders/VertexShader.h"
#include "Framework/Shaders/PixelShader.h"

// 계산 자료형
#include "Framework/Math/Transform.h"

#include "Framework/State/SamplerState.h"
#include "Framework/State/RasterizerState.h"


// System Object Header
#include "Framework/System/Camera.h"

// GameObject Header
#include "GameObject/Quad.h"
#include "GameObject/Cube.h"
#include "GameObject/Plane.h"

// 게임 씬과 게임 프로세스
#include "Scenes/Scene.h"
#include "MyGame/Program.h"

// 외부 전역 변수
extern HWND hWnd;