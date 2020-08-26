#include "Framework.h"

ComputeShader::ComputeShader(wstring file, string csName)
{
    wstring path = L"Shaders/ComputeShaders/" + file + L".hlsl";
    // 컴파일 관련 기본 옵션
    DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

    HRESULT hr;
    hr = D3DCompileFromFile(path.c_str(), nullptr, nullptr,
        csName.c_str(), "cs_5_0", flags, 0, &computeBlob, nullptr);
    assert(SUCCEEDED(hr));

    hr = DEVICE->CreateComputeShader(computeBlob->GetBufferPointer(),
        computeBlob->GetBufferSize(), nullptr, &shader);
    assert(SUCCEEDED(hr));

}

ComputeShader::~ComputeShader()
{
    shader->Release();
    computeBlob->Release();
}

void ComputeShader::Set()
{
    DC->CSSetShader(shader, nullptr, 0);
}
