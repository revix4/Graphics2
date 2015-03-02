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
// Textured
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Pass 0
//--------------------------------------------------------------//
//string Effect_Group_1_Textured_Pass_0_Model : ModelData = "..\\..\\..\\..\\..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Models\\Sphere.3ds";

uniform extern float4x4 matWorld;
uniform extern float4x4 matViewProjection : ViewProjection;
uniform extern float3 diffuseCol;
uniform extern float3 specularCol;
uniform extern float shine;

uniform extern float3 lightPos;
uniform extern float3 viewerPos;

uniform extern texture base_Tex;

uniform extern bool spec_On;
uniform extern bool diff_On;
uniform extern bool tex_On;

sampler baseMap = sampler_state
{
   Texture = (base_Tex);
   ADDRESSU = WRAP;
   ADDRESSV = WRAP;
   MINFILTER = LINEAR;
   MAGFILTER = LINEAR;
};

struct VS_INPUT 
{
   float4 Position : POSITION0;
   float2 Texcoord : TEXCOORD0;
};

struct VS_OUTPUT 
{
   float4 Position : POSITION0;
   float2 Texcoord : TEXCOORD0;
   
};

VS_OUTPUT VS_0( VS_INPUT Input )
{
   VS_OUTPUT Output;

   Output.Position = mul( Input.Position, matViewProjection );
   Output.Texcoord = Input.Texcoord;

   return( Output );
   
}

struct PS_INPUT 
{
   float2 Texcoord : TEXCOORD0;
   
};

float4 PS_0( PS_INPUT Input ) : COLOR0
{

   if(tex_On)
   {
      return tex2D( baseMap, Input.Texcoord );
   }

   float4 color = float4(0,0,0,1);
   return color;
}




//--------------------------------------------------------------//
// Technique Section for Effect Workspace.Effect Group 1.Textured
//--------------------------------------------------------------//
technique Textured
{
   pass Pass_0
   {
      VertexShader = compile vs_2_0 VS_0();
      PixelShader = compile ps_2_0 PS_0();
   }

}

