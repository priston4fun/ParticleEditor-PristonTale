#include "AnimSequence.h"

CAnimSequence::CAnimSequence(LPDIRECT3DDEVICE8 pDev)
{
  m_pVBQuad = NULL;
  m_pd3dDevice = pDev;
  CreateQuad(&m_pVBQuad, D3DPOOL_DEFAULT, 1.0f, 
    D3DCOLOR_ARGB(255,255,255,255), pDev);
}

bool CAnimSequence::AddFrame(const char *strFilename, float fTime)
{
  if (!m_pd3dDevice) return(false); 
  if (fTime <= 0) return(false);

  CAnimFrame *newframe = new CAnimFrame();
  
  // create a texture for this frame
  if (FAILED(D3DXCreateTextureFromFile(m_pd3dDevice, 
             strFilename, &newframe->m_pTexture))) {
    delete newframe;
    return(false);
  }
  
  // add to vector
  newframe->m_fTime = fTime;
  m_vFrames.push_back(newframe);
  return(true);
}

void CAnimSequence::DeleteFrame(int index)
{
  delete *(m_vFrames.begin() + index);
  m_vFrames.erase(m_vFrames.begin() + index);
}

void CAnimSequence::ClearAllFrames()
{
  for (std::vector<CAnimFrame *>::iterator i = m_vFrames.begin(); 
       i != m_vFrames.end(); ++i) {
    delete *i;
  }
  m_vFrames.clear();
}

void CAnimSequence::Render(CTimer &timer, D3DXMATRIX mat)
{
  if (!m_pd3dDevice) return; 
  if (timer.GetTime() < 0) return;

	int iCurFrame = GetCurFrame(timer);

  if (iCurFrame >= m_vFrames.size()) return;

  CAnimFrame *f = m_vFrames[iCurFrame];

  m_pd3dDevice->SetTransform(D3DTS_WORLD, &mat);
  m_pd3dDevice->SetTexture(0, f->m_pTexture);
  m_pd3dDevice->SetVertexShader(D3DFVF_XYZ_DIFFUSE_TEX1);
  m_pd3dDevice->SetStreamSource(0, m_pVBQuad, sizeof(VERTEX_XYZ_DIFFUSE_TEX1));
  m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
}

int CAnimSequence::GetCurFrame(CTimer &timer)
{
  int iCurFrame = 0; float fTimeCount = 0.0f;
  for (std::vector<CAnimFrame *>::iterator i = m_vFrames.begin(); i != m_vFrames.end(); ++i) {
    fTimeCount += (*i)->m_fTime;
    if (timer.GetTime() <= fTimeCount) break;
    iCurFrame++;
  }
	return(iCurFrame);
}
