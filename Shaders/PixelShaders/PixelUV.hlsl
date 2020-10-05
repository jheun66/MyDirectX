#include "PixelHeader.hlsli"

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
};


float4 PS(PixelInput input) : SV_Target
{
    // 거슬리면 주석하기
    if (input.uv.x < 0.001f || input.uv.x > 0.999f || input.uv.y < 0.001f || input.uv.y > 0.999f )
        return float4(1, 1, 1, 1);
    return diffuseMap.Sample(samp, input.uv);
}