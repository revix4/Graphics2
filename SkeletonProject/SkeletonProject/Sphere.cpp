//=============================================================================
//                              Sphere
//
// Writen by James Potvin, 2015
// EGP 300-101, Graphics Programming II  - Project 1
//
// Class that builds and renders a sphere
//=============================================================================
#include "Sphere.h"
#include "3DClasses\Vertex.h"
#include "GfxStats.h"
//=============================================================================
Sphere::Sphere(float radius, int sliceCount, int stackCount)
	:BaseObject3D()
{
	m_radius = radius;
	m_sliceCount = sliceCount;
	m_stackCount = stackCount;

	m_verticeCount = (m_stackCount - 1) * (m_sliceCount + 1) + 2;
	m_indiceCount = 6 * m_sliceCount + (m_stackCount - 2) * m_sliceCount * 6;
	m_triangleCount = 2 * (m_stackCount*m_sliceCount);

	color = D3DCOLOR(D3DCOLOR_XRGB(0, 0, 255));
}

//-----------------------------------------------------------------------------
Sphere::~Sphere()
{
	//ReleaseCOM(m_VertexBuffer);
	//ReleaseCOM(m_IndexBuffer);

	ReleaseCOM(mesh);
}

//-----------------------------------------------------------------------------
void Sphere::Create(IDirect3DDevice9* gd3dDevice)
{
	//m_material->setTexture(gd3dDevice, "stone2.dds");
	m_materialG->setTexture(gd3dDevice, "marble.bmp");
	m_materialP->setTexture(gd3dDevice, "marble.bmp");

	D3DXCreateEffectFromFileA(gd3dDevice, "Textured_Phong.fx", 0, 0, 0, 0, &Phong, 0);
	D3DXCreateEffectFromFileA(gd3dDevice, "Textured_Gouraud.fx", 0, 0, 0, 0, &Gouraud, 0);
	m_materialG->ConnectToEffect(Gouraud);
	m_materialP->ConnectToEffect(Phong);
	m_materialG->buildFX();
	m_materialP->buildFX();

	D3DXCreateSphere(gd3dDevice, m_radius, m_sliceCount, m_stackCount, &mesh, 0);

	//buildVertexBuffer(gd3dDevice);
	//buildIndexBuffer(gd3dDevice);
	setTexCoord(gd3dDevice);
}

//-----------------------------------------------------------------------------
void Sphere::RenderPhong(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection, boolean specularOn, boolean diffuseOn, boolean textureOn, float normalStrength)
{
	// Update the statistics singlton class
	GfxStats::GetInstance()->addVertices(m_verticeCount);
	GfxStats::GetInstance()->addTriangles(m_triangleCount);//how many triangles are there?

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
		HR(gd3dDevice->SetRenderState(D3DRS_WRAP0, D3DWRAP_U));

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
		//HR(gd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_verticeCount, 0, m_triangleCount));
		HR(mesh->DrawSubset(0));

		HR(gd3dDevice->SetRenderState(D3DRS_WRAP0, 0));
		HR(Phong->EndPass());
	}
	HR(Phong->End());
}

void Sphere::RenderGouraud(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection, boolean specularOn, boolean diffuseOn, boolean textureOn)
{
	// Update the statistics singlton class
	GfxStats::GetInstance()->addVertices(m_verticeCount);
	GfxStats::GetInstance()->addTriangles(m_triangleCount);//how many triangles are there?

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
		HR(gd3dDevice->SetRenderState(D3DRS_WRAP0, D3DWRAP_U));

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
		//HR(gd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_verticeCount, 0, m_triangleCount));
		HR(mesh->DrawSubset(0));

		HR(gd3dDevice->SetRenderState(D3DRS_WRAP0, 0));
		HR(Gouraud->EndPass());
	}
	HR(Gouraud->End());
}

//-----------------------------------------------------------------------------
void Sphere::Update(D3DXVECTOR3 lightPos, D3DXVECTOR3 viewPos)
{
	m_materialG->Update(lightPos, viewPos);
	m_materialP->Update(lightPos, viewPos);
}

