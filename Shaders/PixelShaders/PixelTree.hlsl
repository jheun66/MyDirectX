#include "PixelHeader.hlsli"

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 viewDir : VIEWDIR;
};

//                          ¹ÝÈ¯°ªÀÇ ½Ã¸àÆ½ ³×ÀÓ(SV : system value)
float4 PS(PixelInput input) : SV_Target
{
    float3 light = normalize(lightDir);

    float3 N = normalize(input.normal); // z
    
    float3 normal = N;
    
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