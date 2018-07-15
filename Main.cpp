#include "stdafx.h"
#include "CParticleEmitter.h"

//-----------------------------------------------------------------------------
// Name: class CMyD3DApplication
// Desc: Application class. The base class (CD3DApplication) provides the 
//       generic functionality needed in all Direct3D samples. CMyD3DApplication 
//       adds functionality specific to this sample program.
//-----------------------------------------------------------------------------
class CMyD3DApplication : public CD3DApplication
{
  // Font for drawing text
  CD3DFont* m_pFont;
  CD3DFont* m_pFontSmall;

  // Scene
  CGroundPlane m_Ground;
  CUserControlledCamera m_Camera;

  // Mouse Input
  CInputManager m_InputManager;

  // The Particle System
  CParticleEmitter m_PartSys;

  // dialog box window handle
  HWND m_hWndPropDlg;

protected:
  HRESULT OneTimeSceneInit();
  HRESULT InitDeviceObjects();
  HRESULT RestoreDeviceObjects();
  HRESULT InvalidateDeviceObjects();
  HRESULT DeleteDeviceObjects();
  HRESULT FinalCleanup();
  HRESULT Render();
  HRESULT FrameMove();
  HRESULT ConfirmDevice( D3DCAPS8* pCaps, DWORD dwBehavior, D3DFORMAT Format );
  LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

  void ProcessInput();

public:
  CMyD3DApplication();

  // is the ground shown?
  bool m_bShowGround;
};

CMyD3DApplication* g_pD3DApp = NULL;

CParticleEmitter *g_theSystem = NULL;


void BrowseForColor(HWND hWnd, UINT idR, UINT idG, UINT idB, UINT idA)
{
  static COLORREF custcolors[16];

  CHOOSECOLOR cc;
  memset(&cc, 0, sizeof(cc));
  cc.lStructSize = sizeof(cc);
  cc.hwndOwner = hWnd;
  cc.Flags = CC_RGBINIT | CC_FULLOPEN;
  cc.lpCustColors = custcolors;

  // get current values, convert to 0-255 range
  /*
  D3DXCOLOR color = Stuff4EditsIntoColor(hWnd, idR, idG, idB, idA);
  cc.rgbResult = D3DXColorTo255RGB(color);

  if (ChooseColor(&cc)) {
    color.r = (float)GetRValue(cc.rgbResult) / 255.0f;
    color.g = (float)GetGValue(cc.rgbResult) / 255.0f;
    color.b = (float)GetBValue(cc.rgbResult) / 255.0f;
    StuffColorInto4Edits(color, hWnd, idR, idG, idB, idA);
  }
  */
}

std::string GetEditbox(HWND hWnd, UINT id)
{
  std::string str;
  int len = GetWindowTextLength(GetDlgItem(hWnd, id));
  try {
    char *text = new char[len+1]; // +1 so 0 length files will work
    GetWindowText(GetDlgItem(hWnd, id), text, len+1);
    str = text;
    delete[] text;
  } catch(...) { }
  return(str);
}
bool SaveEditbox(HWND hWnd, UINT id, const char *strFilename)
{
  int len = GetWindowTextLength(GetDlgItem(hWnd, id));
  try {
    char *text = new char[len+1]; // +1 so 0 length files will save
    GetWindowText(GetDlgItem(hWnd, id), text, len+1);
    int handle = open(strFilename, O_RDWR | O_BINARY | O_TRUNC | O_CREAT, S_IWRITE | S_IREAD);
    if (handle == -1) { throw("Cannot open file for output!"); }
    if (write(handle, text, len) != len) { throw("error writing file, probably disk full."); }
    close(handle);
    delete[] text;
  } 
  catch(char *e) { ::MessageBox(hWnd, e, "SaveEditbox function error catcher", MB_ICONSTOP); return(false); }
  catch(...) { return(false); }
  return(true);
}

