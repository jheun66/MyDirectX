#include "PixelHeader.hlsli"

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 viewDir : VIEWDIR;
};

//                          ¹ÝÈ¯°ªÀÇ ½Ã¸àÆ½ ³×ÀÓ(SV : system value)
float4 PS(PixelInput input) : SV_Target
{
    float3 light = normalize(lightDir);
                                         // polygonÀÇ 
    float3 T = normalize(input.tangent); // x
    float3 B = normalize(input.binormal); // y
    float3 N = normalize(input.normal); // z
    
    float3 normal = N;
    
    float3x3 TBN = float3x3(T, B, N);
    
    normal = normalize(mul(normal, TBN));
    
    
    float diffuse = saturate(dot(normal, -light));
    
    float4 specular = 0;
    if (diffuse > 0)
    {
        float3 viewDir = normalize(input.viewDir);

        float3 halfWay = normalize(input.viewDir + light);
        specular = saturate(dot(-halfWay, normal));
        
        float4 specualrIntensity = 1;
        
        specular = pow(specular, mSpecular.a) * specualrIntensity;
    }
   
    return (diffuse * mDiffuse) + specular * mSpecular;
}