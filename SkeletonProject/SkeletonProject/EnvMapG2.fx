//EnvMap.fx

//by Paul Webster-Pact
//based off of code by Frank Luna

//this code does the normal phong operations (lighting and texturing)
//this code adds on reflections based on an environment map

uniform extern float4x4 gWorld;
uniform extern float4x4 gWorldInvTrans;
uniform extern float4x4 gWVP;

//material components
//this is given individually instead of as a struct
uniform extern float4 gMtrlAmbient;
uniform extern float4 gMtrlDiffuse;
uniform extern float4 gMtrlSpec;
uniform extern float  gMtrlSpecPower;
//light components
//this is given individually instead of as a struct
uniform extern float4 gLightAmbient;
uniform extern float4 gLightDiffuse;
uniform extern float4 gLightSpec;
uniform extern float3 gLightDirW; 
 
uniform extern float3   gEyePosW;
uniform extern texture  gTex;
uniform extern texture  gEnvMap;

uniform extern float gReflectivity;
uniform extern bool gReflectOn;


sampler TexS = sampler_state
{
	Texture = <gTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU  = WRAP;
    AddressV  = WRAP;
};

sampler EnvMapS = sampler_state
{
	Texture = <gEnvMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU  = WRAP;
    AddressV  = WRAP;
};
 
struct VS_OUTPUT
{
    float4 posH 		: POSITION0;
    float3 normalWorld 	: TEXCOORD0;
    float3 eyeWorld 	: TEXCOORD1;
    float2 tex0			: TEXCOORD2;
};

VS_OUTPUT VS(float3 posLocal: POSITION0, float3 normalLocal: NORMAL0, float2 tex0: TEXCOORD0)
{
	VS_OUTPUT output;
	
	//normal to world space
	output.normalWorld = mul(float4(normalLocal, 0.0f), gWorldInvTrans).xyz;
	
	//vertex to world space
	float3 posW  = mul(float4(posLocal, 1.0f), gWorld).xyz;
	
	output.eyeWorld = gEyePosW - posW;
	output.posH = mul(float4(posLocal, 1.0f), gWVP);
	
	output.tex0 = tex0;
	
    return output;
}

float4 PS(float3 normalWorld: TEXCOORD0, float3 eyeWorld: TEXCOORD1, float2 tex0: TEXCOORD2) : COLOR
{
	//normalize normals in case they aren't normal
	normalWorld = normalize(normalWorld);
	eyeWorld  = normalize(eyeWorld);
	
	//calculate lighting and reflections
	float3 lightVecW = -gLightDirW;
	float3 refl = reflect(-lightVecW, normalWorld);
	float specAmount  = pow(max(dot(refl, eyeWorld), 0.0f), gMtrlSpecPower);
	float intensity = max(dot(lightVecW, normalWorld), 0.0f);
	
	float4 texColor = tex2D(TexS, tex0);
	
	//reflected color based on texture and lighting
	float3 envMapTex = reflect(-eyeWorld, normalWorld);
	float3 reflectedColor = texCUBE(EnvMapS, envMapTex);
	
	float3 ambientMtrl = gReflectivity*reflectedColor + (1.0f-gReflectivity)*(gMtrlAmbient*texColor);
	float3 diffuseMtrl = gReflectivity*reflectedColor + (1.0f-gReflectivity)*(gMtrlDiffuse*texColor);
	
	float3 spec = specAmount*(gMtrlSpec*gLightSpec).rgb;
	float3 diffuse = intensity*(diffuseMtrl*gLightDiffuse.rgb);
	float3 ambient = ambientMtrl*gLightAmbient;
	
	float3 final = ambient + diffuse + spec;
		
    return float4(final, gMtrlDiffuse.a*texColor.a);
}

technique EnvMapTech
{
    pass P0
    {
        verTexShader = compile vs_2_0 VS();
        pixelShader  = compile ps_2_0 PS();
    }
}