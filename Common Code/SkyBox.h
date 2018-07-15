#include <D3DX8.h>
#include "D3DHelperFuncs.h"
#include "CommonFuncs.h"
#include "Camera.h"

class CSkyBox
{
public:

  static const int NUMFACES;

  CSkyBox();
  ~CSkyBox();

  HRESULT RestoreDeviceObjects(
    LPDIRECT3DDEVICE8 pDev, 
    const char *strTopTextureFilename,
    const char *strBottomTextureFilename,
    const char *strFrontTextureFilename,
    const char *strBackTextureFilename,
    const char *strLeftTextureFilename,
    const char *strRightTextureFilename);

  void InvalidateDeviceObjects();
  void DeleteDeviceObjects();

  enum BoxFace {
    Top = 0,
    Bottom,
    Left,
    Right,
    Front,
    Back
  };

  LPDIRECT3DTEXTURE8 GetTexture(BoxFace face) { return(m_Texture[face]); }
  
  void Render(D3DXMATRIX &matView);

	float GetSize(void) const { return(m_fSize); }
	void SetSize(const float data) { m_fSize = data; }
	  
private:
	float m_fSize;
  LPDIRECT3DDEVICE8 m_pd3dDevice;
  LPDIRECT3DTEXTURE8 m_Texture[6];
};