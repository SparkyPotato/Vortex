cbuffer ConstantBuffer : register(b0)
{
	float4x4 worldViewProjectionMatrix;
}

struct VSOut
{
	float4 color : COLOR;
	float4 position : SV_POSITION;
};

struct VSIn
{
	float4 position : POSITION;
	float4 color : COLOR;
};

VSOut main(VSIn input)
{
	VSOut output;
	
	output.position = mul(input.position, worldViewProjectionMatrix);
	
	output.color = input.color;
	
	return output;
}