//=============================================================================
//                          SkeletonClass.h
//
// Adopted from CubeDemo.cpp by Frank Luna (C) 2005 All Rights Reserved.
//
// Edited and changed by Adi Bar-Lev, 2013
// EGP-300-101 - Computer Graphics II, Spring 2013.
//
// Edited and changed by James Potvin and Paul Webster-Pact, 2015
// EGP 300-101, Graphics Programming II  - Project 1
//
// This project creates a skeleton workspace and base classes for the student to be 
// able to start working with sample classes and develop evolve it according to the 
// given exercises
//
// Controls: Use the 'A' and 'D' keys to rotate the camera around the Y axis,
//			 the 'W' and 'S' keys to rotate the camera around the X axis,
//			 the left and right mouse button to zoom in and out
//=============================================================================
#ifndef _SKELETON_CLASS_H
#define _SKELETON_CLASS_H
//=============================================================================
#pragma once
//=============================================================================
#include "d3dApp.h"

#include <vector>
//=============================================================================
class BaseObject3D;
//=============================================================================
class SkeletonClass : public D3DApp
{
public:
	SkeletonClass(HINSTANCE hInstance, std::string winCaption, D3DDEVTYPE devType, DWORD requestedVP);
	~SkeletonClass();

	bool checkDeviceCaps();
	void onLostDevice();
	void onResetDevice();
	void updateScene(float dt);
	void drawScene();

	// Helper methods
	void buildViewMtx();
	void buildProjMtx();

private:
	float mCameraRotationX;
	float mCameraRotationY;
	float mCameraRadius;

	bool mWireFrameOn;
	bool mTextureOn;
	int mCurrentObject;
	bool mSpecularOn;
	bool mDiffuseOn;
	bool mPhongOn;

	D3DXMATRIX mView;
	D3DXMATRIX mProj;

	D3DXVECTOR3 mUp;
	D3DXVECTOR3 mLightPos;

    std::vector<BaseObject3D*>      m_Objects;
};
//=============================================================================
#endif