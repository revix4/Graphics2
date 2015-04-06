#ifndef NORMALMAPMATERIAL_H
#define NORMALMAPMATERIAL_H

#include "BaseMaterial.h"

class NormalMapMaterial : public BaseMaterial
{
private:
	D3DXMATRIX          m_WorldMatInverse;
	IDirect3DTexture9*  m_NormalMap;

	D3DXHANDLE m_WorldMatInverseHandle;
	D3DXHANDLE m_NormalMapHandle;
public:
	NormalMapMaterial();
	~NormalMapMaterial();

	void buildFX();
	void setNormalMap(IDirect3DDevice9* gd3dDevice, LPCSTR filename);
};

#endif