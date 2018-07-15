#include "stdafx.h"
#include "CParticleEvent.h"

void CParticleEvent::ProcessPropEqualsValue(CMinMax<float> &prop,
    std::vector<CParticleEmitterToken>::iterator &TokenIter, 
    std::vector<CParticleEmitterToken>::iterator &EndIter)
{
  // next token should be =
  CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
  if (TokenIter->m_Type != Equals) throw("Expecting = after property.");

  // next token should be a number
  CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
  CParticleEmitterTokenizer::ProcessNumber(prop, TokenIter, EndIter);
}

void CParticleEvent::ProcessPropEqualsValue(CMinMax<D3DXVECTOR3> &prop,
    std::vector<CParticleEmitterToken>::iterator &TokenIter, 
    std::vector<CParticleEmitterToken>::iterator &EndIter)
{
  // next token should be =
  CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
  if (TokenIter->m_Type != Equals) throw("Expecting = after property.");

  // next token should be a number
  CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
  CParticleEmitterTokenizer::ProcessVector(prop, TokenIter, EndIter);
}

void CParticleEvent::ProcessPropEqualsValue(CMinMax<D3DXCOLOR> &prop,
    std::vector<CParticleEmitterToken>::iterator &TokenIter, 
    std::vector<CParticleEmitterToken>::iterator &EndIter)
{
  // next token should be =
  CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
  if (TokenIter->m_Type != Equals) throw("Expecting = after property.");

  // next token should be a number
  CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
  CParticleEmitterTokenizer::ProcessColor(prop, TokenIter, EndIter);
}

bool CParticleEvent_Size::ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
                                             std::vector<CParticleEmitterToken>::iterator &EndIter)
{
  if (TokenIter->m_strValue.find("SIZE") == std::string::npos) throw("Expecting Size!");
  ProcessPropEqualsValue(m_Size, TokenIter, EndIter);
  return(true);
}

bool CParticleEvent_Color::ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
                                             std::vector<CParticleEmitterToken>::iterator &EndIter)
{
  if (TokenIter->m_strValue.find("COLOR") == std::string::npos) throw("Expecting Color!");
  ProcessPropEqualsValue(m_Color, TokenIter, EndIter);
  return(true);
}

bool CParticleEvent_RedColor::ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
                                             std::vector<CParticleEmitterToken>::iterator &EndIter)
{
  if (TokenIter->m_strValue.find("REDCOLOR") == std::string::npos) throw("Expecting RedColor!");
  ProcessPropEqualsValue(m_RedColor, TokenIter, EndIter);
  return(true);
}

bool CParticleEvent_GreenColor::ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
                                             std::vector<CParticleEmitterToken>::iterator &EndIter)
{
  if (TokenIter->m_strValue.find("GREENCOLOR") == std::string::npos) throw("Expecting GreenColor!");
  ProcessPropEqualsValue(m_GreenColor, TokenIter, EndIter);
  return(true);
}

bool CParticleEvent_BlueColor::ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
                                             std::vector<CParticleEmitterToken>::iterator &EndIter)
{
  if (TokenIter->m_strValue.find("BLUECOLOR") == std::string::npos) throw("Expecting BlueColor!");
  ProcessPropEqualsValue(m_BlueColor, TokenIter, EndIter);
  return(true);
}

bool CParticleEvent_Alpha::ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
                                             std::vector<CParticleEmitterToken>::iterator &EndIter)
{
  if (TokenIter->m_strValue.find("ALPHA") == std::string::npos) throw("Expecting Alpha!");
  ProcessPropEqualsValue(m_Alpha, TokenIter, EndIter);
  return(true);
}

bool CParticleEvent_Velocity::ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
                                             std::vector<CParticleEmitterToken>::iterator &EndIter)
{
  if (TokenIter->m_strValue.find("VELOCITY") == std::string::npos) throw("Expecting Velocity!");
  ProcessPropEqualsValue(m_Velocity, TokenIter, EndIter);
  return(true);
}

