#include "framework.h"

Environment* Environment::instance = nullptr;

Environment::Environment()
{
    CreateViewort();
    CreateSamplerState();

    lightBuffer = new LightBuffer();
    
    LightInfo lightInfo;
    lightInfo.type = LightInfo::DIRECTION;
    lightInfo.direction = { 0,-1, 1 };

    lightBuffer->Add(lightInfo);
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

    ImGui::Text("LightInfo");

    ImGui::ColorEdit4("Ambient", (float*)&lightBuffer->data.ambient);
    ImGui::ColorEdit4("AmbientCeil", (float*)&lightBuffer->data.ambientCeil);

    if (ImGui::Button("AddLight"))
    {
        lightBuffer->Add();
    }
    for (UINT i = 0; i < lightBuffer->data.lightCount; i++)
    {
        string menuName = "Light" + to_string(i);
        if (ImGui::BeginMenu(menuName.c_str()))
        {
            ImGui::SliderInt("Type", (int*)&lightBuffer->data.lights[i].type, 0, 3);
            ImGui::SliderFloat3("Position", (float*)&lightBuffer->data.lights[i].position, -100, 100);
            ImGui::SliderFloat("Range", &lightBuffer->data.lights[i].range, 0, 1000);
            ImGui::ColorEdit4("Color", (float*)&lightBuffer->data.lights[i].color);
            ImGui::SliderFloat3("Direction", (float*)&lightBuffer->data.lights[i].direction, -1, 1);
            ImGui::SliderFloat("Outer", &lightBuffer->data.lights[i].outer, 0, 180);
            // 90 ������ ����
            ImGui::SliderFloat("Inner", &lightBuffer->data.lights[i].inner, 0, 89);
            ImGui::SliderFloat("Length", &lightBuffer->data.lights[i].length, 0, 100);
            ImGui::Checkbox("Active", (bool*)&lightBuffer->data.lights[i].active);

            ImGui::EndMenu();
        }
    }
}
