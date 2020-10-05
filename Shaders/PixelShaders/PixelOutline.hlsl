#include "PixelHeader.hlsli"

cbuffer ImageSize : register(b11)
{
    float2 imageSize;
}

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
};

static const float2 edges[8] =
{
    float2(-1, -1), float2(0, -1), float2(1, -1),
    float2(-1, -0), float2(1, 0),
    float2(-1, 1), float2(0, 1), float2(1, 1),
};

float4 Outline(float2 uv)
{
    float4 result = 0;
    float2 pixelSize = 1 / imageSize;
        
    if (diffuseMap.Sample(samp, uv).a == 0)
    {
        for (int i = 0; i < 8; i++)
        {
            float2 temp = edges[i] * pixelSize + uv;
            if (diffuseMap.Sample(samp, temp).a != 0)
            {
                result = float4(1, 1, 1, 1);
                break;
            }
        }
    }
    
    return result;
}


float4 PS(PixelInput input) : SV_Target
{
    return Outline(input.uv);
}