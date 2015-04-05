//=============================================================================
//                              Sphere
//
// Writen by James Potvin and Paul Webster-Pact, 2015
// EGP 300-101, Graphics Programming II  - Project 1
//
// Class that builds and renders a cylinder
//=============================================================================
#ifndef _SPHERE_H
#define _SPHERE_H
//=============================================================================
#pragma once
//=============================================================================
#include "3DClasses\BaseObject3D.h"
//=============================================================================
class Sphere : public BaseObject3D
{
protected:
	float m_radius;
	int m_sliceCount;
	int m_stackCount;

	int m_wallVerticeCount;
	int m_capVerticeCount;

	int m_verticeCount;
	int m_indiceCount;
	int m_triangleCount;

	virtual void buildVertexBuffer(IDirect3DDevice9* gd3dDevice);
	virtual void buildIndexBuffer(IDirect3DDevice9* gd3dDevice);
	virtual void setTexCoord(IDirect3DDevice9* gd3dDevice);

public:
	Sphere(float radius, int sliceCount, int stackCount);
	~Sphere();

	virtual void Create(IDirect3DDevice9* gd3dDevice);
	virtual void RenderGouraud(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection, boolean specularOn, boolean diffuseOn, boolean textureOn);
	virtual void RenderPhong(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection, boolean specularOn, boolean diffuseOn, boolean textureOn);
	virtual void Update(D3DXVECTOR3 lightPos, D3DXVECTOR3 viewPos);
};

#endif