//-----------------------------------------------------------------------------
void Sphere::buildVertexBuffer(IDirect3DDevice9* gd3dDevice)
{
	int ringCount = m_stackCount + 1;
	float dPhi = float(PI / m_stackCount);
	float dTheta = (float)(2.0f * PI / m_sliceCount);
	int vertexCount = 0;
	// Obtain a pointer to a new vertex buffer.
	HR(gd3dDevice->CreateVertexBuffer(m_verticeCount * sizeof(VertexPos), D3DUSAGE_WRITEONLY,
		0, D3DPOOL_MANAGED, &m_VertexBuffer, 0));

	// Now lock it to obtain a pointer to its internal data, and write the
	// sphere's vertex data.

	VertexPos* v = 0;
	HR(m_VertexBuffer->Lock(0, 0, (void**)&v, 0));
	float verticleAngle = 0;
	//compute vertices for wall

	v[0] = VertexPos(0, m_radius, 0, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f);
	vertexCount++;

	for (int i = 1; i < m_stackCount; i++)
	{
		
		float y = cos(verticleAngle)*m_radius;
		float r = y * tan(verticleAngle);

		//ring vertices
		for (int j = 0; j <= m_sliceCount; j++)
		{
			float c = cosf(j*dTheta);
			float s = sinf(j*dTheta);

			float U = float((j*dTheta) / 2 * PI);
			float V = float((i*dPhi) / PI);

			D3DXVECTOR3 p(r*c, y, r*s);
			D3DXVECTOR3 *n = new D3DXVECTOR3();
			D3DXVec3Normalize(n, &p);

			float tx = -m_radius*sinf(i*dPhi)*sinf(j*dTheta);
			float ty = 0.0f;
			float tz = m_radius*sinf(i*dPhi)*cosf(j*dTheta);
			v[vertexCount] = VertexPos(r*c, y, r*s, n->x, n->y, n->z, U, V, tx, ty, tz);
			vertexCount++;
		}
		verticleAngle += (float)(2 * PI / ringCount);
	}

	v[vertexCount] = VertexPos(0, -m_radius, 0, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f);
	vertexCount++;

	HR(m_VertexBuffer->Unlock());
}

//-----------------------------------------------------------------------------
void Sphere::buildIndexBuffer(IDirect3DDevice9* gd3dDevice)
{
	int baseIndex = 1;
	int ringVertexCount = m_sliceCount + 1;

	int bottomPoleIndex = m_verticeCount - 2;

	// Obtain a pointer to a new index buffer.
	HR(gd3dDevice->CreateIndexBuffer(m_indiceCount * sizeof(WORD), D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_IndexBuffer, 0));

	// Now lock it to obtain a pointer to its internal data, and write the
	// sphere's index data.

	WORD* k = 0;
	int indexCount = 0;

	HR(m_IndexBuffer->Lock(0, 0, (void**)&k, 0));

	//top of sphere
	for (int i = 1; i <= m_sliceCount; i++)
	{
		k[indexCount] = 0;
		k[indexCount + 1] = i + 1;
		k[indexCount + 2] = i;
		indexCount += 3;
	}

	for (int i = 0; i < m_stackCount - 2; i++)
	{
		for (int j = 0; j < m_sliceCount; j++)
		{
			
			k[indexCount] = baseIndex + i*ringVertexCount + j;
			k[indexCount + 1] = baseIndex + i * ringVertexCount + j + 1;
			k[indexCount + 2] = baseIndex + (i + 1) * ringVertexCount + j;

			k[indexCount + 3] = baseIndex + (i + 1) * ringVertexCount + j;
			k[indexCount + 4] = baseIndex + i * ringVertexCount + j + 1;
			k[indexCount + 5] = baseIndex + (i + 1) * ringVertexCount + j + 1;

			indexCount += 6;
		}
	}

	baseIndex = bottomPoleIndex - ringVertexCount;
	//bottom of sphere

	for (int i = 0; i < m_sliceCount; i++)
	{
		k[indexCount] = bottomPoleIndex;
		k[indexCount + 1] = baseIndex + i;
		k[indexCount + 2] = baseIndex + i + 1;
		indexCount += 3;
	}

	HR(m_IndexBuffer->Unlock());
}

void Sphere::setTexCoord(IDirect3DDevice9* gd3dDevice)
{
	D3DVERTEXELEMENT9 elements[64];
	UINT numElements = 0;
	VertexPos::Decl->GetDeclaration(elements, &numElements);

	ID3DXMesh* temp = 0;
	HR(mesh->CloneMesh(D3DXMESH_SYSTEMMEM,
		elements, gd3dDevice, &temp));

	ReleaseCOM(mesh);

	// Now generate texture coordinates for each vertex.
	VertexPos* vertices = 0;
	HR(temp->LockVertexBuffer(0, (void**)&vertices));

	for (UINT i = 0; i < temp->GetNumVertices(); ++i)
	{
		// Convert to spherical coordinates.
		D3DXVECTOR3 p = vertices[i].pos;

		float theta = atan2f(p.z, p.x);
		float phi = acosf(p.y / sqrtf(p.x*p.x + p.y*p.y + p.z*p.z));

		// Phi and theta give the texture coordinates, but are not in 
		// the range [0, 1], so scale them into that range.

		float u = theta / (2.0f*D3DX_PI);
		float v = phi / D3DX_PI;

		// Save texture coordinates.

		vertices[i].tex0.x = u;
		vertices[i].tex0.y = v;
	}
	HR(temp->UnlockVertexBuffer());

	// Clone back to a hardware mesh.
	HR(temp->CloneMesh(D3DXMESH_MANAGED | D3DXMESH_WRITEONLY,
		elements, gd3dDevice, &mesh));

	ReleaseCOM(temp);
}