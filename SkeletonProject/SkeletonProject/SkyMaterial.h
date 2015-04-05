#ifndef _SKY_MATERIAL_H
#define _SKY_MATERIAL_H

#include "BaseMaterial.h"

class SkyMaterial : public BaseMaterial
{
private:
	D3DXHANDLE m_envMapHandle;

	IDirect3DCubeTexture9* mEnvMap;

public:
	SkyMaterial();
	~SkyMaterial();

	void setTexture(IDirect3DDevice9* gd3dDevice, LPCSTR filename);
	void buildFX();
};

#endif