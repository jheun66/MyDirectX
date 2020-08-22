#include "framework.h"

Enviroment* Enviroment::instance = nullptr;

Enviroment::Enviroment()
{
    CreateViewort();
    CreateSamplerState();

    mainCamera = new Camera("mainCamera");
    lightBuffer = new DirLightBuffer();
}

Enviroment::~Enviroment()
{
    delete samplerState;

    delete mainCamera;
    delete lightBuffer;
}

void Enviroment::CreateViewort()
{
    D3D11_VIEWPORT vp;
    vp.Width = WIN_WIDTH;
    vp.Height = WIN_HEIGHT;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    DC->RSSetViewports(1, &vp);
}

void Enviroment::CreateSamplerState()
{
    samplerState = new SamplerState();
    //samplerState->Address(D3D11_TEXTURE_ADDRESS_CLAMP);
    samplerState->SetState();
}

void Enviroment::PostRender()
{
    ImGui::Text("FPS : %d", (int)Time::Get()->FPS());
    mainCamera->PostRender();

    ImGui::SliderFloat3("LightDir", (float*)&lightBuffer->data.direction, -100, 100);
    ImGui::SliderFloat("LightSpecularExp", (float*)&lightBuffer->data.specularExp, 1, 100);
    ImGui::ColorEdit4("LightAmbient", (float*)&lightBuffer->data.ambient);
}
