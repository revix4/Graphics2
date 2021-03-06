//=============================================================================
//                              BaseObject3D
//
// Writen by Adi Bar-Lev, 2013
// EGP 300-101, Graphics Programming II  - skeleton project
//
// Base class that can handle 3D rendering via Vertex and Index buffer
//=============================================================================
#ifndef _BASE_OBJECT_3D_H
#define _BASE_OBJECT_3D_H
//=============================================================================
#pragma once
//=============================================================================
#include <d3dx9.h>
#include "Vertex.h"
#include "../BaseMaterial.h"
#include "../NormalMapMaterial.h"
#include "../ReflectiveMaterial.h"
#include "../d3dUtil.h"
//=============================================================================
struct IDirect3DVertexBuffer9;
struct IDirect3DIndexBuffer9;
//=============================================================================
const double PI = 4.0*atan(1.0);

class BaseObject3D
{
protected:	
    D3DXMATRIX                  m_World;

	IDirect3DVertexBuffer9*     m_VertexBuffer;
	IDirect3DIndexBuffer9*      m_IndexBuffer;

	D3DCOLOR color;

	ID3DXMesh *mesh;

	ID3DXEffect *Gouraud;
	ID3DXEffect *Phong;

	ID3DXEffect *NormalMap;

	BaseMaterial *m_materialG;
	BaseMaterial *m_materialP;

	NormalMapMaterial *m_NormalMat;

	ReflectiveMaterial *m_reflMat;

protected:
    // Replace the code in the following methods
    virtual void buildVertexBuffer( IDirect3DDevice9* gd3dDevice );
    virtual void buildIndexBuffer( IDirect3DDevice9* gd3dDevice );
	virtual void setTexCoord(IDirect3DDevice9* gd3dDevice);

public:
    BaseObject3D(void);
    ~BaseObject3D(void);

    // Replace or add to the following code as you progress with the material
    virtual void Create( IDirect3DDevice9* gd3dDevice );
	virtual void RenderPhong(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection, boolean specularOn, boolean normalMapOn, boolean textureOn, float normalStrength, float reflectivity);
	virtual void RenderGouraud(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection, boolean specularOn, boolean diffuseOn, boolean textureOn);
	virtual void Update(D3DXVECTOR3 lightPos, D3DXVECTOR3 viewPos, float shine = 0);
	void setWorldLocation(D3DXMATRIX world);
	void setTBN(IDirect3DDevice9* gd3dDevice);
};
//=============================================================================
#endif // _BASE_OBJECT_3D_H

