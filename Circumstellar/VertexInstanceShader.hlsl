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

struct InstanceIn
{
    float4 row0 : INSTANCE_POSITION0;
    float4 row1 : INSTANCE_POSITION1;
    float4 row2 : INSTANCE_POSITION2;
    float4 row3 : INSTANCE_POSITION3;
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