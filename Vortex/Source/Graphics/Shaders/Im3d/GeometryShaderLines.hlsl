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
void main(line GSIn input[2], inout TriangleStream<GSOut> output)
{
	float2 viewport = viewportDimensions.xy;
	
	float2 pos0 = input[0].position.xy / input[0].position.w;
	float2 pos1 = input[1].position.xy / input[1].position.w;
		
	float2 dir = pos0 - pos1;
	dir = normalize(float2(dir.x, dir.y * viewport.y / viewport.x));
	float2 tang0 = float2(-dir.y, dir.x);
	float2 tang1 = tang0 * input[1].size / viewport;
	tang0 = tang0 * input[0].size / viewport;
		
	GSOut ret;
			
	ret.size = input[0].size;
	ret.color = input[0].color;
	ret.texcoord = float2(0.f, 0.f);
	
    ret.position = float4((pos0 + tang0) * input[0].position.w, input[0].position.zw);
    ret.edgeDistance = input[0].size;
    output.Append(ret);
	ret.position = float4((pos0 - tang0) * input[0].position.w, input[0].position.zw);
	ret.edgeDistance = -input[0].size;
	output.Append(ret);
    ret.size = input[1].size;
    ret.color = input[1].color;
    ret.texcoord = float2(1.f, 1.f);
	
    ret.position = float4((pos1 - tang1) * input[1].position.w, input[1].position.zw);
    ret.edgeDistance = -input[1].size;
    output.Append(ret);
	
    output.RestartStrip();
	
    ret.position = float4((pos1 - tang1) * input[1].position.w, input[1].position.zw);
    ret.edgeDistance = -input[1].size;
    output.Append(ret);
	ret.position = float4((pos1 + tang1) * input[1].position.w, input[1].position.zw);
	ret.edgeDistance = input[1].size;
	output.Append(ret);
    ret.size = input[0].size;
    ret.color = input[0].color;
    ret.texcoord = float2(0.f, 0.f);
	
    ret.position = float4((pos0 + tang0) * input[0].position.w, input[0].position.zw);
    ret.edgeDistance = input[0].size;
    output.Append(ret);
}
