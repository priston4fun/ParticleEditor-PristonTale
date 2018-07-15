#ifndef ANIMSEQUENCE_H_INCLUDED
#define ANIMSEQUENCE_H_INCLUDED

#include <vector>
#include "CommonFuncs.h"
#include "D3DHelperFuncs.h"
#include "Timer.h"

class CAnimFrame
{
public:
  CAnimFrame() { m_pTexture = NULL; }
  virtual ~CAnimFrame() { SAFE_RELEASE(m_pTexture); }

  LPDIRECT3DTEXTURE8 m_pTexture;
  float m_fTime;
};

class CAnimSequence {
public:
  CAnimSequence(LPDIRECT3DDEVICE8 pDev);
  virtual ~CAnimSequence() { ClearAllFrames(); SAFE_RELEASE(m_pVBQuad); }
  
  bool AddFrame(const char *strFilename, float fTime);
  void DeleteFrame(int index);
  void ClearAllFrames();
  int GetCurFrame(CTimer &timer);
  LPDIRECT3DTEXTURE8 GetCurFrameTexture(CTimer &timer) {
    try {
      int iCurFrame = GetCurFrame(timer);
      return(m_vFrames[iCurFrame]->m_pTexture);
    } catch(...) { }
    return(NULL);
  }

  void Render(CTimer &timer, D3DXMATRIX mat);

protected:
  LPDIRECT3DVERTEXBUFFER8 m_pVBQuad;
  LPDIRECT3DDEVICE8 m_pd3dDevice;
  std::vector<CAnimFrame *> m_vFrames;  
};

#endif