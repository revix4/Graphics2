#include "NormalMapMaterial.h"

NormalMapMaterial::NormalMapMaterial():
BaseMaterial()
{
	m_NormalMap = NULL;
}

NormalMapMaterial::~NormalMapMaterial()
{
	ReleaseCOM(m_NormalMap);
	BaseMaterial::~BaseMaterial();
}

void NormalMapMaterial::buildFX()
{
	m_ViewProjectionMatHandle = m_Effect->GetParameterByName(0, "gWVP");
	m_LightPosWHandle = m_Effect->GetParameterByName(0, "dirW");
	m_ViewerPosWHandle = m_Effect->GetParameterByName(0, "gEyePosW");
	m_DiffuseColHandle = m_Effect->GetParameterByName(0, "Diffuse");
	m_SpecularColHandle = m_Effect->GetParameterByName(0, "Spec");
	m_AmbientColHandle = m_Effect->GetParameterByName(0, "Ambient");
	m_ShininessHandle = m_Effect->GetParameterByName(0, "SpecPower");

	m_materialHandle = m_Effect->GetParameterByName(0, "gTex");

	m_WorldMatInverseHandle = m_Effect->GetParameterByName(0, "gWorldInv");
	m_NormalMapHandle = m_Effect->GetParameterByName(0, "gNormalMap");

	HR(m_Effect->SetValue(m_DiffuseColHandle, m_DiffuseColor, sizeof(float) * 4));
	HR(m_Effect->SetValue(m_SpecularColHandle, m_SpecularColor, sizeof(float) * 4));
	HR(m_Effect->SetValue(m_AmbientColHandle, m_AmbientColor, sizeof(float) * 4));
	HR(m_Effect->SetFloat(m_ShininessHandle, m_Shininess));
	HR(m_Effect->SetTexture(m_materialHandle, m_texture));

	HR(m_Effect->SetTexture(m_NormalMapHandle, m_NormalMap));
}

void NormalMapMaterial::setNormalMap(IDirect3DDevice9* gd3dDevice, LPCSTR filename)
{
	HR(D3DXCreateTextureFromFile(gd3dDevice, filename, &m_NormalMap));
}