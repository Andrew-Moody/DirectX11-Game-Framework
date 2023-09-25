Texture2D diffuseMap : Texture : register(t0);

SamplerState samplerState: Sampler : register(s0);


float4 main(float4 pos : SV_Position, float2 texCoord : TexCoord) : SV_TARGET
{
    float4 col;
    
    col = diffuseMap.Sample(samplerState, texCoord);
    
    //col = float4(1.0f, 0.0f, 0.0f, 0.0f);
    
    //col = float4(texCoord.r, texCoord.g, 0.0f, 0.0f);
    
    return col;
}