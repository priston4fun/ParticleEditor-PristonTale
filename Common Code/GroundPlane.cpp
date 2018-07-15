// include files ////////////////////////////////////////////////////////////
#define STRICT

#include "GroundPlane.h"

CGroundPlane::CGroundPlane() : m_Plane( 0.0f, 1.0f, 0.0f, 0.0f )
{
  m_iNumVerts = 0;
  m_iNumIndices = 0;
  m_Texture = NULL;
  m_pvbGround = NULL;
  m_pibGround = NULL;
}

CGroundPlane::~CGroundPlane()
{
}

HRESULT CGroundPlane::RestoreDeviceObjects(
    LPDIRECT3DDEVICE8 pDev, const char *strTextureFilename,
    float fWidth, float fHeight, int iNumGridSquares)
{
  HRESULT hr;
  m_pd3dDevice = pDev;

  // create texture.
  if (FAILED(hr = D3DXCreateTextureFromFile(m_pd3dDevice, 
    strTextureFilename, &m_Texture))) {
    return(hr);
  }
  
  // calculate the size of vertex/index buffers
  m_iNumVerts   = (iNumGridSquares+1)*(iNumGridSquares+1);
  m_iNumIndices = iNumGridSquares*iNumGridSquares*6;
    
  // Create vertex buffer for ground object
  hr = m_pd3dDevice->CreateVertexBuffer( m_iNumVerts*sizeof(VERTEX_XYZ_DIFFUSE_TEX1),
                                         D3DUSAGE_WRITEONLY, D3DFVF_XYZ_DIFFUSE_TEX1,
                                         D3DPOOL_MANAGED, &m_pvbGround );
  if(FAILED(hr)) return(hr);

  // Fill vertex buffer
  VERTEX_XYZ_DIFFUSE_TEX1* pVertices;
  if(FAILED(m_pvbGround->Lock(0, 0, (BYTE**)&pVertices, NULL))) return hr;

  // Fill in vertices
  for(int zz = 0; zz <= iNumGridSquares; zz++)
  {
    for(int xx = 0; xx <= iNumGridSquares; xx++)
    {
        pVertices->position.x   = fWidth * (xx/(FLOAT)iNumGridSquares-0.5f);
        pVertices->position.y   = 0.0f;
        pVertices->position.z   = fHeight * (zz/(FLOAT)iNumGridSquares-0.5f);
        pVertices->color        = D3DCOLOR_ARGB(255,255,255,255);
        pVertices->tu           = (xx % 2) ? 0.0f : 1.0f; 
        pVertices->tv           = (zz % 2) ? 0.0f : 1.0f;
        pVertices++;
    }
  }

  m_pvbGround->Unlock();

  // Create the index buffer
  WORD* pIndices;
  hr = m_pd3dDevice->CreateIndexBuffer( m_iNumIndices*sizeof(WORD),
                                        D3DUSAGE_WRITEONLY,
                                        D3DFMT_INDEX16, D3DPOOL_MANAGED,
                                        &m_pibGround );
  if( FAILED(hr) )
      return E_FAIL;

  // Fill the index buffer
  m_pibGround->Lock( 0, m_iNumIndices*sizeof(WORD), (BYTE**)&pIndices, 0 );
  if( FAILED(hr) ) return E_FAIL;

  // Fill in indices
  for( int z = 0; z < iNumGridSquares; z++ ) {
    for( int x = 0; x < iNumGridSquares; x++ ) {
      DWORD vtx = x + z * (iNumGridSquares+1);
      *pIndices++ = (WORD)( vtx + 1 );
      *pIndices++ = (WORD)( vtx + 0 );
      *pIndices++ = (WORD)( vtx + 0 + (iNumGridSquares+1) );
      *pIndices++ = (WORD)( vtx + 1 );
      *pIndices++ = (WORD)( vtx + 0 + (iNumGridSquares+1) );
      *pIndices++ = (WORD)( vtx + 1 + (iNumGridSquares+1) );
    }
  }

  m_pibGround->Unlock();
  return S_OK;
}

void CGroundPlane::InvalidateDeviceObjects()
{
  SAFE_RELEASE(m_Texture);
  SAFE_RELEASE(m_pvbGround);
  SAFE_RELEASE(m_pibGround);
}

void CGroundPlane::DeleteDeviceObjects()
{
}
