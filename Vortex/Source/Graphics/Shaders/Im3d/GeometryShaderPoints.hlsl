cbuffer ConstantBuffer : register(b0)
{
	float4 viewportDimensions;
};

struct GSOut
{
	linear float4 position : SV_POSITION;
	linear float4 color : COLOR;
	linear float2 texcoord : TEXCOORD;
	noperspective float size : SIZE;
	noperspective float edgeDistance : EDGE_DISTANCE;
};

struct GSIn
{
	linear float4 position : SV_POSITION;
	linear float4 color : COLOR;
	linear float2 texcoord : TEXCOORD;
	noperspective float size : SIZE;
	noperspective float edgeDistance : EDGE_DISTANCE;
};

[maxvertexcount(6)]
void main(point GSIn input[1], inout TriangleStream<GSOut> output)
{
	float2 viewport = viewportDimensions.xy;
	GSOut ret;
			
	float2 scale = 1.f / viewport * input[0].size;
	ret.size = input[0].size;
	ret.color = input[0].color;
	ret.edgeDistance = 0.f;
			
	ret.position = float4(input[0].position.xy + float2(-1.f, -1.f) * scale * input[0].position.w, input[0].position.zw);
	ret.texcoord = float2(0.f, 0.f);
	output.Append(ret);
			
	ret.position = float4(input[0].position.xy + float2(-1.f, 1.f) * scale * input[0].position.w, input[0].position.zw);
	ret.texcoord = float2(0.f, 1.f);
	output.Append(ret);
			
	ret.position = float4(input[0].position.xy + float2(1.f, 1.f) * scale * input[0].position.w, input[0].position.zw);
	ret.texcoord = float2(1.f, 1.f);
	output.Append(ret);
	
    output.RestartStrip();
	
    ret.position = float4(input[0].position.xy + float2(1.f, 1.f) * scale * input[0].position.w, input[0].position.zw);
    ret.texcoord = float2(1.f, 1.f);
    output.Append(ret);
	
    ret.position = float4(input[0].position.xy + float2(1.f, -1.f) * scale * input[0].position.w, input[0].position.zw);
    ret.texcoord = float2(1.f, 0.f);
    output.Append(ret);
	
    ret.position = float4(input[0].position.xy + float2(-1.f, -1.f) * scale * input[0].position.w, input[0].position.zw);
    ret.texcoord = float2(0.f, 0.f);
    output.Append(ret);
}
