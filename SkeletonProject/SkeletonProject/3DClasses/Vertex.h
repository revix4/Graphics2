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
	VertexPos() :pos(0.0f, 0.0f, 0.0f), color(D3DCOLOR_XRGB(0,0,0)),
		texCoord(0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f){}

	VertexPos(float x, float y, float z, D3DCOLOR c, float u, float v, float nX, float nY, float nZ) :
		pos(x, y, z), color(c),
		texCoord(u, v), normal(nX, nY, nZ){}

	VertexPos(const D3DXVECTOR3& v, const D3DCOLOR& c, const D3DXVECTOR2& t, const D3DXVECTOR3& n) :
		pos(v), color(c),
		texCoord(t), normal(n){}

	D3DXVECTOR3 pos;
	D3DCOLOR color;
	D3DXVECTOR2 texCoord;
	D3DXVECTOR3 normal;

	static IDirect3DVertexDeclaration9* Decl;
};
//=============================================================================
#endif // VERTEX_H