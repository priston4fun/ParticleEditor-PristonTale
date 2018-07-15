#ifndef CH19P1_PARTICLEEVENT_H_INCLUDED
#define CH19P1_PARTICLEEVENT_H_INCLUDED

#include "CParticle.h"
#include "CParticleEmitterTokenizer.h"

class CParticleEvent
{
public:
  CParticleEvent() { m_NextFadeEvent = NULL; }
  virtual ~CParticleEvent() { }

  virtual void RestoreDeviceObjects(LPDIRECT3DDEVICE8 pDev) { }
  virtual void InvalidateDeviceObjects() { }

  //bool operator<(CParticleEvent &rhs) { return(m_ActualTime < rhs.m_ActualTime); }
  
  CMinMax<float> GetTimeRange(void) const { return(m_TimeRange); }
  void SetTimeRange(const CMinMax<float> data) { m_TimeRange = data; }

  float GetActualTime(void) const { return(m_ActualTime); }
  void SetActualTime(const float data) { m_ActualTime = data; }

  bool IsFade(void) const { return(m_bFade); }
  void SetFade(const bool data = true) { m_bFade = data; }
    
  virtual void DoItToIt(CParticle &part) = 0;
  virtual bool FadeAllowed() = 0;

  virtual bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
                                  std::vector<CParticleEmitterToken>::iterator &EndIter) = 0;

  static void ProcessPropEqualsValue(CMinMax<float> &prop,
    std::vector<CParticleEmitterToken>::iterator &TokenIter, 
    std::vector<CParticleEmitterToken>::iterator &EndIter);

  static void ProcessPropEqualsValue(CMinMax<D3DXVECTOR3> &prop,
    std::vector<CParticleEmitterToken>::iterator &TokenIter, 
    std::vector<CParticleEmitterToken>::iterator &EndIter);

  static void ProcessPropEqualsValue(CMinMax<D3DXCOLOR> &prop,
    std::vector<CParticleEmitterToken>::iterator &TokenIter, 
    std::vector<CParticleEmitterToken>::iterator &EndIter);

  CParticleEvent *m_NextFadeEvent;
protected:
  
  CMinMax<float> m_TimeRange;
  float m_ActualTime;
  bool m_bFade;
};

class CParticleEvent_CompareFunc : public std::binary_function<CParticleEvent *, CParticleEvent *, bool> {
public:
  bool operator() (const CParticleEvent* lhs, const CParticleEvent* rhs) const {
    return lhs->GetActualTime() < rhs->GetActualTime();
  }
};

class CParticleEvent_Size : public CParticleEvent
{
public:
  bool FadeAllowed() { return(true); }
  void DoItToIt(CParticle &part);
  bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
                          std::vector<CParticleEmitterToken>::iterator &EndIter);

  CMinMax<float> GetSize(void) const { return(m_Size); }
  void SetSize(const CMinMax<float> data) { m_Size = data; }

protected:
  CMinMax<float> m_Size; 
  
};

class CParticleEvent_Color : public CParticleEvent
{
public:
  bool FadeAllowed() { return(true); }
  void DoItToIt(CParticle &part);
  bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
                          std::vector<CParticleEmitterToken>::iterator &EndIter);

  CMinMax<D3DXCOLOR> GetColor(void) const { return(m_Color); }
  void SetColor(const CMinMax<D3DXCOLOR> &data) { m_Color = data; }

protected:
  CMinMax<D3DXCOLOR> m_Color;
};

class CParticleEvent_RedColor : public CParticleEvent
{
public:
  bool FadeAllowed() { return(true); }
  void DoItToIt(CParticle &part);
  bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
                          std::vector<CParticleEmitterToken>::iterator &EndIter);

  CMinMax<float> GetRedColor(void) const { return(m_RedColor); }
  void SetRedColor(const CMinMax<float> data) { m_RedColor = data; }

protected:
  CMinMax<float> m_RedColor;

};

class CParticleEvent_GreenColor : public CParticleEvent
{
public:
  bool FadeAllowed() { return(true); }
  void DoItToIt(CParticle &part);
  bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
                          std::vector<CParticleEmitterToken>::iterator &EndIter);

  CMinMax<float> GetGreenColor(void) const { return(m_GreenColor); }
  void SetGreenColor(const CMinMax<float> data) { m_GreenColor = data; }


protected:
  CMinMax<float> m_GreenColor;


};

class CParticleEvent_BlueColor : public CParticleEvent
{
public:
  bool FadeAllowed() { return(true); }
  void DoItToIt(CParticle &part);
  bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
                          std::vector<CParticleEmitterToken>::iterator &EndIter);

  CMinMax<float> GetBlueColor(void) const { return(m_BlueColor); }
  void SetBlueColor(const CMinMax<float> data) { m_BlueColor = data; }

protected:
  CMinMax<float> m_BlueColor;
};

class CParticleEvent_Alpha : public CParticleEvent
{
public:
  bool FadeAllowed() { return(true); }
  void DoItToIt(CParticle &part);
  bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
                          std::vector<CParticleEmitterToken>::iterator &EndIter);

  CMinMax<float> GetAlpha(void) const { return(m_Alpha); }
  void SetAlpha(const CMinMax<float> data) { m_Alpha = data; }


protected:
  CMinMax<float> m_Alpha;
};

class CParticleEvent_Velocity : public CParticleEvent
{
public:
  bool FadeAllowed() { return(true); }
  void DoItToIt(CParticle &part);
  bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
                          std::vector<CParticleEmitterToken>::iterator &EndIter);

  CMinMax<D3DXVECTOR3> GetVelocity(void) const { return(m_Velocity); }
  void SetVelocity(const CMinMax<D3DXVECTOR3> &data) { m_Velocity = data; }

protected:
  CMinMax<D3DXVECTOR3> m_Velocity;
};

class CParticleEvent_VelocityX : public CParticleEvent
{
public:
  bool FadeAllowed() { return(true); }
  void DoItToIt(CParticle &part);
  bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
                          std::vector<CParticleEmitterToken>::iterator &EndIter);

  CMinMax<float> GetValue(void) const { return(m_VelocityX); }
  void SetValue(const CMinMax<float> &data) { m_VelocityX = data; }

protected:
  CMinMax<float> m_VelocityX;
};

class CParticleEvent_VelocityY : public CParticleEvent
{
public:
  bool FadeAllowed() { return(true); }
  void DoItToIt(CParticle &part);
  bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
                          std::vector<CParticleEmitterToken>::iterator &EndIter);

  CMinMax<float> GetValue(void) const { return(m_VelocityY); }
  void SetValue(const CMinMax<float> &data) { m_VelocityY = data; }

protected:
  CMinMax<float> m_VelocityY;
};

class CParticleEvent_VelocityZ : public CParticleEvent
{
public:
  bool FadeAllowed() { return(true); }
  void DoItToIt(CParticle &part);
  bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
                          std::vector<CParticleEmitterToken>::iterator &EndIter);

  CMinMax<float> GetValue(void) const { return(m_VelocityZ); }
  void SetValue(const CMinMax<float> &data) { m_VelocityZ = data; }

protected:
  CMinMax<float> m_VelocityZ;
};

class CParticleEvent_EventTimer : public CParticleEvent
{
public:
  bool FadeAllowed() { return(false); }
  void DoItToIt(CParticle &part);
  bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
                          std::vector<CParticleEmitterToken>::iterator &EndIter);

  CMinMax<float> GetEventTimer(void) const { return(m_EventTimer); }
  void SetEventTimer(const CMinMax<float> &data) { m_EventTimer = data; }

protected:
  CMinMax<float> m_EventTimer;
};

#endif