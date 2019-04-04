struct VS_IN
{
	float3 Pos : SV_POSITION;
	float3 Color : COLOR;
	
};

struct VS_OUT
{
	float4 Pos : SV_POSITION;
	float3 Color : COLOR;

};

cbuffer CB_PER_FRAME : register(b0)
{
	float4x4 worldMat;
	float4x4 WorldViewProj;
	//view 
	//projection
	float3 camPos;
	float padding;
}

//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
VS_OUT VS_main(VS_IN input)
{
	VS_OUT output;


	output.Pos = mul(input.Pos, WorldViewProj);

	output.Color = input.Color;
	return output;
}