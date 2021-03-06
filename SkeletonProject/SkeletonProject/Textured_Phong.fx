//**************************************************************//
//  Effect File exported by RenderMonkey 1.6
//
//  - Although many improvements were made to RenderMonkey FX  
//    file export, there are still situations that may cause   
//    compilation problems once the file is exported, such as  
//    occasional naming conflicts for methods, since FX format 
//    does not support any notions of name spaces. You need to 
//    try to create workspaces in such a way as to minimize    
//    potential naming conflicts on export.                    
//    
//  - Note that to minimize resulting name collisions in the FX 
//    file, RenderMonkey will mangle names for passes, shaders  
//    and function names as necessary to reduce name conflicts. 
//**************************************************************//

//--------------------------------------------------------------//
// Textured Phong
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Pass 0
//--------------------------------------------------------------//
//string Textured_Phong_Pass_0_Model : ModelData = "..\\..\\..\\..\\..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Models\\Sphere.3ds";

float3 lightPos;

float3 camPosition;

float4x4 matView : View;
float4x4 matViewProjection : ViewProjection;

uniform extern bool spec_On;
uniform extern bool diff_On;
uniform extern bool tex_On;

struct VS_INPUT 
{
   float4 Position : POSITION0;
   float2 Texcoord : TEXCOORD0;
   float3 Normal :   NORMAL0;
   
};

struct VS_OUTPUT 
{
   float4 Position :        POSITION0;
   float2 Texcoord :        TEXCOORD0;
   float3 ViewDirection :   TEXCOORD1;
   float3 LightDirection :  TEXCOORD2;
   float3 Normal :          TEXCOORD3;
   
};

VS_OUTPUT VS_0( VS_INPUT Input )
{
   VS_OUTPUT Output;
   
   //get position in world space
   Output.Position         = mul( Input.Position, matViewProjection );
   Output.Texcoord         = Input.Texcoord;

   //object position in view space
   float3 ObjectPosition = mul( Input.Position, matView );
   
   //cam to obj vector
   Output.ViewDirection    = camPosition - ObjectPosition;
   //light to object vector
   Output.LightDirection   = lightPos - ObjectPosition;
   Output.Normal           = mul( Input.Normal, matView );
      
   return( Output );
   
}



float4 Ambient;

float4 Specular;

float4 Diffuse;

float SpecularPower;

texture base_Tex;

sampler2D baseMap = sampler_state
{
   Texture = (base_Tex);
   ADDRESSU = WRAP;
   ADDRESSV = WRAP;
   MINFILTER = LINEAR;
   MAGFILTER = LINEAR;
   MIPFILTER = LINEAR;
};

struct PS_INPUT 
{
   float2 Texcoord :        TEXCOORD0;
   float3 ViewDirection :   TEXCOORD1;
   float3 LightDirection:   TEXCOORD2;
   float3 Normal :          TEXCOORD3;
   
};

float4 PS_0( PS_INPUT Input ) : COLOR0
{      
   //normalize the 3 vectors
   float3 LightDir = normalize( Input.LightDirection );
   float3 norm     = normalize( Input.Normal );
   float3 ViewDir  = normalize( Input.ViewDirection );

   //get the dot of Normal and Light Pos
   float  NL           = saturate(dot( norm, LightDir )); 
   //the reflection vector for the material
   float3 Reflection     = normalize( ( ( 2.0f * NL ) * ( norm ) ) - LightDir ); 
   //dot of the reflection and view
   float  RV           = max( 0.0f, dot( Reflection, ViewDir ) );

   //texture color
   float4 BaseColor = float4(0.5f,0.5f,0.5f,1.0f);

   if(tex_On)
   {
   	BaseColor      = tex2D( baseMap, Input.Texcoord );
   }
   
   //total the 3 parts
   float4 TotalAmbient   = Ambient * BaseColor; 
   float4 TotalDiffuse   = float4(0.0f,0.0f,0.0f,1.0f); 
   float4 TotalSpecular  = float4(0.0f,0.0f,0.0f,1.0f);

   if(spec_On)
   {
   	TotalSpecular  = Specular * pow( (RV), SpecularPower );
   }

   if(diff_On)
   {
   	TotalDiffuse   = Diffuse * NL * BaseColor; 
   }
   
   return(TotalAmbient + TotalDiffuse + TotalSpecular );
      
}



//--------------------------------------------------------------//
// Technique Section for Textured Phong
//--------------------------------------------------------------//
technique Textured_Phong
{
   pass Pass_0
   {
      VertexShader = compile vs_2_0 VS_0();
      PixelShader = compile ps_2_0 PS_0();
   }

}

