//=============================================================================
//                              Cylinder
//
// Writen by James Potvin, 2015
// EGP 300-101, Graphics Programming II  - Project 1
//
// Class that builds and renders a cylinder
//=============================================================================
#include "Cylinder.h"
#include "3DClasses\Vertex.h"
#include "GfxStats.h"
#include <cmath>
//=============================================================================
Cylinder::Cylinder(float topRadius, float bottomRadius, float height, int slices, int stacks)
	:BaseObject3D()
{
	m_topRadius = topRadius;
	m_bottomRadius = bottomRadius;
	m_height = height;
	m_sliceCount = slices;
	m_stackCount = stacks;

	m_wallVerticeCount = (m_stackCount + 1) * (m_sliceCount + 1);
	m_capVerticeCount = m_sliceCount + 2;

	m_verticeCount = m_wallVerticeCount + (2 * m_capVerticeCount);
	m_indiceCount = (6 * (m_stackCount * m_sliceCount)) + 6 * m_sliceCount;
	m_triangleCount = 2 * (m_stackCount*m_sliceCount) + (2 * m_sliceCount);

	color = D3DCOLOR(D3DCOLOR_XRGB(0, 255, 0));
}

//-----------------------------------------------------------------------------
Cylinder::~Cylinder()
{
	ReleaseCOM(m_VertexBuffer);
	ReleaseCOM(m_IndexBuffer);
}

//-----------------------------------------------------------------------------
void Cylinder::Create(IDirect3DDevice9* gd3dDevice)
{
	buildVertexBuffer(gd3dDevice);
	buildIndexBuffer(gd3dDevice);
}

//-----------------------------------------------------------------------------
void Cylinder::Render(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection)
{
	// Update the statistics singlton class
	GfxStats::GetInstance()->addVertices(m_verticeCount);
	GfxStats::GetInstance()->addTriangles(m_triangleCount);//how many triangles are there?

	// Set the buffers and format
	HR(gd3dDevice->SetStreamSource(0, m_VertexBuffer, 0, sizeof(VertexPos)));
	HR(gd3dDevice->SetIndices(m_IndexBuffer));
	HR(gd3dDevice->SetVertexDeclaration(VertexPos::Decl));

	// Set matrices and model relevant render date
	HR(gd3dDevice->SetTransform(D3DTS_WORLD, &m_World));
	HR(gd3dDevice->SetTransform(D3DTS_VIEW, &view));
	HR(gd3dDevice->SetTransform(D3DTS_PROJECTION, &projection));

	// Send to render
	HR(gd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_verticeCount, 0, m_triangleCount));
}

//-----------------------------------------------------------------------------
void Cylinder::Update()
{

}

