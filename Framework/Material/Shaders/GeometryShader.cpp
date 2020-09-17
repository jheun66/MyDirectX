#include "Framework.h"

GeometryShader::GeometryShader(wstring file, string gsName)
{
    wstring path = L"Shaders/GeometryShaders/" + file + L".hlsl";
    // 컴파일 관련 기본 옵션
    DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

    HRESULT hr;
    hr = D3DCompileFromFile(path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        gsName.c_str(), "gs_5_0", flags, 0, &geometryBlob, nullptr);
    assert(SUCCEEDED(hr));

    hr = DEVICE->CreateGeometryShader(geometryBlob->GetBufferPointer(),
        geometryBlob->GetBufferSize(), nullptr, &geometryShader);
    assert(SUCCEEDED(hr));
}

GeometryShader::~GeometryShader()
{
    geometryShader->Release();
    geometryBlob->Release();
}

void GeometryShader::Set()
{
    DC->GSSetShader(geometryShader, nullptr, 0);
}
