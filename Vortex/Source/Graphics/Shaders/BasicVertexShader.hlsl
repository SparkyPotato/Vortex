cbuffer ConstantBuffer : register(b0)
{
	float4x4 worldViewProjectionMatrix;
}

struct VSOut
{
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
	float4 normal : NORMAL;
	float4 position : SV_POSITION;
};

struct VSIn
{
	float4 position : POSITION;
	float4 normal : NORMAL;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
};

VSOut main(VSIn input)
{
	VSOut output;
	
	output.position = mul(input.position, worldViewProjectionMatrix);
	
	output.color = input.color;
	output.normal = input.normal;
	output.texcoord = input.texcoord;
	
	return output;
}