//-----------------------------------------------------------------------------
void Cylinder::buildVertexBuffer(IDirect3DDevice9* gd3dDevice)
{
	float stackHeight = m_height / m_stackCount;

	float radiusStep = (m_topRadius - m_bottomRadius) / m_stackCount;

	int ringCount = m_stackCount + 1;
	float dTheta = (float)(2.0f * PI / m_sliceCount);
	int vertexCount = 0;
	// Obtain a pointer to a new vertex buffer.
	HR(gd3dDevice->CreateVertexBuffer(m_verticeCount * sizeof(VertexPos), D3DUSAGE_WRITEONLY,
		0, D3DPOOL_MANAGED, &m_VertexBuffer, 0));

	// Now lock it to obtain a pointer to its internal data, and write the
	// cylinder's vertex data.

	VertexPos* v = 0;
	HR(m_VertexBuffer->Lock(0, 0, (void**)&v, 0));

	//compute vertices for wall
	for (int i = 0; i < ringCount; i++)
	{
		float y = -0.5f * m_height + i * stackHeight;//how high up
		float r = m_bottomRadius + i * radiusStep;//radius at that height

		//ring vertices
		for (int j = 0; j <= m_sliceCount; j++)
		{
			float c = cosf(j*dTheta);
			float s = sinf(j*dTheta);

			D3DXVECTOR3 t(-s, 0.0f, c);

			float dr = m_bottomRadius - m_topRadius;
			D3DXVECTOR3 b(dr*c, -m_height, dr*s);

			D3DXVECTOR3 *n = D3DXVec3Normalize(nullptr, D3DXVec3Cross(nullptr, &t, &b));

			v[vertexCount] = VertexPos(r*c, y, r*s, color, (float)j/m_sliceCount, 1.0f - (float)i/m_stackCount, n->x, n->y, n->z);
			vertexCount++;
		}
	}

	//top cap
	float y = 0.5f * m_height;

	for (int i = 0; i <= m_sliceCount; i++)
	{
		float x = m_topRadius*cosf(i*dTheta);
		float z = m_topRadius*sinf(i*dTheta);

		float U = x / m_height + 0.5f;
		float V = z / m_height + 0.5f;

		v[vertexCount] = VertexPos(x, y, z, color, U, V, 0.0f, 1.0f, 0.0f);
		vertexCount++;
	}
	v[vertexCount] = VertexPos(0.0f, y, 0.0f, color, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f);
	vertexCount++;

	//bottom cap
	y = -0.5f * m_height;

	for (int i = m_sliceCount; i >= 0; i--)
	{
		float x = m_bottomRadius*cosf(i*dTheta);
		float z = m_bottomRadius*sinf(i*dTheta);

		float U = x / m_height - 0.5f;
		float V = z / m_height - 0.5f;

		v[vertexCount] = VertexPos(x, y, z, color, U, V, 0.0f, -1.0f, 0.0f);
		vertexCount++;
	}

	v[vertexCount] = VertexPos(0.0f, y, 0.0f, color, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f);
	vertexCount++;

	HR(m_VertexBuffer->Unlock());
}

//-----------------------------------------------------------------------------
void Cylinder::buildIndexBuffer(IDirect3DDevice9* gd3dDevice)
{
	int ringVertexCount = m_sliceCount + 1;

	// Obtain a pointer to a new index buffer.
	HR(gd3dDevice->CreateIndexBuffer(m_indiceCount * sizeof(WORD), D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_IndexBuffer, 0));

	// Now lock it to obtain a pointer to its internal data, and write the
	// cylinder's index data.

	WORD* k = 0;
	int indexCount = 0;

	HR(m_IndexBuffer->Lock(0, 0, (void**)&k, 0));

	for (int i = 0; i < m_stackCount; i++)
	{
		for (int j = 0; j < m_sliceCount; j++)
		{
			//one square

			k[indexCount] = i*ringVertexCount + j;
			k[indexCount + 1] = (i + 1) * ringVertexCount + j;
			k[indexCount + 2] = (i + 1) * ringVertexCount + j + 1;

			k[indexCount + 3] = i * ringVertexCount + j;
			k[indexCount + 4] = (i + 1) * ringVertexCount + j + 1;
			k[indexCount + 5] = i * ringVertexCount + j + 1;

			indexCount += 6;
		}
	}

	//top cap
	int baseIndex = m_wallVerticeCount;
	for (int i = 0; i < m_sliceCount; i++)
	{
		k[indexCount] = m_verticeCount - m_capVerticeCount - 1;
		k[indexCount + 1] = baseIndex + i + 1;
		k[indexCount + 2] = baseIndex + i;
		indexCount += 3;
	}
	//bottom cap
	baseIndex = m_wallVerticeCount + m_capVerticeCount;
	for (int i = 0; i < m_sliceCount; i++)
	{
		k[indexCount] = m_verticeCount - 1;
		k[indexCount + 1] = baseIndex + i + 1;
		k[indexCount + 2] = baseIndex + i;
		indexCount += 3;
	}

	HR(m_IndexBuffer->Unlock());
}

void Cylinder::buildCapVertex(IDirect3DDevice9* gd3dDevic)
{

}

void Cylinder::buildCapIndex(IDirect3DDevice9* gd3dDevic)
{

}