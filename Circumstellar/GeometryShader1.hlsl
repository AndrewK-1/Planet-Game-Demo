struct GSIn
{
    float4 position : SV_Position;
    float4 color : COLOR;
};

struct GSOut
{
    float4 position : SV_Position;
    float4 color : COLOR;
};

[maxvertexcount(6)]
void main(triangle GSIn input[3], inout LineStream<GSOut> OutputStream)
{
    GSOut output = (GSOut) 0;

    for (int i = 0; i < 3; i++)
    {
        output.position = input[i].position;
        output.color = input[i].color;
        
        OutputStream.Append(output);
    }
    
    OutputStream.RestartStrip();
}