bool LoadEditbox(HWND hWnd, UINT id, const char *strFilename)
{
  try {
    int handle = open(strFilename, O_RDONLY | O_BINARY);
    if (handle == -1) { throw("Cannot open file for input!"); }

    int len = filelength(handle);
  
    char *text = new char[len+1]; // +1 so 0 length files will save
    memset(text, 0, len+1);
    read(handle, text, len);
    close(handle);

    SetWindowText(GetDlgItem(hWnd, id), text);
    
    delete[] text;
  } 
  catch(char *e) { ::MessageBox(hWnd, e, "LoadEditbox function error catcher", MB_ICONSTOP); return(false); }
  catch(...) { return(false); }
  
  return(true);
}

LRESULT CALLBACK PropDlgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
  CMyD3DApplication *app = (CMyD3DApplication *)g_pD3DApp;
  static bool bTurnOffUpdates = false;
  switch (uMsg) {
    case WM_INITDIALOG: 
      {
        bTurnOffUpdates = true; // so as we call ourselves we don't change things
        
        SendMessage(GetDlgItem(hWnd, IDC_SHOWGROUND), BM_SETCHECK, app->m_bShowGround, 0);

        bTurnOffUpdates = false; // now allow updates to go to part sys

        return(true); // focus not set
      }
      break;

    case WM_COMMAND:
      {
        if (bTurnOffUpdates) return(false);
        UINT wID = LOWORD(wParam);         // item, control, or accelerator identifier 
        HWND hwndCtl = (HWND) lParam;      // handle of control 
        
        switch(wID) {
          case IDC_SHOWGROUND:
            app->m_bShowGround = app->m_bShowGround ? false : true;
            break;

          case IDC_COMPILE:
            {
              std::string str = GetEditbox(hWnd, IDC_SCRIPT);
              g_theSystem->Compile(str.c_str());
							g_theSystem->Start();
              SetWindowText(GetDlgItem(hWnd, IDC_ERRORS), g_theSystem->m_strLastError.c_str());
            }
            break;

          case IDC_SAVE:
            {
              OPENFILENAME ofn;       // common dialog box structure
              char szFile[260];       // buffer for filename
              
              // Initialize OPENFILENAME
              ZeroMemory(&ofn, sizeof(OPENFILENAME));
              ofn.lStructSize = sizeof(OPENFILENAME);
              ofn.hwndOwner = hWnd;
              strcpy(szFile, "ParticleSystem.txt");
              ofn.lpstrFile = szFile;
              ofn.nMaxFile = sizeof(szFile);
              ofn.lpstrFilter = "Particle Files (*.part)\0*.part\0All Files (*.*)\0*.*\0\0";
              ofn.lpstrFileTitle = "Save Particle System Script";
              ofn.nMaxFileTitle = 0;
              ofn.lpstrInitialDir = NULL;
              ofn.Flags = OFN_OVERWRITEPROMPT;

              // Display the Save As dialog box. 

              if (GetSaveFileName(&ofn)==TRUE) {
                if (!SaveEditbox(hWnd, IDC_SCRIPT, ofn.lpstrFile)) {
                  ::MessageBox(hWnd, "Could not save file to disk (disk full?)", "Error Saving File", MB_ICONSTOP);
                } 
                else SetWindowText(hWnd, ofn.lpstrFile);
              }
            }
            break;

          case IDC_LOAD:
            {
              OPENFILENAME ofn;       // common dialog box structure
              char szFile[260];       // buffer for filename
              
              // Initialize OPENFILENAME
              ZeroMemory(&ofn, sizeof(OPENFILENAME));
              ofn.lStructSize = sizeof(OPENFILENAME);
              ofn.hwndOwner = hWnd;
              strcpy(szFile, "ParticleSystem.txt");
              ofn.lpstrFile = szFile;
              ofn.nMaxFile = sizeof(szFile);
              ofn.lpstrFilter = "Particle Files (*.part)\0*.part\0All Files (*.*)\0*.*\0\0";
              ofn.lpstrFileTitle = "Save Particle System Properties";
              ofn.nMaxFileTitle = 0;
              ofn.lpstrInitialDir = NULL;
              ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

              // Display the Open dialog box. 

              if (GetOpenFileName(&ofn)==TRUE) {
                if (!LoadEditbox(hWnd, IDC_SCRIPT, ofn.lpstrFile)) {
                  ::MessageBox(hWnd, "Could not load file from disk.", "Error Loading File", MB_ICONSTOP);
                } 
                else {
                  SetWindowText(hWnd, ofn.lpstrFile);
                  SendMessage(hWnd, WM_COMMAND, IDC_COMPILE, 0); // click Compile!
                }
              }
            }
            break;
        } // switch wID
      } // WM_COMMAND block
      break;
  } // switch uMsg
  return(0);
}

