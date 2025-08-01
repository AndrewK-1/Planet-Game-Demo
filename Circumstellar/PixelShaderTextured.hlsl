Texture2D loadedTexture : register(t0);
SamplerState samplerState : register(s0);

struct PSIn
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

float4 main(PSIn input) : SV_Target
{
    float4 output;
    output = loadedTexture.Sample(samplerState, input.texcoord);
    return float4(output);
}