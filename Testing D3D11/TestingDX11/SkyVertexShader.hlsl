cbuffer CB_PER_FRAME : register(b0)
{
	float4x4 world;
	float4x4 view;//view
	float4x4 proj;//proj
};
struct VS_IN
{
	float3 position : POSITION;
};

struct VS_OUT
{
	float4 Pos : SV_POSITION;
	//float3 Color : COLOR;
	float3 Tex : TEXCOORD;
};


//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
VS_OUT VS_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;
	/*float4 pos = mul(mul(float4(input.position, 1.0f), view), proj);

	output.Pos = pos.xyww;
	output.Tex = input.position;*/

	output.Pos = mul(float4(input.position, 1.0f), proj).xyww;//proj is worldviewproj
	output.Pos.z = 1.0f;
	output.Tex = input.position;
	return output;
}