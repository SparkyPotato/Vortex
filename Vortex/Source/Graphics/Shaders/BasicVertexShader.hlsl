cbuffer ConstantBuffer : register(b0)
{
    float4x4 worldViewProjectionMatrix;
}

struct VSOut
{
	float4 color : COLOR;
    float4 normal : NORMAL;
	float4 position : SV_POSITION;
};

VSOut main(float4 pos : POSITION, float4 norm : NORMAL, float4 col : COLOR)
{
	VSOut output;
	
    output.position = mul(pos, worldViewProjectionMatrix);
	
	output.color = col;
    output.normal = norm;
	
	return output;
}