//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: Entry point to the program. Initializes everything, and goes into a
//       message-processing loop. Idle time is used to render the scene.
//-----------------------------------------------------------------------------
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
  CMyD3DApplication d3dApp;
  g_pD3DApp = &d3dApp;

  if( FAILED( d3dApp.Create( hInst ) ) )
    return 0;

  return d3dApp.Run();
}

//-----------------------------------------------------------------------------
// Name: CMyD3DApplication()
// Desc: Application constructor. Sets attributes for the app.
//-----------------------------------------------------------------------------
CMyD3DApplication::CMyD3DApplication()
{
  m_strWindowTitle    = _T("Particle Editor v1.0 - www.priston4fun.com");
  m_bUseDepthBuffer   = TRUE;

  m_pFont            = new CD3DFont( _T("Arial"), 12, D3DFONT_BOLD );
  m_pFontSmall       = new CD3DFont( _T("Arial"),  9, D3DFONT_BOLD );
}

//-----------------------------------------------------------------------------
// Name: OneTimeSceneInit()
// Desc: Called during initial app startup, this function performs all the
//       permanent initialization.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::OneTimeSceneInit()
{
  return S_OK;
}

//-----------------------------------------------------------------------------
// Name: FrameMove()
// Desc: Called once per frame, the call is the entry point for animating
//       the scene.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::FrameMove()
{
	ProcessInput();

	if (m_fElapsedTime < 0.001f) m_fElapsedTime = 0.001f;
  m_PartSys.Update(m_fTime, m_fElapsedTime);
  return S_OK;
}

void CMyD3DApplication::ProcessInput()
{
  const float fSpeed = 0.5f;
  unsigned char m_bKey[256];
  ZeroMemory( m_bKey, 256 );
  GetKeyboardState(m_bKey);
  // Process keyboard input
  if(m_bKey['D'] & 128) m_Camera.AddToVelocity(D3DXVECTOR3(fSpeed, 0.0f, 0.0f)); // Slide Right
  if(m_bKey['A'] & 128) m_Camera.AddToVelocity(D3DXVECTOR3(-fSpeed, 0.0f, 0.0f));// Slide Left
  if(m_bKey['Q'] & 128) m_Camera.AddToVelocity(D3DXVECTOR3(0.0f, fSpeed, 0.0f)); // Slide Up
  if(m_bKey['Z'] & 128) m_Camera.AddToVelocity(D3DXVECTOR3(0.0f, -fSpeed, 0.0f));// Slide Down
  if(m_bKey['W'] & 128) m_Camera.AddToVelocity(D3DXVECTOR3(0.0f, 0.0f, fSpeed)); // Slide Foward
  if(m_bKey['S'] & 128) m_Camera.AddToVelocity(D3DXVECTOR3(0.0f, 0.0f, -fSpeed));// Slide Back
  if(m_bKey['L'] & 128) m_Camera.AddToYawPitchRoll(fSpeed, 0.0f, 0.0f);  // Turn Right
  if(m_bKey['J'] & 128) m_Camera.AddToYawPitchRoll(-fSpeed, 0.0f, 0.0f); // Turn Left
  if(m_bKey['K'] & 128) m_Camera.AddToYawPitchRoll(0.0f, fSpeed, 0.0f); // Turn Down
  if(m_bKey['I'] & 128) m_Camera.AddToYawPitchRoll(0.0f, -fSpeed, 0.0f);// Turn Up

  // mouse look
  DIMOUSESTATE2 dims2;
  m_InputManager.ReadMouse(dims2);

  // play with the divisor constants to change the mouselook sensitivity.
  // I've found that these values most accurately simulate my beloved Q3A setup. :)
  m_Camera.AddToYawPitchRoll((float)dims2.lX/0.8f, (float)dims2.lY/0.8f, 0.0f);
  
}

