#include "VertexHeader.hlsli"

struct VertexInput
{
    float4 pos : Position;
    float2 size : Size;
    float3 velocity : Velocity;
    uint isActive : Active;
    float lifeTime : LifeTime;
};

struct VertexOutput
{
    //          Rasterizer가 아니라 Geometry에 보내서 SV 제거
    float3 pos : Position;
    float2 size : Size;
    float3 velocity : Velocity;
    uint isActive : Active;
    float lifeTime : LifeTime;
};


VertexOutput VS(VertexInput input)
{
    VertexOutput output;
	
    output.pos = input.pos.xyz;
    output.size = input.size;
    output.velocity = input.velocity;
    output.isActive = input.isActive;
    output.lifeTime = input.lifeTime;
    
    return output;
}