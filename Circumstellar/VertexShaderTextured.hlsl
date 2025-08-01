cbuffer constMatrixData : register(b0)
{
    float4x4 worldMatrix;
    float4x4 viewMatrix;
    float4x4 projectionMatrix;
};

struct VSIn
{
    float4 position : POSITION;
    float4 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

struct VSOut
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

VSOut main(VSIn input)
{
    VSOut output;
    output.position = mul(float4(input.position), worldMatrix);
    output.position = mul(float4(output.position), viewMatrix);
    output.position = mul(float4(output.position), projectionMatrix);
    output.texcoord = input.texcoord;
    return output;
}