//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Called once per frame, the call is the entry point for 3d
//       rendering. This function sets up render states, clears the
//       viewport, and renders the scene.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::Render()
{
  
  // these are done here so that you can move the camera around during a freeze
  // frame, ala The Matrix

  //ProcessInput();
  m_Camera.Update(m_fElapsedTime);

  m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
  m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
  m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

  // Clear the backbuffer
  m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
                       0x000000, 1.0f, 0L );
  
  // Now that our fire texture's updated, we can begin rendering the scene
  if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
  {
    if (m_bShowGround) {
      // draw our quad
      m_pd3dDevice->SetTransform( D3DTS_VIEW, &m_Camera.GetViewMatrix());
      m_pd3dDevice->SetTexture( 0, m_Ground.GetTexture() );
      m_pd3dDevice->SetVertexShader( D3DFVF_XYZ_DIFFUSE_TEX1 );
      m_pd3dDevice->SetStreamSource( 0, m_Ground.GetVB(), sizeof(VERTEX_XYZ_DIFFUSE_TEX1) );
      m_pd3dDevice->SetIndices( m_Ground.GetIB(), 0L );
      m_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 
        m_Ground.GetNumVerts(), 0, m_Ground.GetNumIndices()/3 );
    }

    // setup alpha blending states
    m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    
    // render the particle system
    m_PartSys.Render();

    char buf[256];
    _snprintf(buf, 256, "Particles: %d, FPS: %0.2f", 
      m_PartSys.GetNumActiveParticles(), m_fFPS);

    m_pFontSmall->DrawText( 2,  0, D3DCOLOR_ARGB(255,255,255,0), buf );

    // End the scene.
    m_pd3dDevice->EndScene();
  }

  return S_OK;
}

//-----------------------------------------------------------------------------
// Name: InitDeviceObjects()
// Desc: Initialize scene objects.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::InitDeviceObjects()
{
  m_pFont->InitDeviceObjects( m_pd3dDevice );
  m_pFontSmall->InitDeviceObjects( m_pd3dDevice );
  return S_OK;
}

