/*
#############################################################################

  D3DHelperFuncs.h - Mason's common D3D helper functions.

#############################################################################
*/

#ifndef D3DHELPERFUNCS_H
#define D3DHELPERFUNCS_H

#include <D3D8.h>
#include <D3DX8Math.h>
#include "CommonFuncs.h"

typedef struct 
{
  D3DXVECTOR3 position; // The position
  D3DCOLOR    color;    // The color
  FLOAT       tu, tv;   // The texture coordinates
} VERTEX_XYZ_DIFFUSE_TEX1;

// Our custom FVF, which describes the VERTEX_XYZ_DIFFUSE_TEX1 vertex type
#define D3DFVF_XYZ_DIFFUSE_TEX1 (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

// given a surface format, returns the stringified name of that surface 
char *SurfFormatToString(D3DFORMAT srcformat);

// create a flat quad of a given size and color.  Creates a 6 element
// triangle list VB, then fills it up with appropriate coords for a quad
// of size fSize.
HRESULT CreateQuad(LPDIRECT3DVERTEXBUFFER8 *pVB, D3DPOOL pool, float fSize, DWORD dwColor,
                   LPDIRECT3DDEVICE8 pDev, float fTexTileX = 1.0f, float fTexTileY = 1.0f);

// create a vertex grid of a given size and color.  this probably should use
// an index buffer.
HRESULT CreateVertexGrid(LPDIRECT3DVERTEXBUFFER8 *pVB, LPDIRECT3DINDEXBUFFER8 *pIB,
                         D3DPOOL pool, float fTotalSize, DWORD dwColor,
                         int iNumVerticesX, int iNumVerticesY, LPDIRECT3DDEVICE8 pDev);

// creates a "hollow" vertex grid (vertices only along the edges)
HRESULT CreateVertexFrame(LPDIRECT3DVERTEXBUFFER8 *pVB, LPDIRECT3DINDEXBUFFER8 *pIB,
                         D3DPOOL pool, float fTotalSize, DWORD dwColor, 
                         int iNumVerticesX, int iNumVerticesY, LPDIRECT3DDEVICE8 pDev);

// copy one texture from another - warning: kills the world, view, and 
// proj matrices, and the z-buffer state, stream 0, vertex shader and
// texture stage states.
HRESULT RenderCopy(LPDIRECT3DTEXTURE8 ptexSource, LPDIRECT3DTEXTURE8 ptexDest,
                   int iDestWidth, int iDestHeight, LPDIRECT3DDEVICE8 pDev,
                   LPDIRECT3DVERTEXBUFFER8 pvbQuad);

// same as render copy, but with bilinear filtering turned on.
HRESULT SmoothRenderCopy(LPDIRECT3DTEXTURE8 ptexSource, LPDIRECT3DTEXTURE8 ptexDest, 
                         int iDestWidth, int iDestHeight, LPDIRECT3DDEVICE8 pDev, 
                         LPDIRECT3DVERTEXBUFFER8 pvbQuad);

// sets up an ortho projection (world,view,proj mats) for render copying 
// a texture onto a texture with the specified width/height.
HRESULT SetupOrthoProjForRenderCopy(LPDIRECT3DDEVICE8 pDev, int iWidth, int iHeight);

// returns true if the given ray intersects the given triangle
bool DoesRayIntersectTriangle( const D3DXVECTOR3& orig,
                               const D3DXVECTOR3& dir, D3DXVECTOR3& v0,
                               D3DXVECTOR3& v1, D3DXVECTOR3& v2,
                               FLOAT* t, FLOAT* u, FLOAT* v );

// creates a vertex or pixel shader from a file on disk for the given device
// if pDecl == NULL, we assume it's a pixel shader... otherwise it's a vertex
// shader.
HRESULT CreateShader(LPDIRECT3DDEVICE8 pDev, const char *strFilename, 
                     DWORD *pDecl, DWORD &dwID);


inline DWORD D3DXColorTo255RGB(D3DXCOLOR &color)
{
  int r = (int)floor(color.r * 255.0f); if (r < 0) r = 0; if (r > 255) r = 255;
  int g = (int)floor(color.g * 255.0f); if (g < 0) g = 0; if (g > 255) g = 255;
  int b = (int)floor(color.b * 255.0f); if (b < 0) b = 0; if (b > 255) b = 255;
  return(RGB(r,g,b));
}

inline void D3DXColorTo255RGBA(D3DXCOLOR color, 
                                unsigned char &cr, unsigned char &cg, unsigned char &cb, unsigned char &ca)
{
  if (color.r > 1.0f) color.r = 1.0f; if (color.r < 0.0f) color.r = 0.0f;
  if (color.g > 1.0f) color.g = 1.0f; if (color.g < 0.0f) color.g = 0.0f;
  if (color.b > 1.0f) color.b = 1.0f; if (color.b < 0.0f) color.b = 0.0f;
  if (color.a > 1.0f) color.a = 1.0f; if (color.a < 0.0f) color.a = 0.0f;

  int r = (int)floor(color.r * 255.0f);
  int g = (int)floor(color.g * 255.0f);
  int b = (int)floor(color.b * 255.0f);
  int a = (int)floor(color.a * 255.0f);
  
  cr = (unsigned char)r;
  cg = (unsigned char)g;
  cb = (unsigned char)b;
  ca = (unsigned char)a;
}

inline D3DXCOLOR RGBAToD3DXColor(unsigned char cr, unsigned char cg, unsigned char cb, unsigned char ca)
{
  D3DXCOLOR c;
  c.r = (float)cr / 255.0f;
  c.g = (float)cg / 255.0f;
  c.b = (float)cb / 255.0f;
  c.a = (float)ca / 255.0f;
  return(c);
}

inline D3DXVECTOR3 RandomNumber(D3DXVECTOR3 vMin, D3DXVECTOR3 vMax)
{
  float x = RandomNumber(vMin.x, vMax.x);
  float y = RandomNumber(vMin.y, vMax.y);
  float z = RandomNumber(vMin.z, vMax.z);
  return(D3DXVECTOR3(x,y,z));
}

inline D3DXCOLOR RandomNumber(D3DXCOLOR Min, D3DXCOLOR Max)
{
  float r = RandomNumber(Min.r, Max.r);
  float g = RandomNumber(Min.g, Max.g);
  float b = RandomNumber(Min.b, Max.b);
  float a = RandomNumber(Min.a, Max.a);
  return(D3DXCOLOR(r,g,b,a));
}


#endif