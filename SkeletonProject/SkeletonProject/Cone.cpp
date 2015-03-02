//=============================================================================
//                              Cone
//
// Writen by James Potvin, 2015
// EGP 300-101, Graphics Programming II  - Project 1
//
// Class that builds and renders a cone
//=============================================================================
#include "Cone.h"
#include "3DClasses\Vertex.h"
#include "GfxStats.h"
//=============================================================================
Cone::Cone(float radius, float height, int slices)
	:Cylinder(0, radius, height, slices, 1)
{

}

//-----------------------------------------------------------------------------
Cone::~Cone()
{

}

//-----------------------------------------------------------------------------
void Cone::Create(IDirect3DDevice9* gd3dDevice)
{
	Cylinder::Create(gd3dDevice);
}

//-----------------------------------------------------------------------------
void Cone::Render(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection, boolean specularOn, boolean diffuseOn, boolean textureOn)
{
	Cylinder::Render(gd3dDevice, view, projection, specularOn, diffuseOn, textureOn);
}

//-----------------------------------------------------------------------------
void Cone::Update(D3DXVECTOR3 lightPos, D3DXVECTOR3 viewPos)
{
	m_material->Update(lightPos, viewPos);
}

//-----------------------------------------------------------------------------
void Cone::buildVertexBuffer(IDirect3DDevice9* gd3dDevice)
{
	Cylinder::buildVertexBuffer(gd3dDevice);
}

//-----------------------------------------------------------------------------
void Cone::buildIndexBuffer(IDirect3DDevice9* gd3dDevice)
{
	Cylinder::buildIndexBuffer(gd3dDevice);
}