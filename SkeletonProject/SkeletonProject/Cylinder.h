//=============================================================================
//                              Cylinder
//
// Writen by James Potvin and Paul Webster-Pact, 2015
// EGP 300-101, Graphics Programming II  - Project 1
//
// Class that builds and renders a cylinder
//=============================================================================
#ifndef _CYLINDER_H
#define _CYLINDER_H
//=============================================================================
#pragma once
//=============================================================================
#include "3DClasses\BaseObject3D.h"
//=============================================================================
class Cylinder : public BaseObject3D
{
private:
	float m_topRadius;
	float m_bottomRadius;
	float m_height;
	int m_sliceCount;
	int m_stackCount;

	int m_wallVerticeCount;
	int m_capVerticeCount;

	int m_verticeCount;
	int m_indiceCount;
	int m_triangleCount;

	void buildCapVertex(IDirect3DDevice9* gd3dDevice);
	void buildCapIndex(IDirect3DDevice9* gd3dDevice);

protected:
	virtual void buildVertexBuffer(IDirect3DDevice9* gd3dDevice);
	virtual void buildIndexBuffer(IDirect3DDevice9* gd3dDevice);
	virtual void setTexCoord(IDirect3DDevice9* gd3dDevice);

public:
	Cylinder(float topRadius, float bottomRadius, float height, int slices, int stacks);
	~Cylinder();

	virtual void Create(IDirect3DDevice9* gd3dDevice);
	virtual void RenderPhong(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection, boolean specularOn, boolean diffuseOn, boolean textureOn);
	virtual void RenderGouraud(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection, boolean specularOn, boolean diffuseOn, boolean textureOn);
	virtual void Update(D3DXVECTOR3 lightPos, D3DXVECTOR3 viewPos);
};

#endif