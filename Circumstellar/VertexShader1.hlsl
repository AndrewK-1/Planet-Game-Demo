struct VSIn
{
    float4 position : POSITION4;
    float4 color : COLOR4;
};

struct VSOut
{
    float4 position : VS_Position;
    float4 color : COLOR4;
};

VSOut main(VSIn input)
{
    VSOut output;
    output.position = input.position;
    output.color = input.color;
    return output;
}