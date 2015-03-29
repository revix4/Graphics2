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
	m_materialG = new BaseMaterial();
	m_materialP = new BaseMaterial();

    D3DXMatrixIdentity(&m_World);

	mesh = nullptr;
	Gouraud = nullptr;
	Phong = nullptr;
}

//-----------------------------------------------------------------------------
BaseObject3D::~BaseObject3D(void)
{
    //ReleaseCOM(m_VertexBuffer);
	//ReleaseCOM(m_IndexBuffer);

	ReleaseCOM(mesh);
	ReleaseCOM(Gouraud);
	ReleaseCOM(Phong);
}

//-----------------------------------------------------------------------------
void BaseObject3D::Create( IDirect3DDevice9* gd3dDevice )
{
	HRESULT hr;
	//m_material->setTexture(gd3dDevice, "stone2.dds");
	m_materialG->setTexture(gd3dDevice, "marble.bmp");
	m_materialP->setTexture(gd3dDevice, "marble.bmp");
	//D3DXCreateTextureFromFile(gd3dDevice, "marble.bmp", &m_texture);

	hr = D3DXCreateEffectFromFileA(gd3dDevice, "Textured_Phong.fx", 0, 0, 0, 0, &Phong, 0);
	hr = D3DXCreateEffectFromFileA(gd3dDevice, "Textured_Gouraud.fx", 0, 0, 0, 0, &Gouraud, 0);
	m_materialG->ConnectToEffect(Gouraud);
	m_materialG->buildFX();
	m_materialP->ConnectToEffect(Phong);
	m_materialP->buildFX();

	D3DXCreateBox(gd3dDevice, 10, 10, 10, &mesh, 0);

    //buildVertexBuffer( gd3dDevice );
    //buildIndexBuffer( gd3dDevice );

	setTexCoord(gd3dDevice);
}

//-----------------------------------------------------------------------------
void BaseObject3D::RenderPhong( IDirect3DDevice9* gd3dDevice,
    D3DXMATRIX& view, D3DXMATRIX& projection , boolean specularOn, boolean diffuseOn, boolean textureOn)
{
    // Update the statistics singlton class
    GfxStats::GetInstance()->addVertices(8);
    GfxStats::GetInstance()->addTriangles(12);

	HR(Phong->SetMatrix("matView", &m_World));
	HR(Phong->SetMatrix("matViewProjection", &(m_World*view*projection)));
	HR(Phong->SetBool("spec_On", specularOn));
	HR(Phong->SetBool("diff_On", diffuseOn));
	HR(Phong->SetBool("tex_On", textureOn));

	unsigned int numPass = 0;
	HR(Phong->Begin(&numPass, 0));

	for (unsigned int i = 0; i < numPass; i++)
	{
		HR(Phong->BeginPass(i));

		// Set the buffers and format
		//HR(gd3dDevice->SetStreamSource(0, m_VertexBuffer, 0, sizeof(VertexPos)));
		//HR(gd3dDevice->SetIndices(m_IndexBuffer));
		//HR(gd3dDevice->SetVertexDeclaration(VertexPos::Decl));

		// Set matrices and model relevant render date
		HR(gd3dDevice->SetTransform(D3DTS_WORLD, &m_World));
		HR(gd3dDevice->SetTransform(D3DTS_VIEW, &view));
		HR(gd3dDevice->SetTransform(D3DTS_PROJECTION, &projection));

		HR(Phong->CommitChanges());

		// Send to render
		//HR(gd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12));
		HR(mesh->DrawSubset(0));
		

		HR(Phong->EndPass());
	}
	HR(Phong->End());
}

