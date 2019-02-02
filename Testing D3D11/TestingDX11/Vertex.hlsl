struct VS_IN
{
	float3 Pos : SV_POSITION;
	//float3 Color : COLOR;
	float2 Tex : TEXCOORD;
	float3 Normal : NORMAL;
	float3 ThePoint : MYVALUE;
};

struct VS_OUT
{
	float4 Pos : SV_POSITION;
	//float3 Color : COLOR;
	float2 Tex : TEXCOORD;
	float3 Normal : NORMAL;
	float3 ThePoint : MYVALUE;
};


//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
VS_OUT VS_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	//output.Pos = mul(worldMat, float4(float3(input.Pos.x + offsetX, input.Pos.yz) , 1));
	output.Pos = float4(input.Pos, 1.0);
	//output.Color = input.Color;
	output.Tex = input.Tex;
	output.Normal = input.Normal;
	output.ThePoint = input.ThePoint;
	return output;
}