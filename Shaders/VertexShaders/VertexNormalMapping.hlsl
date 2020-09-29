#include "VertexHeader.hlsli"

struct VertexInput
{
	//          ½Ã¸àÆ½ ³×ÀÓ
    float4 pos : Position;
    float2 uv : UV;
    float3 normal : NORMAL;
    // ¹ý¼± º¤ÅÍ
    float3 tangent : TANGENT;
};

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 viewDir : VIEWDIR;
    
    // lighting Ãß°¡
    float3 worldPos : Position0;
    float3 camPos : Position1;
};


PixelInput VS(VertexInput input)
{
    PixelInput output;
    
    output.pos = mul(input.pos, world);
    
    // lighting ¼öÁ¤
    output.worldPos = output.pos.xyz;
    output.camPos = invView._41_42_43;
    
    output.viewDir = normalize(output.pos.xyz - output.camPos);
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);

    output.uv = input.uv;

    output.normal = mul(input.normal, (float3x3) world);
    output.tangent = mul(input.tangent, (float3x3) world);
    output.binormal = cross(output.normal, output.tangent);
    
    return output;
}