void BaseObject3D::RenderGouraud(IDirect3DDevice9* gd3dDevice,
	D3DXMATRIX& view, D3DXMATRIX& projection, boolean specularOn, boolean diffuseOn, boolean textureOn)
{
	// Update the statistics singlton class
	GfxStats::GetInstance()->addVertices(8);
	GfxStats::GetInstance()->addTriangles(12);

	HR(Gouraud->SetMatrix("matView", &m_World));
	HR(Gouraud->SetMatrix("matViewProjection", &(m_World*view*projection)));
	HR(Gouraud->SetBool("spec_On", specularOn));
	HR(Gouraud->SetBool("diff_On", diffuseOn));
	HR(Gouraud->SetBool("tex_On", textureOn));

	unsigned int numPass = 0;
	HR(Gouraud->Begin(&numPass, 0));

	for (unsigned int i = 0; i < numPass; i++)
	{
		HR(Gouraud->BeginPass(i));

		// Set the buffers and format
		//HR(gd3dDevice->SetStreamSource(0, m_VertexBuffer, 0, sizeof(VertexPos)));
		//HR(gd3dDevice->SetIndices(m_IndexBuffer));
		//HR(gd3dDevice->SetVertexDeclaration(VertexPos::Decl));

		// Set matrices and model relevant render date
		HR(gd3dDevice->SetTransform(D3DTS_WORLD, &m_World));
		HR(gd3dDevice->SetTransform(D3DTS_VIEW, &view));
		HR(gd3dDevice->SetTransform(D3DTS_PROJECTION, &projection));

		HR(Gouraud->CommitChanges());

		// Send to render
		//HR(gd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12));
		HR(mesh->DrawSubset(0));


		HR(Gouraud->EndPass());
	}
	HR(Gouraud->End());
}

void BaseObject3D::setWorldLocation(D3DXMATRIX transform)
{
	D3DXMatrixTranspose(&m_World, &transform);
}

//-----------------------------------------------------------------------------
void BaseObject3D::Update(D3DXVECTOR3 lightPos, D3DXVECTOR3 viewPos)
{
	m_materialG->Update(lightPos, viewPos);
	m_materialP->Update(lightPos, viewPos);
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

	v[0] = VertexPos(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f);
	v[1] = VertexPos(-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[2] = VertexPos(1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[3] = VertexPos(1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f);
	v[4] = VertexPos(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f);
	v[5] = VertexPos(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f);
	v[6] = VertexPos(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[7] = VertexPos(1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f);

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

void BaseObject3D::setTexCoord(IDirect3DDevice9* gd3dDevice)
{
	D3DVERTEXELEMENT9 elements[64];
	UINT numElements = 0;
	VertexPos::Decl->GetDeclaration(elements, &numElements);

	ID3DXMesh* temp = 0;

	HR(mesh->CloneMesh(D3DXMESH_SYSTEMMEM, elements, gd3dDevice, &temp));

	ReleaseCOM(mesh);

	//gen tex coords
	VertexPos* vertices = 0;
	HR(temp->LockVertexBuffer(0, (void**)&vertices));

	D3DXVECTOR3 maxPoint(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	D3DXVECTOR3 minPoint(FLT_MAX, FLT_MAX, FLT_MAX);

	for (UINT i = 0; i < temp->GetNumVertices(); ++i)
	{
		D3DXVec3Maximize(&maxPoint, &maxPoint, &vertices[i].pos);
		D3DXVec3Minimize(&minPoint, &minPoint, &vertices[i].pos);
	}

	// Iterate over each vertex and compute its texture coordinate.
	for (UINT i = 0; i < temp->GetNumVertices(); ++i)
	{
		// Get the coordinates along the axes orthogonal to the
		// axis the cylinder is aligned with.

		float a = 0.0f;
		float b = 0.0f;
		float h = 0.0f;

		a = minPoint.x;
		b = maxPoint.x;
		h = b - a;

		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;

		x = vertices[i].pos.x;
		z = vertices[i].pos.y;
		y = vertices[i].pos.z;

		float u = (y) / h - 0.5f;
		float v = (x) / h - 0.5f;

		vertices[i].tex0.x = u;
		vertices[i].tex0.y = v;
	}

	HR(temp->UnlockVertexBuffer());

	// Clone back to a hardware mesh.
	HR(temp->CloneMesh(D3DXMESH_MANAGED | D3DXMESH_WRITEONLY,
		elements, gd3dDevice, &mesh));

	ReleaseCOM(temp);
}

//void 
//=============================================================================
