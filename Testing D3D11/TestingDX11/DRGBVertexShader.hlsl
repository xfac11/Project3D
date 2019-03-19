cbuffer CB_PER_FRAME : register(b0)
{
	float4x4 world;//world
	float4x4 view;//view
	float4x4 proj;//proj
	float3 camPos;
	float padding;
};

struct VS_IN
{
	float3 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
	float3 Normal : NORMALPOS;
	float3 ThePoint : THEPOINT;
	float3 Tangent : TANGENT; //Normal maps
	float3 Binormal :BINORMAL; //Normal maps
};

struct VS_OUT
{
	float4 PosCS : SV_POSITION;
	float2 Tex : TEXCOORD;
	float3 NormalWS : NORMALWS;
	float4 PositionWS : POSITIONWS;
	float3 Tangent : TANGENT; //Normal maps
	float3 Binormal :BINORMAL; //Normal maps
};


//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
VS_OUT VS_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	//// Change the position vector to be 4 units for proper matrix calculations.
	//input.Pos.w = 1.0f;

	//// Calculate the position of the vertex against the world, view, and projection matrices.
	//output.PosCS = mul(input.Pos, world);
	//output.PosCS = mul(output.PosCS, view);
	//output.PosCS = mul(output.PosCS, proj);

	//// Store the texture coordinates for the pixel shader.
	//output.Tex = input.Tex;

	//// Calculate the normal vector against the world matrix only.
	output.NormalWS = normalize(mul(float4((input.Normal), 0.0f), world)).xyz;//=

	//// Normalize the normal vector.
	//output.NormalWS = normalize(input.Normal);

	//return output;

	//Book:
	//output.PositionWS = mul(input.Pos, world).xyz;
	//output.NormalWS = mul((float3x3)world,output.NormalWS);
	//output.NormalWS = mul(input.Normal, (float3x3)world);
	//output.NormalWS = mul(output.NormalWS, (float3x3)view);
	//output.PosCS = mul(input.Pos, world);
	//output.PosCS = mul(output.PosCS, view);
	//output.PosCS = mul(world, float4(input.Pos, 1.0f));
	output.PosCS = mul(proj, float4(input.Pos, 1.0f));
	//output.PosCS = float4(input.Pos, 1.0f);
	output.PositionWS = mul(float4(input.Pos, 1.0f), world);
	//output.PositionWS = input.Pos.xyz;
	output.Tex = input.Tex;
	output.Tangent = input.Tangent;
	output.Binormal = input.Binormal;
	//Color shader vertex shader
	//output.PosCS = mul(proj, input.Pos);
	////output.PosCS = input.Pos;
	//output.Tex = input.Tex;
	//output.NormalWS = mul((float3x3)world, input.Normal);
	//output.PositionWS = mul(world, input.Pos).xyz;


	return output;
}