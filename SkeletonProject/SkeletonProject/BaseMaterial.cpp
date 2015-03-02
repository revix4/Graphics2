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
	m_texture = NULL;

	m_Shininess = 0.7f;
	m_DiffuseColor = D3DXVECTOR3(0.5f, 0.5f, 1.0f);
	m_SpecularColor = D3DXVECTOR3(0.5f, 0.5f, 1.0f);


}

//-----------------------------------------------------------------------------
// Relase shader, blah...
BaseMaterial::~BaseMaterial(void)
{
	ReleaseCOM(m_Effect);
	ReleaseCOM(m_texture);
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
	m_WorldMatHandle = m_Effect->GetParameterByName(0, "matWorld");
	m_ViewProjectionMatHandle = m_Effect->GetParameterByName(0, "matViewProjection");
	m_LightPosWHandle = m_Effect->GetParameterByName(0, "lightPos");
	m_ViewerPosWHandle = m_Effect->GetParameterByName(0, "viewerPos");
	m_DiffuseColHandle = m_Effect->GetParameterByName(0, "diffuseCol");
	m_SpecularColHandle = m_Effect->GetParameterByName(0, "specularCol");
	m_ShininessHandle = m_Effect->GetParameterByName(0, "shine");

	m_materialHandle = m_Effect->GetParameterByName(0, "base_Tex");

	HR(m_Effect->SetMatrix(m_WorldMatHandle, &m_WorldMat));
	HR(m_Effect->SetMatrix(m_ViewProjectionMatHandle, &m_ViewProjectionMat));

	HR(m_Effect->SetValue(m_DiffuseColHandle, m_DiffuseColor, sizeof(float) * 4));
	HR(m_Effect->SetValue(m_SpecularColHandle, m_SpecularColor, sizeof(float) * 4));
	HR(m_Effect->SetFloat(m_ShininessHandle, m_Shininess));
	HR(m_Effect->SetTexture(m_materialHandle, m_texture));
}

void BaseMaterial::Update(D3DXVECTOR3 lightPos, D3DXVECTOR3 viewPos)
{
	HR(m_Effect->SetValue(m_LightPosWHandle, lightPos, sizeof(float) * 3));
	HR(m_Effect->SetValue(m_ViewerPosWHandle, viewPos, sizeof(float) * 3));
}

//=============================================================================
