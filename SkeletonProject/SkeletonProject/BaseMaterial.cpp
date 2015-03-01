//=============================================================================
//                              BaseMaterial
//
// Writen by Adi Bar-Lev, 2013
// EGP 300-101, Graphics Programming II  - skeleton project
//
// Base class that contains the most intrinsic parameters to implement per model
// lighting / shaders effects.
//=============================================================================
#include "BaseMaterial.h"
//=============================================================================
BaseMaterial::BaseMaterial(void)
{
    m_Effect = NULL;
}

//-----------------------------------------------------------------------------
// Relase shader, blah...
BaseMaterial::~BaseMaterial(void)
{
}

//-----------------------------------------------------------------------------
// Need to add here a code that will associate with your shader parameters and 
// register them.
void BaseMaterial::ConnectToEffect( ID3DXEffect* effect )
{
    m_Effect = effect;
}

//set the texture to the file passed in
void BaseMaterial::setTexture(IDirect3DDevice9* gd3dDevice, LPCSTR filename)
{
	HR(D3DXCreateTextureFromFile(gd3dDevice, filename, &m_texture));
}

//gset the handles in BaseMaterial to the ones in the shader
void BaseMaterial::buildFX()
{
	m_WorldMatHandle = m_Effect->GetParameterByName(0, "g_worldMat");
	m_ViewProjectionMatHandle = m_Effect->GetParameterByName(0, "g_viewProjMat");
	m_LightPosWHandle = m_Effect->GetParameterByName(0, "g_lightPos");
	m_ViewerPosWHandle = m_Effect->GetParameterByName(0, "g_viewerPos");
	m_DiffuseColHandle = m_Effect->GetParameterByName(0, "g_diffuseCol");
	m_SpecularColHandle = m_Effect->GetParameterByName(0, "g_specularCol");
	m_ShininessHandle = m_Effect->GetParameterByName(0, "g_shine");
}

//=============================================================================
