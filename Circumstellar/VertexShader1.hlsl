cbuffer constMatrixData : register(b0)
{
    float4x4 worldMatrix;
    float4x4 viewMatrix;
    float4x4 projectionMatrix;
};

struct VSIn
{
    float4 position : POSITION;
    float4 color : COLOR;
};

struct VSOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VSOut main(VSIn input)
{
    VSOut output;
    output.position = mul(float4(input.position), worldMatrix);
    output.position = mul(float4(output.position), viewMatrix);
    output.position = mul(float4(output.position), projectionMatrix);
    output.color = input.color;
    return output;
}