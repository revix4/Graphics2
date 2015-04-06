#include "ReflectiveMaterial.h"

ReflectiveMaterial::ReflectiveMaterial()
{
	m_DiffuseColor = D3DXVECTOR4(0.65,0.65,0.65,1);
	m_SpecularColor = D3DXVECTOR4(0.15,0.15,0.15,1);
	m_AmbientColor = D3DXVECTOR4(0.2,0.2,0.2,1);
	m_Shininess = 1;

	m_LightDiffuseColor = D3DXVECTOR4(1, 1, 1, 1);
	m_LightSpecularColor = D3DXVECTOR4(1, 1, 1, 1);
	m_LightAmbientColor = D3DXVECTOR4(1, 1, 1, 1);


}

void ReflectiveMaterial::buildFX()
{
	// Obtain handles.
	//mhTech = mFX->GetTechniqueByName("EnvMapTech");
	//m_ViewProjectionMatHandle = m_Effect->GetParameterByName(0, "gWVP");
	//mhWorldInvTrans = m_Effect->GetParameterByName(0, "gWorldInvTrans");

	//... = mFX->GetParameterByName(0, "gMtrl");
	m_AmbientColHandle = m_Effect->GetParameterByName(0, "gMtrlAmbient");
	m_DiffuseColHandle = m_Effect->GetParameterByName(0, "gMtrlDiffuse");
	m_SpecularColHandle = m_Effect->GetParameterByName(0, "gMtrlSpec");
	m_ShininessHandle = m_Effect->GetParameterByName(0, "gMtrlSpecPower");
	//... = mFX->GetParameterByName(0, "gLight");
	m_LightAmbientColHandle = m_Effect->GetParameterByName(0, "gLightAmbient");
	m_LightDiffuseColHandle = m_Effect->GetParameterByName(0, "gLightDiffuse");
	m_LightSpecularColHandle = m_Effect->GetParameterByName(0, "gLightSpec");
	//this light dirW handle might be wrong
	m_LightPosWHandle = m_Effect->GetParameterByName(0, "gLightDirW");

	m_ViewerPosWHandle = m_Effect->GetParameterByName(0, "gEyePosW");
	//m_WorldMatHandle = m_Effect->GetParameterByName(0, "gWorld");
	m_materialHandle = m_Effect->GetParameterByName(0, "gTex");
	m_envMapHandle = m_Effect->GetParameterByName(0, "gEnvMap");

	// Set parameters that do not vary:

	HR(m_Effect->SetValue(m_DiffuseColHandle, m_DiffuseColor, sizeof(float) * 4));
	HR(m_Effect->SetValue(m_SpecularColHandle, m_SpecularColor, sizeof(float) * 4));
	HR(m_Effect->SetValue(m_AmbientColHandle, m_AmbientColor, sizeof(float) * 4));
	HR(m_Effect->SetFloat(m_ShininessHandle, m_Shininess));
	HR(m_Effect->SetTexture(m_materialHandle, m_texture));

	HR(m_Effect->SetValue(m_LightDiffuseColHandle, m_LightDiffuseColor, sizeof(float) * 4));
	HR(m_Effect->SetValue(m_LightSpecularColHandle, m_LightSpecularColor, sizeof(float) * 4));
	HR(m_Effect->SetValue(m_LightAmbientColHandle, m_LightAmbientColor, sizeof(float) * 4));
	HR(m_Effect->SetTexture(m_envMapHandle, m_envMap));

	// World is the identity, so inverse-transpose also identity.
	//HR(mFX->SetMatrix(mhWorldInvTrans, &mSceneWorld));
	//HR(mFX->SetMatrix(mhWorld, &mSceneWorld));

	//HR(mFX->SetTexture(mhEnvMap, mSky->getEnvMap()));
	//HR(mFX->SetTechnique(mhTech));
}

void ReflectiveMaterial::setEnvMap(IDirect3DDevice9* gd3dDevice, LPCSTR filename)
{
	HR(D3DXCreateTextureFromFile(gd3dDevice, filename, &m_envMap));
}

void ReflectiveMaterial::Update(D3DXVECTOR3 lightPos, D3DXVECTOR3 viewPos)
{
	HR(m_Effect->SetValue(m_LightPosWHandle, lightPos, sizeof(float) * 3));
	HR(m_Effect->SetValue(m_ViewerPosWHandle, viewPos, sizeof(float) * 3));
}