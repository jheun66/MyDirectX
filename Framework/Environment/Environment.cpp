#include "framework.h"

Environment* Environment::instance = nullptr;

Environment::Environment()
{
    CreateViewort();
    CreateSamplerState();

    lightBuffer = new DirLightBuffer();
}

Environment::~Environment()
{
    delete samplerState;

    delete lightBuffer;
}

void Environment::CreateViewort()
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

void Environment::CreateSamplerState()
{
    samplerState = new SamplerState();
    //samplerState->Address(D3D11_TEXTURE_ADDRESS_CLAMP);
    samplerState->SetState();
}

void Environment::PostRender()
{
    ImGui::Text("FPS : %d", (int)Time::Get()->FPS());

    ImGui::SliderFloat3("LightDir", (float*)&lightBuffer->data.direction, -100, 100);
    ImGui::SliderFloat("LightSpecularExp", (float*)&lightBuffer->data.specularExp, 1, 100);
    ImGui::ColorEdit4("LightAmbient", (float*)&lightBuffer->data.ambient);
}
