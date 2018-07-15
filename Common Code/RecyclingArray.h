#include <stdexcept>
#include <vector>
#include "CommonFuncs.h"

template <class TArrayElement, int iNumElements> class CRecyclingArray 
{
public:
  CRecyclingArray()
  {
    for(int q=0; q < iNumElements; q++) {
      m_bAlive[q] = false;
    }
    m_iUsedElements = 0;
    m_iLastNew = 0;
  }

  virtual ~CRecyclingArray() { }

  TArrayElement *New()
  {
    // assert that we have space for this one
    if (GetNumFreeElements() < 1) throw(std::out_of_range("CRecyclingArray::New: too many objects!"));

    // find first element not in use.  as an optimization, we start at
    // the position that was allocated last, in the hopes that the next position
    // will be free.

    int i = m_iLastNew;
    for (int q=0; q < iNumElements; q++) {
      if (!m_bAlive[i]) {
        // we've found our free spot!  use it!
        break;
      }
      else {
        i++; if (i >= iNumElements) i = 0;
      }
    }

    if (m_bAlive[i]) {
      // huh? if we got here, there are no free elements in the list... yet 
      // GetNumFreeElements didn't tell us that in the beginning.  Logic error.
      throw(std::logic_error("TArrayElement::New(): internal logic error."));
    }

    // clear it
    m_aElements[i] = m_NewTemplate;

    // increment used count
    m_iUsedElements++;
    m_bAlive[i] = true;
    m_iLastNew = i;
    
    // return it
    return(&m_aElements[i]);
  }

  bool Delete(int index)
  {
    if (index < 0 || index >= iNumElements || !m_bAlive[index]) return false;
    // don't actually delete element at index;
    // just mark it free.
    m_bAlive[index] = false;
    m_iUsedElements--;
    return true;
  }

  bool Delete(TArrayElement *elem)
  {
    if (m_iUsedElements == 0) return(false);
    int iElem = reinterpret_cast<int>(elem);
    iElem -= reinterpret_cast<int>(&m_aElements[0]);
    int index = iElem / sizeof(TArrayElement);
    if (index < 0 || index >= iNumElements || !m_bAlive[index]) return(false);
    m_bAlive[index] = false;
    m_iUsedElements--;
    return(true);
  }

  int GetNumFreeElements(void) { return(iNumElements-GetNumUsedElements()); }
  int GetNumUsedElements(void) { return(m_iUsedElements); }

  int GetTotalElements(void) { return(iNumElements); }

  TArrayElement &GetAt(int index) { return(m_aElements[index]); }
  bool IsAlive(int index) { return(m_bAlive[index]); }

  void DeleteAll(void)
  {
    for (int q=0; q < iNumElements; q++) {
      m_bAlive[q] = false;
    }
    m_iUsedElements = 0;
    m_iLastNew = 0;
  }

protected:
  int m_iUsedElements;
  TArrayElement m_aElements[iNumElements]; 
  bool m_bAlive[iNumElements];
  int m_iLastNew;
  TArrayElement m_NewTemplate;

};

