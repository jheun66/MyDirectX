#include "PixelHeader.hlsli"

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
    uint isActive : Active;
    float lifeTime : LifeTime;
};


float4 PS(PixelInput input) : SV_Target
{
    float4 result = diffuseMap.Sample(samp, input.uv) * mDiffuse;
    
    if(input.isActive == 0)
        result = float4(0, 0, 0, 0);
    
    return result;
}