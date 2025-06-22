cbuffer ConstantMatrixBuffer : register(b0)
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
    float4 position : POSITION;
    float4 color : COLOR;
};

VSOut main(VSIn input)
{
    VSOut output;
    output.position = mul(input.position, worldMatrix);
    //output.position = mul(input.position, viewMatrix);
    output.position = mul(input.position, projectionMatrix);
    output.color = input.color;
    return output;
}