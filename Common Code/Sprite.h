#ifndef SPRITE_H_INCLUDED
#define SPRITE_H_INCLUDED

#include <D3DX8.h>
#include "AnimSequence.h"
#include "Timer.h"

class CSprite
{
public:
  CSprite();
  CSprite(CAnimSequence *anim);
  virtual ~CSprite();

  CTimer &Timer() { return(m_Timer); }
  void SetAnim(CAnimSequence *anim) { m_pAnim = anim; }
  CAnimSequence *GetAnim() { return(m_pAnim); }
  D3DXVECTOR3 &Pos() { return(m_vPos); }

  void Render(D3DXMATRIX view);

  void SetSize(float size) { m_fSize = size; }
  float GetSize(void) { return(m_fSize); }

  void SetRotationYaw(float fRadians) { m_fRotationYaw = fRadians; }
  void SetRotationPitch(float fRadians) { m_fRotationPitch = fRadians; }
  void SetRotationRoll(float fRadians) { m_fRotationRoll = fRadians; }

  float GetRotationYaw(void) { return(m_fRotationYaw); }
  float GetRotationPitch(void) { return(m_fRotationPitch); }
  float GetRotationRoll(void) { return(m_fRotationRoll); }
  
private:
  float m_fSize;
  D3DXVECTOR3 m_vPos;
  
  float m_fRotationYaw, m_fRotationPitch, m_fRotationRoll;

  CTimer m_Timer;
  CAnimSequence *m_pAnim;
};

#endif