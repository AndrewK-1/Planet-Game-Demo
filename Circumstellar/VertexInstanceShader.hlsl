cbuffer constMatrixData : register(b0)
{
    float4x4 viewMatrix;
    float4x4 projectionMatrix;
};

struct VSIn
{
    float4 position : POSITION;
    float4 color : COLOR;
};

struct InstanceIn
{
    float4 row0 : POSITION;
    float4 row1 : POSITION;
    float4 row2 : POSITION;
    float4 row3 : POSITION;
};

struct VSOut
{
    float4 position : SV_Position;
    float4 color : COLOR;
};

VSOut main(VSIn input, InstanceIn instance)
{
    VSOut output;
    float4x4 transformMat = float4x4(instance.row0, instance.row1, instance.row2, instance.row3);
    output.position = mul(float4(input.position), transformMat);
    output.position = mul(float4(output.position), viewMatrix);
    output.position = mul(float4(output.position), projectionMatrix);
    output.color = input.color;
    return output;
}