//=============================================================================
//                              Cone
//
// Writen by James Potvin and Paul Webster-Pact, 2015
// EGP 300-101, Graphics Programming II  - Project 1
//
// Class that builds and renders a cone
//=============================================================================
#ifndef _CONE_H
#define _CONE_H
//=============================================================================
#pragma once
//=============================================================================
#include "3DClasses\BaseObject3D.h"
#include "Cylinder.h"
//=============================================================================
class Cone : public Cylinder
{
protected:
	virtual void buildVertexBuffer(IDirect3DDevice9* gd3dDevice);
	virtual void buildIndexBuffer(IDirect3DDevice9* gd3dDevice);

public:
	Cone(float radius, float height, int slices);
	~Cone();

	virtual void Create(IDirect3DDevice9* gd3dDevice);
	virtual void RenderGouraud(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection, boolean specularOn, boolean diffuseOn, boolean textureOn);
	virtual void RenderPhong(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection, boolean specularOn, boolean diffuseOn, boolean textureOn);
	virtual void Update(D3DXVECTOR3 lightPos, D3DXVECTOR3 viewPos);
};

#endif