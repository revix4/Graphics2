#ifndef _SKY_H
#define _SKY_H

#include "Sphere.h"
#include "SkyMaterial.h"

class Sky : public Sphere
{
private:
	ID3DXEffect* mSkyEffect;
	SkyMaterial* m_skyMaterial;

public:
	Sky(const std::string& envmapFilename, float skyRadius);
	~Sky();

	virtual void Create(IDirect3DDevice9* gd3dDevice);
	virtual void RenderPhong(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection, boolean specularOn, boolean diffuseOn, boolean textureOn);
	virtual void Update(D3DXVECTOR3 lightPos, D3DXVECTOR3 viewPos);

	void draw();
};


#endif