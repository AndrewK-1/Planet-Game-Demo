struct PSIn
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

float4 main(PSIn input) : SV_Target
{
    float4 output;
    output.x = input.position.x / 1920.0f; 
    output.y = input.position.y / 1080.0f;
    output.z = 1.0f;
    output.w = 1.0f;
    return float4(output);
}