//-----------------------------------------------------------------------------
// Name: RestoreDeviceObjects()
// Desc: Initialize scene objects.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::RestoreDeviceObjects()
{
  static bool bBeenHere = false;
  m_InputManager.CreateDevices(m_hWnd, false, true);

  m_pFont->RestoreDeviceObjects();
  m_pFontSmall->RestoreDeviceObjects();
  m_Camera.SetPosition(D3DXVECTOR3(0.0f, 2.0f, -5.0f));

  // create the ground plane
  m_Ground.RestoreDeviceObjects(m_pd3dDevice, "../Data/Ground.png", 256.0f, 256.0f, 8);

  // initialize the particle system
  if (!bBeenHere) {
    // first time here, set initial system properties
    m_PartSys.Compile(
      "ParticleSystem \"TestParticles\" 1.00 { "
      "  Position = XYZ(0,1,0) "
      " "
      "  EventSequence \"Orange Flame\" { "
      "    spawndir = XYZ(Random(1,2),Random(3,4),Random(5,6)) "
      "    emitradius = XYZ(Random(10,20),Random(30,40),50) "
      "    texture = \"Ch19p1_ParticleTexture.png\" " 
      "    sourceblendmode = D3DBLEND_SRCALPHA "
      "    destblendmode = D3DBLEND_DESTALPHA "
      "    emitrate = Random(10,20) "
      "    lifetime = Random(2,4) "
      "    initial velocity = XYZ(Random(0,1),Random(0,1),Random(0,1)) "
      "    initial size = Random(1.0, 10.0) "
      "    initial color = RGBA(Random(0,1),Random(0,1),Random(0,1),Random(0,1)) "
      "    fade so at 1.0 size = Random(5.0, 5.0) "
      "    fade so at 2.0 size = 3.0 "
      "    at 2.0 color = RGBA(Random(0,1),Random(0,1),Random(0,1),Random(0,1)) "
      "    fade so final size = 5.0 "
      "    final color = RGBA(0.0,0.0,0.0,0.0) "
      "  } "
      "} "
      );

  
    /*

    TODO:  make a script to do this

    m_PartSys.SetSpawnDir1(D3DXVECTOR3(-2.0f, -2.0f, -2.0f));
    m_PartSys.SetSpawnDir2(D3DXVECTOR3( 2.0f, 2.0f,  2.0f));
    m_PartSys.SetStartColor1(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
    m_PartSys.SetStartColor2(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
    m_PartSys.SetEndColor1(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
    m_PartSys.SetEndColor2(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
    m_PartSys.SetPos(D3DXVECTOR3(0.0f, 2.0f, 0.0f));
    m_PartSys.SetLifetime(CMinMax<float>(0.0f, 10.0f));
    m_PartSys.SetEmitRate(CMinMax<float>(10.0f, 20.0f));
    m_PartSys.SetTexture("Ch19p1_ParticleTexture.png");
    
    */
  }  

  bBeenHere = true;
  
  // initialize it!
  m_PartSys.RestoreDeviceObjects(m_pd3dDevice);

  // initialize the dialog
  {
    g_theSystem = &m_PartSys; // so dialog box can get at it.

    m_hWndPropDlg = CreateDialog(NULL, MAKEINTRESOURCE(IDD_PARTICLEPROPS), m_hWnd, 
      (DLGPROC) PropDlgProc); 

    ShowWindow(m_hWndPropDlg, SW_SHOW); 
  }

  // Set the world matrix
  D3DXMATRIX matWorld;
  D3DXMatrixIdentity( &matWorld );
  m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

  // Set projection matrix
  D3DXMATRIX matProj;
  FLOAT fAspect = ((FLOAT)m_d3dsdBackBuffer.Width) / m_d3dsdBackBuffer.Height;
  D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, fAspect, 0.1f, 100.0f );
  m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

  m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
  m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
  m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
  m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
  m_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
  m_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );

  m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE );
  m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
  m_pd3dDevice->SetRenderState( D3DRS_LIGHTING,  FALSE );
  m_pd3dDevice->SetRenderState( D3DRS_CULLMODE,  D3DCULL_CCW );
  m_pd3dDevice->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_FLAT );

  return S_OK;
}

//-----------------------------------------------------------------------------
// Name: InvalidateDeviceObjects()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::InvalidateDeviceObjects()
{
  DestroyWindow(m_hWndPropDlg);

  m_InputManager.DestroyDevices();

  m_pFont->InvalidateDeviceObjects();
  m_pFontSmall->InvalidateDeviceObjects();
  
  m_Ground.InvalidateDeviceObjects();
  m_PartSys.InvalidateDeviceObjects();
  return S_OK;
}

//-----------------------------------------------------------------------------
// Name: DeleteDeviceObjects()
// Desc: Called when the app is exiting, or the device is being changed,
//       this function deletes any device dependent objects.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::DeleteDeviceObjects()
{
  m_pFont->DeleteDeviceObjects();
  m_pFontSmall->DeleteDeviceObjects();
  
  m_Ground.DeleteDeviceObjects();
  return S_OK;
}

//-----------------------------------------------------------------------------
// Name: FinalCleanup()
// Desc: Called before the app exits, this function gives the app the chance
//       to cleanup after itself.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::FinalCleanup()
{
  SAFE_DELETE( m_pFont );
  SAFE_DELETE( m_pFontSmall );
  return S_OK;
}

//-----------------------------------------------------------------------------
// Name: ConfirmDevice()
// Desc: Called during device intialization, this code checks the device
//       for some minimum set of capabilities
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::ConfirmDevice( D3DCAPS8* pCaps, DWORD dwBehavior,
                                          D3DFORMAT Format )
{
  return S_OK;
}

//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: Message proc function to handle key and menu input
//-----------------------------------------------------------------------------
LRESULT CMyD3DApplication::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam,
                                    LPARAM lParam )
{
  // Pass remaining messages to default handler
  return CD3DApplication::MsgProc( hWnd, uMsg, wParam, lParam );
}