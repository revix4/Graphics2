//=============================================================================
//                              BaseObject3D
//
// Writen by Adi Bar-Lev, 2013
// EGP 300-101, Graphics Programming II  - skeleton project
//
// Base class that can handle 3D rendering via Vertex and Index buffer
//=============================================================================
#include "BaseObject3D.h"
#include "Vertex.h"
#include "../GfxStats.h"
//=============================================================================
BaseObject3D::BaseObject3D(void)
{
    m_VertexBuffer = NULL;
    m_IndexBuffer = NULL;

	color = D3DCOLOR(D3DCOLOR_XRGB(0, 0, 0));

    D3DXMatrixIdentity(&m_World);

	mesh = nullptr;
	shader = nullptr;
}

//-----------------------------------------------------------------------------
BaseObject3D::~BaseObject3D(void)
{
    //ReleaseCOM(m_VertexBuffer);
	//ReleaseCOM(m_IndexBuffer);

	ReleaseCOM(mesh);
}

//-----------------------------------------------------------------------------
void BaseObject3D::Create( IDirect3DDevice9* gd3dDevice )
{
	D3DXCreateEffectFromFileA(gd3dDevice, "Effect.fx", 0, 0, 0, 0, &shader, 0);

	D3DXCreateBox(gd3dDevice, 1, 1, 1, &mesh, 0);

    //buildVertexBuffer( gd3dDevice );
    //buildIndexBuffer( gd3dDevice );
}

//-----------------------------------------------------------------------------
void BaseObject3D::Render( IDirect3DDevice9* gd3dDevice,
    D3DXMATRIX& view, D3DXMATRIX& projection )
{
    // Update the statistics singlton class
    GfxStats::GetInstance()->addVertices(8);
    GfxStats::GetInstance()->addTriangles(12);

	HR(shader->SetMatrix("matViewProjection", &(m_World*view*projection)));

	unsigned int numPass = 0;
	HR(shader->Begin(&numPass, 0));

	for (unsigned int i = 0; i < numPass; i++)
	{
		HR(shader->BeginPass(i));

		// Set the buffers and format
		//HR(gd3dDevice->SetStreamSource(0, m_VertexBuffer, 0, sizeof(VertexPos)));
		//HR(gd3dDevice->SetIndices(m_IndexBuffer));
		//HR(gd3dDevice->SetVertexDeclaration(VertexPos::Decl));

		// Set matrices and model relevant render date
		HR(gd3dDevice->SetTransform(D3DTS_WORLD, &m_World));
		HR(gd3dDevice->SetTransform(D3DTS_VIEW, &view));
		HR(gd3dDevice->SetTransform(D3DTS_PROJECTION, &projection));

		HR(shader->CommitChanges());

		// Send to render
		//HR(gd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12));
		HR(mesh->DrawSubset(0));
		

		HR(shader->EndPass());
	}
	HR(shader->End());
}

void BaseObject3D::setWorldLocation(D3DXMATRIX transform)
{

	D3DXMatrixTranspose(&m_World, &transform);
}

//-----------------------------------------------------------------------------
void BaseObject3D::Update()
{

}

//-----------------------------------------------------------------------------
void BaseObject3D::buildVertexBuffer( IDirect3DDevice9* gd3dDevice )
{
	// Obtain a pointer to a new vertex buffer.
	HR(gd3dDevice->CreateVertexBuffer(8 * sizeof(VertexPos), D3DUSAGE_WRITEONLY,
		0, D3DPOOL_MANAGED, &m_VertexBuffer, 0));

	// Now lock it to obtain a pointer to its internal data, and write the
	// cube's vertex data.

	VertexPos* v = 0;
	HR(m_VertexBuffer->Lock(0, 0, (void**)&v, 0));

	v[0] = VertexPos(-1.0f, -1.0f, -1.0f, color, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f);
	v[1] = VertexPos(-1.0f, 1.0f, -1.0f, color, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f);
	v[2] = VertexPos(1.0f, 1.0f, -1.0f, color, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f);
	v[3] = VertexPos(1.0f, -1.0f, -1.0f, color, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f);
	v[4] = VertexPos(-1.0f, -1.0f, 1.0f, color, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[5] = VertexPos(-1.0f, 1.0f, 1.0f, color, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[6] = VertexPos(1.0f, 1.0f, 1.0f, color, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[7] = VertexPos(1.0f, -1.0f, 1.0f, color, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	HR(m_VertexBuffer->Unlock());
}

//-----------------------------------------------------------------------------
void BaseObject3D::buildIndexBuffer( IDirect3DDevice9* gd3dDevice )
{
	// Obtain a pointer to a new index buffer.
	HR(gd3dDevice->CreateIndexBuffer(36 * sizeof(WORD), D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_IndexBuffer, 0));

	// Now lock it to obtain a pointer to its internal data, and write the
	// cube's index data.

	WORD* k = 0;

	HR(m_IndexBuffer->Lock(0, 0, (void**)&k, 0));

	// Front face.
	k[0] = 0; k[1] = 1; k[2] = 2;
	k[3] = 0; k[4] = 2; k[5] = 3;

	// Back face.
	k[6] = 4; k[7]  = 6; k[8]  = 5;
	k[9] = 4; k[10] = 7; k[11] = 6;

	// Left face.
	k[12] = 4; k[13] = 5; k[14] = 1;
	k[15] = 4; k[16] = 1; k[17] = 0;

	// Right face.
	k[18] = 3; k[19] = 2; k[20] = 6;
	k[21] = 3; k[22] = 6; k[23] = 7;

	// Top face.
	k[24] = 1; k[25] = 5; k[26] = 6;
	k[27] = 1; k[28] = 6; k[29] = 2;

	// Bottom face.
	k[30] = 4; k[31] = 0; k[32] = 3;
	k[33] = 4; k[34] = 3; k[35] = 7;

	HR(m_IndexBuffer->Unlock());
}

//void 
//=============================================================================
