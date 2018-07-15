#include "Sprite.h"

CSprite::CSprite()
{
  m_pAnim = NULL;
  m_fSize = 1.0f;
	m_fRotationYaw = m_fRotationPitch = m_fRotationRoll = 0.0f;
}

CSprite::CSprite(CAnimSequence *anim)
{
  m_pAnim = anim;
  m_fSize = 1.0f;
}

CSprite::~CSprite()
{
}

void CSprite::Render(D3DXMATRIX view)
{
  if (!m_pAnim) return;

  // Set up a rotation matrix to orient the explo sprite towards the camera.
  D3DXMATRIX matScale, matTranslate, matTranspose, matBillboardRot, matWorld;
	D3DXMATRIX matRot; // user-specified rotation
  D3DXMatrixTranspose(&matTranspose, &view);
  D3DXMatrixIdentity(&matBillboardRot);
  
  matBillboardRot._11 = matTranspose._11;
  matBillboardRot._12 = matTranspose._12;
  matBillboardRot._13 = matTranspose._13;
  matBillboardRot._21 = matTranspose._21;
  matBillboardRot._22 = matTranspose._22;
  matBillboardRot._23 = matTranspose._23;
  matBillboardRot._31 = matTranspose._31;
  matBillboardRot._32 = matTranspose._32;
  matBillboardRot._33 = matTranspose._33;
      
  D3DXMatrixTranslation(&matWorld, m_vPos.x, m_vPos.y, m_vPos.z);
	D3DXMatrixRotationYawPitchRoll(&matRot, m_fRotationYaw, m_fRotationPitch, m_fRotationRoll);

  D3DXMatrixScaling(&matScale, m_fSize, m_fSize, 1);
  m_pAnim->Render(m_Timer, matScale * matRot * matBillboardRot * matWorld);
}