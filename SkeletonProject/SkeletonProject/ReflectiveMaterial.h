#pragma once

#include "BaseMaterial.h"

class ReflectiveMaterial : public BaseMaterial
{
protected:

	D3DXVECTOR4         m_LightDiffuseColor;
	D3DXVECTOR4         m_LightSpecularColor;
	D3DXVECTOR4         m_LightAmbientColor;
	//float               m_LightShininess; //specular power

	D3DXHANDLE          m_LightDiffuseColHandle;
	D3DXHANDLE          m_LightSpecularColHandle;
	D3DXHANDLE          m_LightAmbientColHandle;
	//D3DXHANDLE          m_LightDirWHandle;

	IDirect3DTexture9*  m_envMap;
	D3DXHANDLE		    m_envMapHandle;

public:
	ReflectiveMaterial();
	void buildFX();
	void setEnvMap(IDirect3DDevice9* gd3dDevice, LPCSTR filename);
	void Update(D3DXVECTOR3 lightPos, D3DXVECTOR3 viewPos);
};