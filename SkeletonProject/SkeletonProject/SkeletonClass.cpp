//=============================================================================
//                          SkeletonClass.cpp
//
// Adopted from CubeDemo.cpp by Frank Luna (C) 2005 All Rights Reserved.
//
// Edited and changed by Adi Bar-Lev, 2013
// EGP-300-101 - Computer Graphics II, Spring 2013.
//
// This project creates a skeleton workspace and base classes for the student to be 
// able to start working with sample classes and develop evolve it according to the 
// given exercises
//
// Controls: Use mouse to orbit and zoom; use the 'W' and 'S' keys to 
//           alter the height of the camera.
//=============================================================================
#include "DirectInput.h"
#include <crtdbg.h>
#include "GfxStats.h"
#include <list>

#include "SkeletonClass.h"
#include "3DClasses\BaseObject3D.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Cone.h"
#include "3DClasses\Vertex.h"
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif

	SkeletonClass app(hInstance, "Exercise Skeleton Project", D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING);
	gd3dApp = &app;

	DirectInput di(DISCL_NONEXCLUSIVE|DISCL_FOREGROUND, DISCL_NONEXCLUSIVE|DISCL_FOREGROUND);
	gDInput = &di;

	return gd3dApp->run();
}

SkeletonClass::SkeletonClass(HINSTANCE hInstance, std::string winCaption, D3DDEVTYPE devType, DWORD requestedVP)
: D3DApp(hInstance, winCaption, devType, requestedVP)
{
	mUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	if(!checkDeviceCaps())
	{
		MessageBox(0, "checkDeviceCaps() Failed", 0, 0);
		PostQuitMessage(0);
	}

	mCameraRadius    = 50.0f;
	mCameraRotationY = 1.75f * D3DX_PI;
	mCameraRotationX = 1.75f * D3DX_PI;

	InitAllVertexDeclarations();

	skyBox = new Sky("cubeMap.dds", 10000);
	skyBox->Create(gd3dDevice);

    // repleace or add to the following object creation
	m_Objects.push_back(new Cylinder(5, 5, 10, 25, 25));
    m_Objects[0]->Create( gd3dDevice );

	m_Objects.push_back(new Sphere(5, 25, 25));
	m_Objects[1]->Create(gd3dDevice);

	m_Objects.push_back(new Cone(5,10,25));
	m_Objects[2]->Create(gd3dDevice);

	m_Objects.push_back(new BaseObject3D());
	m_Objects[3]->Create(gd3dDevice);

	mLightPos = D3DXVECTOR3(50, 100, 1);

	mWireFrameOn = false;
	mTextureOn = true;
	GfxStats::GetInstance()->setTextureOn(mTextureOn);
	mNormalMappingOn = true;
	GfxStats::GetInstance()->setNormalMappingOn(mNormalMappingOn);
	mReflectionOn = true;
	GfxStats::GetInstance()->setReflectionOn(mReflectionOn);
	mCurrentObject = 0;
	mSpecularOn = true;
	mDiffuseOn = true;
	mPhongOn = true;

	mReflectSpecBlend = 1;
	mNormalStrength = 0.7f;
	mSpecularCoefficient = 2;
	GfxStats::GetInstance()->setSpecularcoefficient(mSpecularCoefficient);

	onResetDevice();
}

SkeletonClass::~SkeletonClass()
{
    GfxStats::DeleteInstance();

    for ( unsigned int obj=0 ; obj<m_Objects.size() ; obj++ )
        delete m_Objects[obj];
    m_Objects.clear();

	delete skyBox;
	skyBox = NULL;

	DestroyAllVertexDeclarations();
}

bool SkeletonClass::checkDeviceCaps()
{
	D3DCAPS9 caps;
	HR(gd3dDevice->GetDeviceCaps(&caps));

	// Check for vertex shader version 2.0 support.
	if (caps.VertexShaderVersion < D3DVS_VERSION(2, 0))
	{
		return false;
	}

	// Check for pixel shader version 2.0 support.
	if (caps.PixelShaderVersion < D3DPS_VERSION(2, 0))
	{
		return false;
	}
	return true;
}

