#include <dinput.h>
#include <stdio.h>
#include "CommonFuncs.h"

class CInputManager
{
public:
  CInputManager();
  virtual ~CInputManager();

  HRESULT CreateDevices(HWND hwnd, bool bExclusive, bool bForeground);
  void DestroyDevices();

  HRESULT ReadMouse(DIMOUSESTATE2 &state);

protected:
  LPDIRECTINPUT8 m_pDI;         
  LPDIRECTINPUTDEVICE8 m_pMouse;     
};