cbuffer WorldCBuff : register(b0)
{
    matrix worldMat;
};


cbuffer ViewProjCBuff : register(b1)
{
    matrix viewProjMat;
};


cbuffer BoneTransforms : register(b2)
{
    matrix boneTransforms[64];
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
    //float4 weightsx : WeightsX;
    //uint4 boneIndicesx : BoneIndicesX;
};


VSOut main(VSIn vsin)
{
    VSOut vsout;
    // matrix * matrix is actually element wise mult not matrix mult
    //vsout.pos = mul(float4(vsin.position, 1.0f), mul(worldMat, viewProjMat));
    vsout.texCoord = vsin.texCoord;
    
    // Reconstruct the 4th weight from the 3 provided
    //float4 weights = float4(vsin.weights, 1.0f - vsin.weights.x - vsin.weights.y - vsin.weights.z);
    
    float4 weights = float4(vsin.weights);
    //weights.w = 1.0f - vsin.weights.x - vsin.weights.y - vsin.weights.z;
    
    float3 position = float3(0.0f, 0.0f, 0.0f);
    float3 normal = float3(0.0f, 0.0f, 0.0f);
    float3 tangent = float3(0.0f, 0.0f, 0.0f);
    
    
    for (int i = 0; i < 4; ++i)
    {
        int bidx = vsin.boneIndices[i];
        
        if (bidx < 64)
        {
            position += weights[i] * mul(float4(vsin.position, 1.0f), boneTransforms[bidx]).xyz;
        
            normal += weights[i] * mul(vsin.normal, (float3x3) boneTransforms[bidx]);
        
            tangent += weights[i] * mul(vsin.tangent.xyz, (float3x3) boneTransforms[bidx]);
        }
    }
    
    //for (int i = 0; i < 4; ++i)
    //{
    //    int bidx = vsin.boneIndices[i];
        
    //    if (bidx < 64)
    //    {
    //        position += vsin.weights[i] * mul(float4(vsin.position, 1.0f), boneTransforms[bidx]).xyz;
        
    //        normal += vsin.weights[i] * mul(vsin.normal, (float3x3) boneTransforms[bidx]);
        
    //        tangent += vsin.weights[i] * mul(vsin.tangent.xyz, (float3x3) boneTransforms[bidx]);
    //    }
    //}
    
    //for (int j = 0; j < 4; ++j)
    //{
    //    int bidx = vsin.boneIndicesx[j];
        
    //    if (bidx < 64)
    //    {
    //        position += vsin.weightsx[j] * mul(float4(vsin.position, 1.0f), boneTransforms[bidx]).xyz;
        
    //        normal += vsin.weightsx[j] * mul(vsin.normal, (float3x3) boneTransforms[bidx]);
        
    //        tangent += vsin.weightsx[j] * mul(vsin.tangent.xyz, (float3x3) boneTransforms[bidx]);
    //    }
    //}
    
    // Now the position in root space can be transformed by world and view/projection matrices
    vsout.pos = mul(float4(position, 1.0f), mul(worldMat, viewProjMat));
    
    // need world inverse transpose but not using normals and tangents yet anyway
    
    
    return vsout;
}