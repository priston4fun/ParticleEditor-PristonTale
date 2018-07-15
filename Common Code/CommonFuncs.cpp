/*
#############################################################################

  CommonFuncs.cpp - a library of common windows functions.

#############################################################################
*/
#include <windows.h>
#include <math.h>
#include <io.h>
#include <fcntl.h>
#include "CommonFuncs.h"

/****************************************************************************

 CenterWindow: centers the given window on the screen.

 ****************************************************************************/
void CenterWindow(HWND hwnd)
{
  // get the width and height of the screen
  int iScreenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
  int iScreenHeight= GetSystemMetrics(SM_CYFULLSCREEN);

  // get the rectangle of the window
  RECT rectWindow;
  GetWindowRect(hwnd, &rectWindow);

  // do the centering math
  int iDestX = (iScreenWidth-(rectWindow.right-rectWindow.left)) / 2;
  int iDestY = (iScreenHeight-(rectWindow.bottom-rectWindow.top)) / 2;

  // center the window
  MoveWindow(hwnd, iDestX, iDestY, 
    rectWindow.right-rectWindow.left,
    rectWindow.bottom-rectWindow.top,
    true);
}

/****************************************************************************

 LoadFileIntoString: loads a file into a STL string.

 ****************************************************************************/
bool LoadFileIntoString(const char *strFilename, std::string &str)
{
	int handle = open(strFilename, O_RDONLY | O_BINARY);
	if (handle == -1) return(false);

	int iFilesize = filelength(handle);
	str.reserve(iFilesize);

	int iPos = 0; str = ""; bool bSuccess = true;
	while (iPos < iFilesize) {
		char buf[1024];
		memset(buf, 0, sizeof(buf));
		int iBytesRead = read(handle, buf, sizeof(buf)-1);
		str += buf;
		iPos += iBytesRead;
		if (iBytesRead == 0) { bSuccess = false; break; }
	}
	close(handle);
	return(bSuccess);
}

/****************************************************************************

 ConvertLastErrorToString: calls GetLastError, then converts it to a string
 using FormatMessage.  Puts up to nMaxStrLen characters of the converted
 error string into szDest.

 ****************************************************************************/
void ConvertLastErrorToString(LPSTR szDest, int nMaxStrLen)
{
  LPVOID lpMsgBuf;
  FormatMessage( 
    FORMAT_MESSAGE_ALLOCATE_BUFFER | 
    FORMAT_MESSAGE_FROM_SYSTEM | 
    FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL,
    GetLastError(),
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
    (LPTSTR) &lpMsgBuf,
    0,
    NULL 
  );
  
  strncpy(szDest, reinterpret_cast<char *>(lpMsgBuf), nMaxStrLen);

  LocalFree( lpMsgBuf );
}

bool IsPowerOf2(int n) { return (!(n & (n - 1))); }

int GetLowestPowerOf2(int n)
{
  int lowest = 1;
  while(lowest < n) lowest <<= 1;
  return lowest;
}

/****************************************************************************

 RandomNumber: returns a random number between iMin and iMax.

 ****************************************************************************/
int RandomNumber(int iMin, int iMax)
{
  if (iMin == iMax) return(iMin);
  return((rand() % (abs(iMax-iMin)+1))+iMin);
}

float RandomNumber(float fMin, float fMax)
{
  if (fMin == fMax) return(fMin);
  float fRandom = (float)rand() / (float)RAND_MAX;
  return((fRandom * (float)fabs(fMax-fMin))+fMin);
}

/****************************************************************************
 *                                                                          *
 * PluckFirstField: pulls the first "field" off of str and puts it in dest  *
 * (uses delim as a delimiter)                                              *
 *                                                                          *
 ****************************************************************************/
void PluckFirstField(char *str, char *dest, int maxlen, 
                     const char *delim)
{
  char *endpos;
  int p;

  if (!strlen(delim)) { strcpy(dest, str); return; }
  endpos = strstr(str, delim);
  if (!endpos) { strcpy(dest, str); return; }
  p = endpos - str;

  memset(dest, 0, maxlen);
  memcpy(dest, str, p);

  // pluck it off of str...
  strcpy(str, &str[p+strlen(delim)]);
}