//=============================================================================
//              Adopted from Franc Luna's CubeDemo sample project.
//
// Please use this file to add classes / inherit and create various vertex classes
//=============================================================================
#ifndef VERTEX_H
#define VERTEX_H
//=============================================================================
#include <d3dx9.h>
//=============================================================================
// Call in constructor and destructor, respectively, of derived application class.
void InitAllVertexDeclarations();
void DestroyAllVertexDeclarations();
//=============================================================================
struct VertexPos
{
	VertexPos()
		:pos(0.0f, 0.0f, 0.0f),
		normal(0.0f, 0.0f, 0.0f),
		tex0(0.0f, 0.0f),
		tan(0.0f, 0.0f, 0.0f),
		biNorm(0.0f, 0.0f, 0.0f){}
	VertexPos(float x, float y, float z,
		float nx, float ny, float nz,
		float u, float v,
		float tanx, float tany, float tanz
		/*float bix, float biy, float biz*/) :pos(x, y, z), normal(nx, ny, nz), tex0(u, v), tan(1.0f, 1.0f, 1.0f), biNorm(1.0f, 1.0f, 1.0f){}
	VertexPos(const D3DXVECTOR3& v, const D3DXVECTOR3& n, const D3DXVECTOR2& uv, const D3DXVECTOR3& tangent, const D3DXVECTOR3& bi)
		:pos(v), normal(n), tex0(uv), tan(tangent), biNorm(bi){}

	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 tex0;
	D3DXVECTOR3 tan;
	D3DXVECTOR3 biNorm;

	static IDirect3DVertexDeclaration9* Decl;
};
//=============================================================================
#endif // VERTEX_H