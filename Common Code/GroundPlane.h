#include <D3DX8.h>
#include "D3DHelperFuncs.h"
#include "CommonFuncs.h"

class CGroundPlane
{
public:

  CGroundPlane();
  ~CGroundPlane();

  HRESULT RestoreDeviceObjects(
    LPDIRECT3DDEVICE8 pDev, const char *strTextureFilename,
    float fWidth, float fHeight, int iNumGridSquares);
  void InvalidateDeviceObjects();
  void DeleteDeviceObjects();

  LPDIRECT3DTEXTURE8 GetTexture() { return(m_Texture); }
  LPDIRECT3DVERTEXBUFFER8 GetVB() { return(m_pvbGround); }
  LPDIRECT3DINDEXBUFFER8 GetIB()  { return(m_pibGround); }
  int GetNumVerts() { return(m_iNumVerts); }
  int GetNumIndices() { return(m_iNumIndices); }
  D3DXPLANE GetPlane() { return(m_Plane); }

private:
  LPDIRECT3DDEVICE8 m_pd3dDevice;
  LPDIRECT3DTEXTURE8 m_Texture;
  LPDIRECT3DVERTEXBUFFER8 m_pvbGround;
  LPDIRECT3DINDEXBUFFER8 m_pibGround;
  int m_iNumVerts;
  int m_iNumIndices;
  D3DXPLANE m_Plane;

};