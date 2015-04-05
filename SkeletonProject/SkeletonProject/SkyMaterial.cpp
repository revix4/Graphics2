#include "SkyMaterial.h"

SkyMaterial::SkyMaterial()
	:BaseMaterial()
{

}

SkyMaterial::~SkyMaterial()
{
	ReleaseCOM(mEnvMap);
	BaseMaterial::~BaseMaterial();
}

void SkyMaterial::setTexture(IDirect3DDevice9* gd3dDevice, LPCSTR filename)
{
	HR(D3DXCreateCubeTextureFromFile(gd3dDevice, filename, &mEnvMap));
}

void SkyMaterial::buildFX()
{
	m_envMapHandle = m_Effect->GetParameterByName(0, "gEnvMap");

	HR(m_Effect->SetTexture(m_envMapHandle, mEnvMap));
}