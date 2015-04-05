#include "Sky.h"

Sky::Sky(const std::string& envmapFilename, float skyRadius):
	Sphere(skyRadius, 30, 30)
{
	m_skyMaterial = new SkyMaterial();
}

Sky::~Sky()
{
	Sphere::~Sphere();
}

void Sky::Create(IDirect3DDevice9* gd3dDevice)
{
	m_skyMaterial->setTexture(gd3dDevice, "grassenvmap1024.dds");

	HRESULT hr = D3DXCreateEffectFromFile(gd3dDevice, "sky.fx", 0, 0, 0, 0, &mSkyEffect, 0);
	//HR(D3DXCreateEffectFromFile(gd3dDevice, "sky.fx", 0, 0, 0, 0, &mSkyEffect, 0));

	m_skyMaterial->ConnectToEffect(mSkyEffect);

	m_skyMaterial->buildFX();

	D3DXCreateSphere(gd3dDevice, m_radius, m_sliceCount, m_stackCount, &mesh, 0);

	//buildVertexBuffer(gd3dDevice);
	//buildIndexBuffer(gd3dDevice);
	Sphere::setTexCoord(gd3dDevice);
}

void Sky::RenderPhong(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection, boolean specularOn, boolean diffuseOn, boolean textureOn)
{
	D3DXMATRIX world;
	D3DXVECTOR3 pos(0, 0, 0);
	D3DXMatrixTranslation(&world, pos.x, pos.y, pos.z);
	HR(mSkyEffect->SetMatrix("gWVP", &(world*view*projection)));

	unsigned int numPass = 0;
	HR(mSkyEffect->Begin(&numPass, 0));
	HR(mSkyEffect->BeginPass(0));

	// Set matrices and model relevant render date
	HR(gd3dDevice->SetTransform(D3DTS_WORLD, &m_World));
	HR(gd3dDevice->SetTransform(D3DTS_VIEW, &view));
	HR(gd3dDevice->SetTransform(D3DTS_PROJECTION, &projection));

	HR(mSkyEffect->CommitChanges());

	// Send to render
	HR(mesh->DrawSubset(0));
	
	HR(mSkyEffect->EndPass());
	HR(mSkyEffect->End());
	
}

void Sky::Update(D3DXVECTOR3 lightPos, D3DXVECTOR3 viewPos)
{

}