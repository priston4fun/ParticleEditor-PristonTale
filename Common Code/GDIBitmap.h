/*
#############################################################################

  GDIBitmap.h - declaration of CGDIBitmap, a class that abstracts a GDI
  DC and a GDI bitmap into a single entity.

  Automatically creates and deletes DCs and bitmaps as needed.

#############################################################################
*/

#if !defined(AFX_GDIBITMAP_H__9CA4EBA8_127B_4703_ACBD_025D1226D2A5__INCLUDED_)
#define AFX_GDIBITMAP_H__9CA4EBA8_127B_4703_ACBD_025D1226D2A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

#include "CommonFuncs.h"

class CGDIBitmap  
{
public:
	CGDIBitmap();
	virtual ~CGDIBitmap();

  // properties
  int GetWidth(void) const { return(m_iWidth); }
  int GetHeight(void) const { return(m_iHeight); }
  HDC GetBitmapDC(void) const { return(m_hdcBitmap); }

  // methods
  void Load(HDC hdcCompatible, LPCTSTR lpszFilename);
  void CreateBlank(HDC hdcCompatible, int width, int height);

private:

  void Init(void);
  void UnInit(void);

  HDC m_hdcBitmap;      // the handle of the DC that's attached to our bitmap
	HBITMAP m_hBitmap;    // handle to our bitmap
	HBITMAP m_hOldBitmap; // handle to the 1x1 mono bitmap (old selected one)
  int m_iWidth;         // width of our bitmap, in pixels
	int m_iHeight;        // height of our bitmap, in pixels
};

#endif // !defined(AFX_GDIBITMAP_H__9CA4EBA8_127B_4703_ACBD_025D1226D2A5__INCLUDED_)
