#include "SkyBox.h"

const int CSkyBox::NUMFACES = 6;

CSkyBox::CSkyBox()
{
  for (int q=0; q < NUMFACES; q++) {
    m_Texture[q] = NULL;
  }
	m_fSize = 10.0f;
}

CSkyBox::~CSkyBox()
{
  InvalidateDeviceObjects();
}

HRESULT CSkyBox::RestoreDeviceObjects(LPDIRECT3DDEVICE8 pDev, 
  const char *strTopTextureFilename,
  const char *strBottomTextureFilename,
  const char *strFrontTextureFilename,
  const char *strBackTextureFilename,
  const char *strLeftTextureFilename,
  const char *strRightTextureFilename)
{
  HRESULT hr;
  m_pd3dDevice = pDev;

  // create textures

	  if (FAILED(hr = D3DXCreateTextureFromFileEx(m_pd3dDevice, 
    strTopTextureFilename, D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &m_Texture[Top]))) {
    return(hr);
  }
  
  if (FAILED(hr = D3DXCreateTextureFromFileEx(m_pd3dDevice, 
    strBottomTextureFilename, D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &m_Texture[Bottom]))) {
    return(hr);
  }

  if (FAILED(hr = D3DXCreateTextureFromFileEx(m_pd3dDevice, 
    strLeftTextureFilename, D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &m_Texture[Left]))) {
    return(hr);
  }
  
  if (FAILED(hr = D3DXCreateTextureFromFileEx(m_pd3dDevice, 
    strRightTextureFilename, D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &m_Texture[Right]))) {
    return(hr);
  }
  
  if (FAILED(hr = D3DXCreateTextureFromFileEx(m_pd3dDevice, 
    strFrontTextureFilename, D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &m_Texture[Front]))) {
    return(hr);
  }
  
  if (FAILED(hr = D3DXCreateTextureFromFileEx(m_pd3dDevice, 
    strBackTextureFilename, D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &m_Texture[Back]))) {
    return(hr);
  }
  
  return S_OK;
}

void CSkyBox::InvalidateDeviceObjects()
{
  for (int q=0; q < NUMFACES; q++) {
    SAFE_RELEASE(m_Texture[q]);
  }
}

void CSkyBox::DeleteDeviceObjects()
{
}

void CSkyBox::Render(D3DXMATRIX &matView)
{
  float f;
  VERTEX_XYZ_DIFFUSE_TEX1 vert[4];

	D3DXMATRIX mat = matView;
  mat._41 = mat._42 = mat._43 = 0.0f;
  m_pd3dDevice->SetTransform(D3DTS_VIEW, &mat);

	D3DSURFACE_DESC desc;
	m_Texture[Front]->GetLevelDesc(0, &desc);

  f = 0.5f / (float)(desc.Width);

  vert[0].tu = 0.0f + f; vert[0].tv = 0.0f + f;
  vert[1].tu = 0.0f + f; vert[1].tv = 1.0f - f;
  vert[2].tu = 1.0f - f; vert[2].tv = 0.0f + f;
  vert[3].tu = 1.0f - f; vert[3].tv = 1.0f - f;

  m_pd3dDevice->SetVertexShader(D3DFVF_XYZ_DIFFUSE_TEX1);
  f = m_fSize * 0.5f;


  // left (negative x)
  vert[0].position = D3DXVECTOR3(-f,  f,  f);
  vert[1].position = D3DXVECTOR3(-f, -f,  f);
  vert[2].position = D3DXVECTOR3(-f,  f, -f);
  vert[3].position = D3DXVECTOR3(-f, -f, -f);

  m_pd3dDevice->SetTexture(0, m_Texture[Left]);
  m_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, (LPVOID) vert, sizeof(VERTEX_XYZ_DIFFUSE_TEX1));


  // right (positive x)
  vert[0].position = D3DXVECTOR3( f,  f, -f);
  vert[1].position = D3DXVECTOR3( f, -f, -f);
  vert[2].position = D3DXVECTOR3( f,  f,  f);
  vert[3].position = D3DXVECTOR3( f, -f,  f);

  m_pd3dDevice->SetTexture(0, m_Texture[Right]);
  m_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, (LPVOID) vert, sizeof(VERTEX_XYZ_DIFFUSE_TEX1));


  // down (negative y)
  vert[1].position = D3DXVECTOR3(-f, -f, -f);
  vert[3].position = D3DXVECTOR3(-f, -f,  f);
  vert[0].position = D3DXVECTOR3( f, -f, -f);
  vert[2].position = D3DXVECTOR3( f, -f,  f);

  m_pd3dDevice->SetTexture(0, m_Texture[Bottom]);
  m_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, (LPVOID) vert, sizeof(VERTEX_XYZ_DIFFUSE_TEX1));

  
  // up (positive y)
  vert[0].position = D3DXVECTOR3(-f,  f,  f);
  vert[1].position = D3DXVECTOR3(-f,  f, -f);
  vert[2].position = D3DXVECTOR3( f,  f,  f);
  vert[3].position = D3DXVECTOR3( f,  f, -f);

  m_pd3dDevice->SetTexture(0, m_Texture[Top]);
  m_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, (LPVOID) vert, sizeof(VERTEX_XYZ_DIFFUSE_TEX1));

  // back (negative z)
  vert[0].position = D3DXVECTOR3(-f,  f, -f);
  vert[1].position = D3DXVECTOR3(-f, -f, -f);
  vert[2].position = D3DXVECTOR3( f,  f, -f);
  vert[3].position = D3DXVECTOR3( f, -f, -f);

  m_pd3dDevice->SetTexture(0, m_Texture[Back]);
  m_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, (LPVOID) vert, sizeof(VERTEX_XYZ_DIFFUSE_TEX1));

  // front (positive z)
  vert[0].position = D3DXVECTOR3( f,  f,  f);
  vert[1].position = D3DXVECTOR3( f, -f,  f);
  vert[2].position = D3DXVECTOR3(-f,  f,  f);
  vert[3].position = D3DXVECTOR3(-f, -f,  f);

  m_pd3dDevice->SetTexture(0, m_Texture[Front]);
  m_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, (LPVOID) vert, sizeof(VERTEX_XYZ_DIFFUSE_TEX1));
}
  
