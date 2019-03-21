struct PointLight
{
	float4x4 world;
	float4 lightPos;//.w=radius
	float4 lightColor;//.w=intensity
	//float radius;
	//float lightIntensity;
};
cbuffer LightProps : register(b0)
{
	float4x4 worldLight;
	float4 lightPos;//.w=radius
	float4 lightColor;//.w=intensity
	float4 LcamPos;
}

cbuffer CB_PER_FRAME : register(b1)
{
	float4x4 world;
	float4x4 view;
	float4x4 proj;
	float3 camPos;
	float padding;
};
Texture2D NormalTex : register(t0);
Texture2D Tex : register(t1);
Texture2D PositionTexture : register(t2);
Texture2D BumpNormalTex : register(t3);
SamplerState SampSt :register(s0);
struct PS_IN
{
	float4 screenPos : SV_POSITION;
	float2 TexCoord : TEXCOORD;
};

//void GetGBufferAttributes(in float2 texCoord, out float3 normal,
//	out float3 position, out float3 diffuseAlbedo, out float3 specularAlbedo,
//	out float specularPower)
//{
//
//	//int3 sampleIndices = int3(screenPos.xy, 0);
//	//float3 color = Tex.Sample(SampSt, input.Tex).xyz;
//	//normal = NormalTex.Load(sampleIndices).xyz;
//	//position = PositionTexture.Load(sampleIndices).xyz;
//	//diffuseAlbedo = Tex.Load(sampleIndices).xyz;
//
//	normal = NormalTex.Sample(SampSt, texCoord).xyz;
//	position = PositionTexture.Sample(SampSt,texCoord).xyz;
//	diffuseAlbedo = Tex.Sample(SampSt, texCoord).xyz;
//
//	float4 spec = float4(1.0f, 1.0f, 1.0f, 1.0f);
//	specularAlbedo = spec.xyz;
//	specularPower = spec.w;
//}

//float3 CalcLighting(in float3 normal, in float3 position, in float3 diffuseAlbedo,
//	in float3 specularAlbedo, in float3 specularPower)
//{
//	float3 L = 0;
//	float attenuation = 1.0f;
//	//L = light - position;
//
//	float dist = length(L);
//	//attenuation = max(0, 1.0f - (dist / radius));
//
//	L /= dist;
//
//	float nDotL = saturate(dot(normal, L));
//	float3 diffuse = nDotL * lightColor*diffuseAlbedo;
//
//	
//	//Specular here
//
//	/*
//	if ((diffuse*attenuation).z == 0.0f)
//	{
//		return float3(1.0f, 1.0f, 1.0f);
//	}*/
//	
//	return(diffuse);//*attenuation);
//}

float4 PS_main(PS_IN input) : SV_Target0
{
	float3 colors;
	float3 bumpNormal;
	float3 normals;
	//float3 lightDir;
	float lightIntensity;
	//float4 outputColor;
	float3 posCol;
	int3 sampleIndices = int3(input.TexCoord.xy, 0);



	float4 nor = NormalTex.Sample(SampSt, sampleIndices.xy);
	float4 pos = PositionTexture.Sample(SampSt, sampleIndices.xy);
	float4 tex = Tex.Sample(SampSt, sampleIndices.xy);

	bumpNormal = BumpNormalTex.Sample(SampSt, input.TexCoord).xyz;
	colors = Tex.Sample(SampSt, input.TexCoord).xyz;
	posCol = PositionTexture.Sample(SampSt, input.TexCoord).xyz;

	normals = NormalTex.Sample(SampSt, input.TexCoord).xyz *2.0f - 1.0f;// back to [-1...1] 
	if (length(normals) > 0.0f) //normals with that 
	{
		float3 final_colour = float3(0.0f, 0.0f, 0.0f);
		float3 ambient = colors * final_colour;
		//for (int i = 0; i < 2; i++)
		//{


			float3 lightPW = mul(float4(lightPos.xyz, 1.0f), world).xyz;


			float3 vecToLight = lightPW - posCol;
			float d = length(vecToLight); //distance
			vecToLight /= d;
			float howMuchLight = dot(vecToLight, bumpNormal); //bumpNormal , normals
				//cell-shading
				/*float theShade = max(dot(normal, normalize(vecToLight)),0);
					if( theShade < 0.2f)
						theShade=0.f;
					else if( theShade >= 0.2f && theShade < 0.4f)
						theShade=0.2f;
					else if( theShade >= 0.4f && theShade < 0.6f)
						theShade=0.4f;
					else if( theShade >= 0.6f && theShade < 0.8f)
						theShade=0.6f;
					else if( theShade >= 0.8f)
						theShade=0.8f;
					float diffuse = max(theShade,0);*/
			float specularStrength = 0.1;
			float3 viewDir = normalize(camPos.xyz - posCol);
			float3 reflectDir = reflect(-vecToLight, normals); //normals

			float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
			float3 specular = specularStrength * spec * lightColor.xyz; //arr
			lightIntensity = saturate(dot(bumpNormal, -vecToLight));
			float diffuse = max(howMuchLight, 0); //smooth
			float3 diffusefinal = saturate(colors * lightColor.xyz*diffuse * lightPos.w *(1 / d)); //array of for multiple light
			final_colour = float3(ambient + diffusefinal);
			final_colour = saturate(final_colour * (howMuchLight));
		//}

			// UPDATE THIS LINE TO ACCOUNT FOR SATURATION (PIXEL COLOUR CANNOT GO OVER 1.0)
		final_colour = min(final_colour, float3(1.f, 1.f, 1.f));
		return float4(final_colour, 1.0f); //final_colour, normals, bumpNormal posCol, colors
	}
	//We can then perform our directional lighting equation using this sampled information.

	// Invert the light direction for calculations.
	//lightDir = -lightDirection;

	// Calculate the amount of light on this pixel.
	//lightIntensity = saturate(dot(normals.xyz, lightDir));

	// Determine the final amount of diffuse color based on the color of the pixel combined with the light intensity.
	//outputColor = saturate(colors * lightIntensity);

	//return float4(1.0f, 0.1f, 0.0f, 1.0f);
	float4 output;
		//output = float4(light,1.0f);
	output = float4(0.0f,0.0f,0.0f, 1.0f);
		//output = float4(normals, 1.0f);

	return output;
}