bool CParticleEvent_VelocityX::ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
                                             std::vector<CParticleEmitterToken>::iterator &EndIter)
{
  if (TokenIter->m_strValue.find("VELOCITYX") == std::string::npos) throw("Expecting VelocityX!");
  ProcessPropEqualsValue(m_VelocityX, TokenIter, EndIter);
  return(true);
}

bool CParticleEvent_VelocityY::ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
                                             std::vector<CParticleEmitterToken>::iterator &EndIter)
{
  if (TokenIter->m_strValue.find("VelocityY") == std::string::npos) throw("Expecting VelocityY!");
  ProcessPropEqualsValue(m_VelocityY, TokenIter, EndIter);
  return(true);
}

bool CParticleEvent_VelocityZ::ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
                                             std::vector<CParticleEmitterToken>::iterator &EndIter)
{
  if (TokenIter->m_strValue.find("VelocityZ") == std::string::npos) throw("Expecting VelocityZ!");
  ProcessPropEqualsValue(m_VelocityZ, TokenIter, EndIter);
  return(true);
}

bool CParticleEvent_EventTimer::ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
                                             std::vector<CParticleEmitterToken>::iterator &EndIter)
{
  if (TokenIter->m_strValue.find("EVENTTIMER") == std::string::npos) throw("Expecting EventTimer!");
  ProcessPropEqualsValue(m_EventTimer, TokenIter, EndIter);
  return(true);
}

void CParticleEvent_Size::DoItToIt(CParticle &part)
{
  if (!IsFade()) part.m_fSize = m_Size.GetRandomNumInRange();
  if (m_NextFadeEvent) {
    float newvalue = static_cast<CParticleEvent_Size *>(m_NextFadeEvent)->GetSize().GetRandomNumInRange();
    float timedelta = m_NextFadeEvent->GetActualTime() - GetActualTime();
    if (timedelta == 0) timedelta = 1; // prevent divide by zero errors
    part.m_fSizeStep = newvalue - part.m_fSize / timedelta;
  }
}

void CParticleEvent_Color::DoItToIt(CParticle &part)
{
  if (!IsFade()) part.m_Color = m_Color.GetRandomNumInRange();
  if (m_NextFadeEvent) {
    D3DXCOLOR newcolor = static_cast<CParticleEvent_Color *>(m_NextFadeEvent)->GetColor().GetRandomNumInRange();
    float timedelta = m_NextFadeEvent->GetActualTime() - GetActualTime();
    if (timedelta == 0) timedelta = 1; // prevent divide by zero errors
    // calculate color deltas to get us to the next fade event.
    part.m_ColorStep.r = newcolor.r - part.m_Color.r / timedelta;
    part.m_ColorStep.g = newcolor.g - part.m_Color.g / timedelta;
    part.m_ColorStep.b = newcolor.b - part.m_Color.b / timedelta;
    part.m_ColorStep.a = newcolor.a - part.m_Color.a / timedelta;
  }
}

void CParticleEvent_RedColor::DoItToIt(CParticle &part)
{
  if (!IsFade()) part.m_Color.r = m_RedColor.GetRandomNumInRange();
  if (m_NextFadeEvent) {
    float newvalue = static_cast<CParticleEvent_RedColor *>(m_NextFadeEvent)->GetRedColor().GetRandomNumInRange();
    float timedelta = m_NextFadeEvent->GetActualTime() - GetActualTime();
    if (timedelta == 0) timedelta = 1; // prevent divide by zero errors
    part.m_ColorStep.r = newvalue - part.m_Color.r / timedelta;
  }

}

void CParticleEvent_GreenColor::DoItToIt(CParticle &part)
{
  if (!IsFade()) part.m_Color.g = m_GreenColor.GetRandomNumInRange();
  if (m_NextFadeEvent) {
    float newvalue = static_cast<CParticleEvent_GreenColor *>(m_NextFadeEvent)->GetGreenColor().GetRandomNumInRange();
    float timedelta = m_NextFadeEvent->GetActualTime() - GetActualTime();
    if (timedelta == 0) timedelta = 1; // prevent divide by zero errors
    part.m_ColorStep.g = newvalue - part.m_Color.g / timedelta;
  }
}