void SkeletonClass::onLostDevice()
{
	GfxStats::GetInstance()->onLostDevice();
}

void SkeletonClass::onResetDevice()
{
	GfxStats::GetInstance()->onResetDevice();

	// The aspect ratio depends on the backbuffer dimensions, which can 
	// possibly change after a reset.  So rebuild the projection matrix.
	buildProjMtx();
}

void SkeletonClass::updateScene(float dt)
{
	// Reset the statistics for the scene - each object will add to it.
	GfxStats::GetInstance()->setVertexCount(0);
	GfxStats::GetInstance()->setTriCount(0);
	GfxStats::GetInstance()->update(dt);

	input(dt);

	if (mCurrentObject == m_Objects.size())
		mCurrentObject = 0;

	//if we go above 360 or below 0, wrap around
	if (mCameraRotationX > 2 * D3DX_PI)
		mCameraRotationX = 0;
	else if (mCameraRotationX < 0)
		mCameraRotationX = 2 * D3DX_PI;

	//if the camera is on one side, it's up is one direction. The other side, the camera's up is in the opposite direction
	//this prevents the camera from switching orientation at the bottom of the camera swing
	if (mCameraRotationX > D3DX_PI)
		mUp.y = 1;
	if (mCameraRotationX < D3DX_PI)
		mUp.y = -1;
		
	// If we rotate over 360 degrees, just roll back to 0
	if (fabsf(mCameraRotationY) >= 2.0f * D3DX_PI)
		mCameraRotationY = 0.0f;

	// Don't let radius get too small.
	if( mCameraRadius < 5.0f )
		mCameraRadius = 5.0f;

	if (mNormalStrength < 0.0f)
	{
		mNormalStrength = 0.0f;
	}
	if (mNormalStrength > 1.0f)
	{
		mNormalStrength = 1.0f;
	}

	if (mReflectSpecBlend < 0.0f)
	{
		mReflectSpecBlend = 0.0f;
	}
	if (mReflectSpecBlend > 1.0f)
	{
		mReflectSpecBlend = 1.0f;
	}
	GfxStats::GetInstance()->setReflectSpecBlend(mReflectSpecBlend);
	GfxStats::GetInstance()->setNormalStrength(mNormalStrength);
	// The camera position/orientation relative to world space can 
	// change every frame based on input, so we need to rebuild the
	// view matrix every frame with the latest changes.
	buildViewMtx();

	//D3DXMATRIX mViewInv;
	//D3DXMatrixTranspose(&mViewInv, &mView);
	m_Objects[mCurrentObject % m_Objects.size()]->Update(mLightPos, D3DXVECTOR3(mView._41, mView._42, mView._43), mSpecularCoefficient);
}


void SkeletonClass::drawScene()
{
	// Clear the backbuffer and depth buffer.
	HR(gd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0));

	HR(gd3dDevice->BeginScene());

	// Set render statws for the entire scene here:
	if (mWireFrameOn)
	{
		HR(gd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME));
	}
	else
	{
		HR(gd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID));
	}

	// Render all the objects
	if (mPhongOn)
	{
		skyBox->RenderPhong(gd3dDevice, mView, mProj, mSpecularOn, mDiffuseOn, mTextureOn);
		m_Objects[mCurrentObject % m_Objects.size()]->RenderPhong(gd3dDevice, mView, mProj, mSpecularOn, mNormalMappingOn, mTextureOn, mNormalStrength, mReflectSpecBlend);
	}
	else
	{
		m_Objects[mCurrentObject % m_Objects.size()]->RenderGouraud(gd3dDevice, mView, mProj, mSpecularOn, mDiffuseOn, mTextureOn);
	}

	// display the render statistics
	GfxStats::GetInstance()->display();

	HR(gd3dDevice->EndScene());

	// Present the backbuffer.
	HR(gd3dDevice->Present(0, 0, 0, 0));
}

