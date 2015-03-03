//Gouraud shader by Paul Webster-Pact

float4x4 matView : View;
float4x4 matViewProjection : ViewProjection;

float3 lightPos;
float4 Ambient;
float4 Diffuse;
float4 Specular;

float SpecularPower;
texture base_Tex;
float3 camPosition;

uniform extern bool spec_On;
uniform extern bool diff_On;
uniform extern bool tex_On;

struct VS_INPUT 
{
    float2 Texcoord : TEXCOORD0;
	float4 position : POSITION0;
	float3 normal	: NORMAL0;
};

struct VS_OUTPUT 
{
	float2 Texcoord : TEXCOORD0;
	float4 position : POSITION0;
	float4 finColor	: COLOR0;
};

struct PS_INPUT 
{
   float2 Texcoord : TEXCOORD0;
   float4 finColor	: COLOR0;
};

sampler2D baseMap = sampler_state
{
   Texture = (base_Tex);
   ADDRESSU = WRAP;
   ADDRESSV = WRAP;
   MINFILTER = LINEAR;
   MAGFILTER = LINEAR;
   MIPFILTER = LINEAR;
};

//all of the calculation are done in the vector shader because this is Gouraud
VS_OUTPUT VS_0( VS_INPUT Input )
{
	VS_OUTPUT	Output;
 
	Output.position = mul(Input.position, matViewProjection);
	//find the position of the vertex in the world
	float3 worldPos = mul(Input.position, matView);
	
	//calculate the light direction using the vertex's position and the light's position
	float3 lightDir	= normalize( lightPos - worldPos );
	
	//this calculates the view direction. this will be used for specular lighting
	float3 viewDir	= normalize( camPosition - worldPos );
 
	//get the dot of the normal of the vertex
	float 	diff 	= saturate(dot ( lightDir, Input.normal ));
 
	//this is the reflection vector
	//it is the direction the light is travelling once it bounces off the vertex
	float3 	reflec 	=  (Input.normal * 2 * dot ( lightDir, Input.normal )) - lightDir;
	
	//this is the dot product of the view and reflection vectors.
	//this is the primary part of makes Gouraud shading look the way it does
	//we are computing the specular based on the average between the vertex reflection
		//and the view instead of doing it per-pixel like in phong
	float	dotViewRef	= dot(reflec, viewDir);
	
	//increase the specular component by our specularPowwer
	float 	spec 	= saturate(dotViewRef) * SpecularPower;
	
	//pass on the texture coordinate so it can be used in the pixel shader
	Output.Texcoord         = Input.Texcoord;
	
	//this is the final color, but missing the texture and ambient components
	//those will be added in the pixel shader
	Output.finColor = diff  * Diffuse + spec*Specular;
	return Output;
}

float4 PS_0( PS_INPUT Input ) : COLOR0
{      
	float4 BaseColor = float4(0.5f,0.5f,0.5f,1.0f);
	
	//get the texture data for the specific pixel
	//this can't be done in the vertex shader, so it is done here
	if(tex_On)
	{
		BaseColor = tex2D( baseMap, Input.Texcoord );
	}
	//return the final color mixed with the base and ambient components
	return (Input.finColor + BaseColor * Ambient);
}

technique Textured_Gouraud
{
   pass Pass_0
   {
      VertexShader = compile vs_2_0 VS_0();
      PixelShader = compile ps_2_0 PS_0();
   }

}