void CParticleEvent_BlueColor::DoItToIt(CParticle &part)
{
  if (!IsFade()) part.m_Color.b = m_BlueColor.GetRandomNumInRange();
  if (m_NextFadeEvent) {
    float newvalue = static_cast<CParticleEvent_BlueColor *>(m_NextFadeEvent)->GetBlueColor().GetRandomNumInRange();
    float timedelta = m_NextFadeEvent->GetActualTime() - GetActualTime();
    if (timedelta == 0) timedelta = 1; // prevent divide by zero errors
    part.m_ColorStep.b = newvalue - part.m_Color.b / timedelta;
  }
}

void CParticleEvent_Alpha::DoItToIt(CParticle &part)
{
  if (!IsFade()) part.m_Color.a = m_Alpha.GetRandomNumInRange();
  if (m_NextFadeEvent) {
    float newvalue = static_cast<CParticleEvent_Alpha *>(m_NextFadeEvent)->GetAlpha().GetRandomNumInRange();
    float timedelta = m_NextFadeEvent->GetActualTime() - GetActualTime();
    if (timedelta == 0) timedelta = 1; // prevent divide by zero errors
    part.m_ColorStep.a = newvalue - part.m_Color.a / timedelta;
  }
}

void CParticleEvent_Velocity::DoItToIt(CParticle &part)
{
  if (!IsFade()) part.m_vDir = m_Velocity.GetRandomNumInRange();
  if (m_NextFadeEvent) {
    D3DXVECTOR3 newvalue = static_cast<CParticleEvent_Velocity *>(m_NextFadeEvent)->GetVelocity().GetRandomNumInRange();
    float timedelta = m_NextFadeEvent->GetActualTime() - GetActualTime();
    if (timedelta == 0) timedelta = 1; // prevent divide by zero errors
    part.m_vDirStep.x = newvalue.x - part.m_vDir.x / timedelta;
    part.m_vDirStep.y = newvalue.y - part.m_vDir.y / timedelta;
    part.m_vDirStep.z = newvalue.z - part.m_vDir.z / timedelta;
  }
}

void CParticleEvent_VelocityX::DoItToIt(CParticle &part)
{
  if (!IsFade()) part.m_vDir.x = m_VelocityX.GetRandomNumInRange();
  if (m_NextFadeEvent) {
    float newvalue = static_cast<CParticleEvent_VelocityX *>(m_NextFadeEvent)->GetValue().GetRandomNumInRange();
    float timedelta = m_NextFadeEvent->GetActualTime() - GetActualTime();
    if (timedelta == 0) timedelta = 1; // prevent divide by zero errors
    part.m_vDirStep.x = newvalue - part.m_vDir.x / timedelta;
  }
}

void CParticleEvent_VelocityY::DoItToIt(CParticle &part)
{
  if (!IsFade()) part.m_vDir.y = m_VelocityY.GetRandomNumInRange();
  if (m_NextFadeEvent) {
    float newvalue = static_cast<CParticleEvent_VelocityY *>(m_NextFadeEvent)->GetValue().GetRandomNumInRange();
    float timedelta = m_NextFadeEvent->GetActualTime() - GetActualTime();
    if (timedelta == 0) timedelta = 1; // prevent divide by zero errors
    part.m_vDirStep.y = newvalue - part.m_vDir.y / timedelta;
  }
}

void CParticleEvent_VelocityZ::DoItToIt(CParticle &part)
{
  if (!IsFade()) part.m_vDir.z = m_VelocityZ.GetRandomNumInRange();
  if (m_NextFadeEvent) {
    float newvalue = static_cast<CParticleEvent_VelocityZ *>(m_NextFadeEvent)->GetValue().GetRandomNumInRange();
    float timedelta = m_NextFadeEvent->GetActualTime() - GetActualTime();
    if (timedelta == 0) timedelta = 1; // prevent divide by zero errors
    part.m_vDirStep.z = newvalue - part.m_vDir.z / timedelta;
  }
}


void CParticleEvent_EventTimer::DoItToIt(CParticle &part)
{
  part.m_fEventTimer = m_EventTimer.GetRandomNumInRange();
}