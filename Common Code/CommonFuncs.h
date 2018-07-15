/*
#############################################################################

  CommonFuncs.h - a library of common windows functions.

#############################################################################
*/
#if !defined(AFX_COMMONFUNCS_H__DC47117F_2750_4C69_A954_184DB6487700__INCLUDED_)
#define AFX_COMMONFUNCS_H__DC47117F_2750_4C69_A954_184DB6487700__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <d3dx8.h>

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#define SAFE_UNACQUIRE(p)    { if(p) { (p)->Unacquire();         } }

#define PI 3.14159
#define TORADIANS(p) ((p)/180.0*PI)

// Helper function to stuff a FLOAT into a DWORD argument
inline DWORD FtoDW( FLOAT f ) { return *((DWORD*)&f); }

// stuffs a file into a string
extern bool LoadFileIntoString(const char *strFilename, std::string &str);

// centers a window on the screen
extern void CenterWindow(HWND hwnd); 

// gets the last error and converts it to a string.
extern void ConvertLastErrorToString(LPSTR szDest, int nMaxStrLen);

// returns true if the given number is a power of 2.
extern bool IsPowerOf2(int n);

// returns the lowest power of two >= the given number.
extern int GetLowestPowerOf2(int n);

// given a field delimiter, takes the first field off string and puts it in dest
extern void PluckFirstField(char *str, char *dest, int maxlen, const char *delim);


// returns a random number between min and max            
int RandomNumber(int iMin, int iMax);
float RandomNumber(float fMin, float fMax);

inline void MakeUpperCase(std::string &str) {
  for (std::string::iterator i = str.begin(); i != str.end(); i++) {
    *i = toupper(*i);
  }
}

inline std::string RemoveQuotes(std::string &str) {
  for (std::string::iterator i = str.begin(); i != str.end(); i++) {
    if (*i == '\"') {
      i = str.erase(i); if (i == str.end()) break;
    }
  }
  return(str);
}

/****************************************************************************

 Noise2D/Noise3D: random number generators, originally from Hugo Elias, and
 from Kim Pallister's Procedural Clouds demo.  These are used by the sample
 programs for Chapter 15.

 ****************************************************************************/
inline float Noise2D(int x, int y)
{
  int n = x + y * 57;
  n = (n<<13) ^ n;
  return ( 1.0f - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);    
}
 
inline float Noise3D(int x, int y, int z)
{
  int n = x + y * 57 + z * 131;
  n = (n<<13) ^ n;
  return ( 1.0f - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) 
	* 0.000000000931322574615478515625f);    
}

#endif