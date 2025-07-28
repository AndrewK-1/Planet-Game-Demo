cbuffer constMatrixData : register(b0)
{
    float4x4 worldMatrix;
    float4x4 viewMatrix;
    float4x4 projectionMatrix;
    float4 cameraPosition;
};

struct VSIn
{
    float4 position : POSITION;
    float4 normal : NORMAL;
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
    
    float3x3 normalMatrix = (float3x3) transformMat;
    float3 normal = normalize(mul(normalMatrix, input.normal.xyz));
    
    //Calculate diffuse lighting
    float3 DiffuseLightDirection = float3(1, 0, 0);
    float3 DiffuseLightDirection2 = float3(-0.7, 0.8, 0.9);
    float3 DiffuseLightDirection3 = float3(-0.7, -0.8, -0.9);
    float4 DiffuseColor = float4(0, 1, 0.9, 1);
    float4 DiffuseColor2 = float4(0.0, 0.0, 1.0, 1);
    float4 DiffuseColor3 = float4(0.5, 0.0, 1.0, 1);
    float DiffuseIntensity = 1.0;
    float DiffuseIntensity2 = 0.7;
    float DiffuseIntensity3 = 0.3;
    float lightIntensity = saturate(dot(normal, DiffuseLightDirection));
    float lightIntensity2 = saturate(dot(normal, DiffuseLightDirection2));
    float lightIntensity3 = saturate(dot(normal, DiffuseLightDirection3));
    output.color = max(input.color * 0.5,
        saturate(input.color + input.color * (DiffuseColor * DiffuseIntensity * lightIntensity)
        + input.color * (DiffuseColor2 * DiffuseIntensity2 * lightIntensity2)
        + input.color * (DiffuseColor3 * DiffuseIntensity3 * lightIntensity3)));
    
    //Calculate reflective lighting
    float3 reflectiveLightDirection = float3(1, 0, 0);
    float3 cameraPosVec = normalize((cameraPosition.xyz) - mul(float4(input.position), transformMat));
    float3 reflectVec = normalize(reflect(reflectiveLightDirection, normal));
    float reflectValue = saturate(dot(reflectVec, cameraPosVec));
    float reflectIntensity = abs(reflectValue) * reflectValue;
    float4 reflectColor = float4(1, 1, 1, 1);
    output.color = saturate(max(output.color + (reflectIntensity * reflectColor) * (float4(1.0f, 1.0f, 1.0f, 1.0f) - output.color), output.color));
    
    return output;
}