void SkeletonClass::buildViewMtx()
{
	float x = mCameraRadius * cosf(mCameraRotationY) * sinf(mCameraRotationX);
	float y = mCameraRadius * cosf(mCameraRotationX);
	float z = mCameraRadius * sinf(mCameraRotationY) * sinf(mCameraRotationX);
	D3DXVECTOR3 pos(x, y, z);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXMatrixLookAtLH(&mView, &pos, &target, &mUp);
}

void SkeletonClass::input(float dt)
{
	// Get snapshot of input devices.
	gDInput->poll();

	if (gDInput->keyDown(DIK_I))
		mCameraRotationX -= 5.0f * dt;
	if (gDInput->keyDown(DIK_K))
		mCameraRotationX += 5.0f * dt;

	if (gDInput->keyDown(DIK_J))
		mCameraRotationY += 5.0f * dt;
	if (gDInput->keyDown(DIK_L))
		mCameraRotationY -= 5.0f * dt;

	if (gDInput->mouseButtonDown(0))
		mCameraRadius -= 25.0f * dt;
	if (gDInput->mouseButtonDown(1))
		mCameraRadius += 25.0f * dt;

	if (gDInput->keyPressed(DIK_W))
		mWireFrameOn = !mWireFrameOn;
	if (gDInput->keyPressed(DIK_T))
	{
		mTextureOn = !mTextureOn;
		GfxStats::GetInstance()->setTextureOn(mTextureOn);
	}
	if (gDInput->keyPressed(DIK_O))
		mCurrentObject++;
	if (gDInput->keyPressed(DIK_D))
		mDiffuseOn = !mDiffuseOn;

	if (gDInput->keyPressed(DIK_R))
	{
		mReflectionOn = !mReflectionOn;
		GfxStats::GetInstance()->setReflectionOn(mReflectionOn);
	}
	if (gDInput->keyPressed(DIK_N))
	{
		mNormalMappingOn = !mNormalMappingOn;
		GfxStats::GetInstance()->setNormalMappingOn(mNormalMappingOn);
	}

	if (gDInput->keyPressed(DIK_S))
	{
		mNormalStrength += .1;
	}
	if (gDInput->keyPressed(DIK_A))
	{
		mNormalStrength -= .1;
	}

	if (gDInput->keyPressed(DIK_EQUALS))
	{
		mReflectSpecBlend += .1;
	}
	if (gDInput->keyPressed(DIK_MINUS))
	{
		mReflectSpecBlend -= .1;
	}

	if (gDInput->keyPressed(DIK_1))
	{
		mSpecularCoefficient = 2;
	}
	if (gDInput->keyPressed(DIK_2))
	{
		mSpecularCoefficient = 4;
	}
	if (gDInput->keyPressed(DIK_3))
	{
		mSpecularCoefficient = 8;
	}
	if (gDInput->keyPressed(DIK_4))
	{
		mSpecularCoefficient = 16;
	}
	if (gDInput->keyPressed(DIK_5))
	{
		mSpecularCoefficient = 32;
	}
	if (gDInput->keyPressed(DIK_6))
	{
		mSpecularCoefficient = 64;
	}
	if (gDInput->keyPressed(DIK_7))
	{
		mSpecularCoefficient = 128;
	}
	GfxStats::GetInstance()->setSpecularcoefficient(mSpecularCoefficient);
}

void SkeletonClass::buildProjMtx()
{
	float w = (float)md3dPP.BackBufferWidth;
	float h = (float)md3dPP.BackBufferHeight;
	D3DXMatrixPerspectiveFovLH(&mProj, D3DX_PI * 0.25f, w/h, 1.0f, 5000.0f);
}