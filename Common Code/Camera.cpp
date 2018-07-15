/*
#############################################################################

  CAMERA.CPP - Implementation of a simple user-controlled camera.


  Release: 1.00 on 4/8/00 by Mason

#############################################################################
*/

// Include Directives ///////////////////////////////////////////////////////
#include "Camera.h"

// Local Constants, namespace usage /////////////////////////////////////////

// Local Variables / functions //////////////////////////////////////////////

CUserControlledCamera::CUserControlledCamera()
{
  m_vVelocity.x = 0.0f;
  m_vVelocity.y = 0.0f;
  m_vVelocity.z = 0.0f;
  m_fYaw = 0.0f;
  m_fYawVelocity = 0.0f;
  m_fPitch = 0.0f;
  m_fPitchVelocity = 0.0f;
  m_fRoll = 0.0f;
  m_fRollVelocity = 0.0f;
  m_vPosition = D3DXVECTOR3(0.0f,0.0f,0.0f);
  D3DXMatrixIdentity(&m_matOrientation);
}

CUserControlledCamera::~CUserControlledCamera()
{
}

void CUserControlledCamera::Update(float fElapsedTime)
{
  FLOAT fTime;
  
  if( fElapsedTime > 0.0f ) fTime = fElapsedTime;
  else                      fTime = 0.05f;

  FLOAT fSpeed        = 3.0f*fTime;
  FLOAT fAngularSpeed = 1.0f*fTime;
  
  
  // Update the position vector
  D3DXVECTOR3 vT = m_vVelocity * fSpeed;
  D3DXVec3TransformNormal( &vT, &vT, &m_matOrientation );
  m_vPosition += vT;
  
  // Update the yaw-pitch-rotation vector
  m_fYaw   += fAngularSpeed * m_fYawVelocity;
  m_fPitch += fAngularSpeed * m_fPitchVelocity;
  
  // Set the view matrix
  D3DXQUATERNION qR;
  D3DXQuaternionRotationYawPitchRoll(&qR, m_fYaw, m_fPitch, m_fRoll);
  D3DXMatrixAffineTransformation(&m_matOrientation, 1.25f, NULL, &qR, &m_vPosition );
  D3DXMatrixInverse( &m_matView, NULL, &m_matOrientation );

  // Decelerate only the camera's position velocity (for smooth motion)
  m_vVelocity      *= 0.9f;

  // Deceleating the yaw/pitch/roll velocities works great for keyboard control, 
  // but it gives me motion sickness when used with mouselook, so I commented it out
  // and replaced it with multiplication by zero, which disables the velocities.  
  // Your mileage may vary.

  /*
  
  m_fYawVelocity   *= 0.9f;
  m_fPitchVelocity *= 0.9f;
  */
  
  
  m_fYawVelocity   *= 0.0f;
  m_fPitchVelocity *= 0.0f;


}

void CUserControlledCamera::AddToYawPitchRoll(float fYaw, float fPitch, float fRoll)
{
  m_fPitchVelocity += fPitch;
  m_fYawVelocity += fYaw;
  m_fRollVelocity += fRoll;
}

void CUserControlledCamera::AddToVelocity(D3DXVECTOR3 &add)
{
  m_vVelocity += add;
}



