//=============================================================================
// NormalMap.fx by Frank Luna (C) 2004 All Rights Reserved.
// Edited by James Potvin
// Does per-pixel lighting where the normals are obtained from a normal map.
//=============================================================================

uniform extern float4x4 gWorldInv;
uniform extern float4x4 gWVP;

uniform extern float3   gEyePosW;
uniform extern texture  gTex;
uniform extern texture  gNormalMap;

uniform extern float4 Ambient;
uniform extern float4 Diffuse;
uniform extern float4 Spec;
uniform extern float  SpecPower;

uniform extern float3 dirW;//light position

uniform extern bool tex_On;
uniform extern bool mapping_On;
uniform extern float normalStrength = 0.7f;

sampler TexS = sampler_state
{
	Texture = <gTex>;
	MinFilter = ANISOTROPIC;
	MaxAnisotropy = 8;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU  = WRAP;
    AddressV  = WRAP;
};

sampler NormalMapS = sampler_state
{
	Texture = <gNormalMap>;
	MinFilter = ANISOTROPIC;
	MaxAnisotropy = 8;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU  = WRAP;
    AddressV  = WRAP;
};
 
struct OutputVS
{
    float4 posH      : POSITION0;
    float3 toEyeT    : TEXCOORD0;
    float3 lightDirT : TEXCOORD1;
    float2 tex0      : TEXCOORD2;
	float3 normal    : TEXCOORD3;
};

OutputVS NormalMapVS(float3 posL      : POSITION0, 
                     float3 tangentL  : TANGENT0,
                     float3 binormalL : BINORMAL0,
                     float3 normalL   : NORMAL0, 
                     float2 tex0      : TEXCOORD0)
{
    // Zero out our output.
	OutputVS outVS = (OutputVS)0;
	
	// Build TBN-basis.
	float3x3 TBN;
	TBN[0] = normalize(tangentL);
	TBN[1] = normalize(binormalL);
	TBN[2] = normalize(normalL);
	
	// Matrix transforms from object space to tangent space.
	float3x3 toTangentSpace = transpose(TBN);
	
	// Transform eye position to local space.
	float3 eyePosL = mul(float4(gEyePosW, 1.0f), gWorldInv).xyz;
	
	// Transform to-eye vector to tangent space.
	float3 toEyeL = eyePosL - posL;
	outVS.toEyeT = mul(toEyeL, toTangentSpace);
	
	// Transform light direction to tangent space.
	float3 lightDirL = mul(float4(-dirW, 0.0f), gWorldInv).xyz;
	outVS.lightDirT  = mul(lightDirL, toTangentSpace);
	
	// Transform to homogeneous clip space.
	outVS.posH = mul(float4(posL, 1.0f), gWVP);
	
	// Pass on texture coordinates to be interpolated in rasterization.
	outVS.tex0 = tex0;

	outVS.normal = normalize(normalL);
	
	// Done--return the output.
    return outVS;
}

float4 NormalMapPS(float3 toEyeT    : TEXCOORD0,
                   float3 lightDirT : TEXCOORD1,
                   float2 tex0      : TEXCOORD2,
				   float3 normal    : TEXCOORD3) : COLOR
{
	// Interpolated normals can become unnormal--so normalize.
	toEyeT    = normalize(toEyeT);
	lightDirT = normalize(lightDirT);
	
	// Light vector is opposite the direction of the light.
	float3 lightVecT = -lightDirT;
	
	// Sample normal map.
	float3 normalT = tex2D(NormalMapS, tex0)*normalStrength;
	
	// Expand from [0, 1] compressed interval to true [-1, 1] interval.
    normalT = 2.0f*normalT - 1.0f;
    
    // Make it a unit vector.
	normalT = normalize(normalT);

	if (!mapping_On)
	{
		normalT = normal;
	}

	//Phong shading stuff dot of normal and light pos
	float NL = saturate(dot(normalT, lightDirT));
	
	// Compute the reflection vector.
	float3 r = normalize(((2.0f*NL)*normalT)-lightDirT);

	//dot of reflection and view
	float RV = max(0.0f, dot(r, toEyeT));
	
	// Determine how much (if any) specular light makes it into the eye.
	float toteSpec  = pow(RV, SpecPower);
	
	// Determine the diffuse light intensity that strikes the vertex.
	float s = max(dot(lightVecT, normalT), 0.0f);
	
	// If the diffuse light intensity is low, kill the specular lighting term.
	// It doesn't look right to add specular light when the surface receives 
	// little diffuse light.

	
	// Get the texture color.
	float4 texColor = float4(0.7f, 0.7f, 0.7f, 1.0f);

	if (tex_On)
	{
		texColor = tex2D(TexS, tex0);
	}

	// Compute the ambient, diffuse and specular terms separatly. 
	float3 spec = toteSpec*(Spec);
	float3 diffuse = s*(Diffuse)*texColor;
	float3 ambient = Ambient*texColor;
	
	// Combine the color from lighting with the texture color.
	float3 color = ambient + diffuse + spec;
	
	// Output the color and the alpha.
    return float4(color, Diffuse.a*texColor.a);
}



technique NormalMapTech
{
    pass P0
    {
        // Specify the vertex and pixel shader associated with this pass.
        vertexShader = compile vs_2_0 NormalMapVS();
        pixelShader  = compile ps_2_0 NormalMapPS();
    }
}