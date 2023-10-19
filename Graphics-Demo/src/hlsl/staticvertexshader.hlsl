cbuffer WorldCBuff : register(b0)
{
    matrix worldMat;
};


cbuffer ViewProjCBuff : register(b1)
{
    matrix viewProjMat;
};


struct VSOut
{
    float4 pos : SV_Position;
    float2 texCoord : TexCoord;
};


struct VSIn
{
    float3 position : Position;
    float3 normal : Normal;
    float2 texCoord : TexCoord;
    float4 tangent : Tangent;
    float4 weights : Weights;
    uint4 boneIndices : BoneIndices;
};

VSOut main(VSIn vsin)
{
    VSOut vsout;
    // matrix * matrix is actually element wise mult not matrix mult
    vsout.pos = mul(float4(vsin.position, 1.0f), mul(worldMat, viewProjMat));
    vsout.texCoord = vsin.texCoord;
    
    return vsout;
}
