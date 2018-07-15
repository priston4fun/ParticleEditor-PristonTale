#ifndef CAMERA_H__INCLUDED
#define CAMERA_H__INCLUDED

#include <D3DX8.h>

class CCamera
{
public:
  CCamera() { }
  virtual ~CCamera() { }
  D3DXMATRIX SetViewMatrix(D3DXMATRIX &mat) { m_matView = mat; }
  D3DXMATRIX &GetViewMatrix() { return(m_matView); }

	void SetPosition(D3DXVECTOR3 &pos) { m_vPosition = pos; }
  D3DXVECTOR3 GetPosition() { return(m_vPosition); }
  
  virtual void Update(float fElapsedTime) = 0;

protected:
  D3DXMATRIX m_matView;
  D3DXMATRIX m_matOrientation;
	D3DXVECTOR3 m_vPosition;
};

class CUserControlledCamera : public CCamera
{
public:
  CUserControlledCamera();
  virtual ~CUserControlledCamera();

  void AddToYawPitchRoll(float fYaw, float fPitch, float fRoll);
  void AddToVelocity(D3DXVECTOR3 &add);

  void Update(float fElapsedTime);

protected:

  
  D3DXVECTOR3 m_vVelocity;
  float m_fYaw;
  float m_fYawVelocity;
  float m_fPitch;
  float m_fPitchVelocity;
  float m_fRoll;
  float m_fRollVelocity;
  



};
#endif