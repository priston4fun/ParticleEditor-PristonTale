#include "InputManager.h"

CInputManager::CInputManager()
{
  m_pDI = NULL; m_pMouse = NULL;
}

CInputManager::~CInputManager()
{
  DestroyDevices();
}

void CInputManager::DestroyDevices()
{
  SAFE_UNACQUIRE(m_pMouse);
  SAFE_RELEASE(m_pMouse);
  SAFE_RELEASE(m_pDI);
}

HRESULT CInputManager::ReadMouse(DIMOUSESTATE2 &state)
{
  HRESULT hr;
  ZeroMemory(&state, sizeof(state));

  if(NULL == m_pMouse) return S_OK;
  
  // Get the input's device state, and put the state in dims
  hr = m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE2), &state);
  if(FAILED(hr)) 
  {
    // DirectInput may be telling us that the input stream has been
    // interrupted.  We aren't tracking any state between polls, so
    // we don't have any special reset that needs to be done.
    // We just re-acquire and try again.
    
    // If input is lost then acquire and keep trying 
    hr = m_pMouse->Acquire();
    while(hr == DIERR_INPUTLOST) hr = m_pMouse->Acquire();

    // hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
    // may occur when the app is minimized or in the process of 
    // switching, so just try again later 
  }
  return S_OK;
}

HRESULT CInputManager::CreateDevices(HWND hwnd, 
                                     bool bExclusive, 
                                     bool bForeground)
{
  HRESULT hr;
  DWORD   dwCoopFlags;

  // Cleanup any previous call first
  DestroyDevices();
  
  if( bExclusive )  dwCoopFlags = DISCL_EXCLUSIVE;
  else              dwCoopFlags = DISCL_NONEXCLUSIVE;

  if( bForeground ) dwCoopFlags |= DISCL_FOREGROUND;
  else              dwCoopFlags |= DISCL_BACKGROUND;

  // Create a DInput object
  if(FAILED(hr = DirectInput8Create(GetModuleHandle(NULL), 
    DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&m_pDI, NULL)))
    return hr;

  // create mouse device
  {
    // Obtain an interface to the system mouse device.
    if(FAILED(hr = m_pDI->CreateDevice(GUID_SysMouse, &m_pMouse, NULL)))
      return hr;

    // Set the data format to "mouse format" - a predefined data format 
    //
    // A data format specifies which controls on a device we
    // are interested in, and how they should be reported.
    //
    // This tells DirectInput that we will be passing a
    // DIMOUSESTATE2 structure to IDirectInputDevice::GetDeviceState.
    if(FAILED(hr = m_pMouse->SetDataFormat(&c_dfDIMouse2)))
      return hr;

    // Set the cooperativity level to let DirectInput know how
    // this device should interact with the system and with other
    // DirectInput applications.
    hr = m_pMouse->SetCooperativeLevel(hwnd, dwCoopFlags );
    if(FAILED(hr)) return hr;

    // Acquire the newly created device
    m_pMouse->Acquire();
  }

  return S_OK;

}

