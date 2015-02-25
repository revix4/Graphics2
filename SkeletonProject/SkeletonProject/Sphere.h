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
private:
	float m_radius;
	int m_sliceCount;
	int m_stackCount;

	int m_wallVerticeCount;
	int m_capVerticeCount;

	int m_verticeCount;
	int m_indiceCount;
	int m_triangleCount;

protected:
	virtual void buildVertexBuffer(IDirect3DDevice9* gd3dDevice);
	virtual void buildIndexBuffer(IDirect3DDevice9* gd3dDevice);

public:
	Sphere(float radius, int sliceCount, int stackCount);
	~Sphere();

	virtual void Create(IDirect3DDevice9* gd3dDevice);
	virtual void Render(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection);
	virtual void Update();
};

#endif