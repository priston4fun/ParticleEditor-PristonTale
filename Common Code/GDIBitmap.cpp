/*
#############################################################################

  GDIBitmap.cpp - implementation of CGDIBitmap, a class that abstracts a GDI
  DC and a GDI bitmap into a single entity.

  Automatically creates and deletes DCs and bitmaps as needed.

#############################################################################
*/

// Include Directives ///////////////////////////////////////////////////////
#include <stdio.h>

#include "GDIBitmap.h"

/****************************************************************************

 CGDIBitmap: constructor / destructor

 ****************************************************************************/
CGDIBitmap::CGDIBitmap()
{
  Init();
}

CGDIBitmap::~CGDIBitmap()
{
	// if the m_hdcBitmap has not been UnInited, do so
	if(m_hdcBitmap) UnInit();
}

/****************************************************************************

 Init: initializes this object.  called by the constructor.

 ****************************************************************************/
void CGDIBitmap::Init(void)
{
	// initialize all members to NULL or 0
	m_hdcBitmap=NULL;
	m_hBitmap=NULL;
	m_hOldBitmap=NULL;
	m_iWidth=0;
	m_iHeight=0;
}

/****************************************************************************

 UnInit: uninitializes this object.  called by destructor.

 ****************************************************************************/
void CGDIBitmap::UnInit()
{
  if (m_hdcBitmap == NULL) return;

	// restore old bitmap
	SelectObject(m_hdcBitmap,m_hOldBitmap);

	// delete new bitmap
	DeleteObject(m_hBitmap);

	// delete device context
	DeleteDC(m_hdcBitmap);
}

/****************************************************************************

 Load: creates a bitmap and DC compatible with the given DC, then loads a 
 BMP file onto the bitmap. 

 ****************************************************************************/
void CGDIBitmap::Load(HDC hdcCompatible, LPCTSTR lpszFilename)
{
	// if we've already got a bitmap loaded, UnInit it so we can load a new
  // one.
	if(m_hdcBitmap) UnInit();

	// create the memory dc
	m_hdcBitmap = CreateCompatibleDC(hdcCompatible);

	// load the BMP file
	m_hBitmap = static_cast<HBITMAP>(LoadImage(
    NULL,lpszFilename,IMAGE_BITMAP,0,0,LR_LOADFROMFILE));

  // if the load failed, throw an error.
  if (m_hBitmap == NULL) {
    char debugmsg[512];
    char errormsg[128];
    ConvertLastErrorToString(errormsg, sizeof(errormsg));
    sprintf(debugmsg, "Error while trying to load \"%s\": %s", 
      lpszFilename, errormsg);
    ::MessageBox(NULL, debugmsg, "Bitmap Loader", MB_ICONSTOP);
  }

	// select our new bitmap into the dc
	m_hOldBitmap = static_cast<HBITMAP>(SelectObject(m_hdcBitmap,m_hBitmap));

	// query the bitmap's properties
	BITMAP bmp;
	GetObject(m_hBitmap,sizeof(BITMAP),(LPVOID)&bmp);

	// assign height and width from those properties
	m_iWidth=bmp.bmWidth;
	m_iHeight=bmp.bmHeight;
}

/****************************************************************************

 CreateBlank: creates a blank bitmap of the given width/height, and a DC 
 that's compatible with the given DC.

 ****************************************************************************/
void CGDIBitmap::CreateBlank(HDC hdcCompatible, int width, int height)
{
	// if we've already got a bitmap loaded, UnInit it so we can load a new
  // one.
	if(m_hdcBitmap) UnInit();

	// create the memory dc
	m_hdcBitmap=CreateCompatibleDC(hdcCompatible);

	// create the image
	m_hBitmap=CreateCompatibleBitmap(hdcCompatible,width,height);

	// select the image into the dc
	m_hOldBitmap=(HBITMAP)SelectObject(m_hdcBitmap,m_hBitmap);

	// assign height and width from those properties
	m_iWidth=width;
	m_iHeight=height;
}

