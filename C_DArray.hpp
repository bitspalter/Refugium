//////////////////////////////////////////////////////////////////////////////////
// [ DArray_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
// [DYNAMIC ARRAY CLASS]
//
// Last modi: 07.08.07 L.ey (�~)
//
#ifndef _C_DARRAY_H_
 #define _C_DARRAY_H_

  //#include <windows.h>

  #include "C_Array.hpp"

//////////////////////////////////////////////////////////////////////////////////

 const DWORD C_DARRAY_READY = 0x00000001;
 const DWORD C_DARRAY_ERROR = 0x00000000;
 
//////////////////////////////////////////////////////////////////////////////////

 struct S_C_DArray
 {
    S_C_DArray* _pNext;
    S_C_DArray* _pPrev;
    C_Array*    _pData;
 };

//////////////////////////////////////////////////////////////////////////////////

 class C_DArray
 {
   public:

     C_DArray();
     C_DArray(const C_DArray&);
    ~C_DArray();  
     C_DArray& operator=(const C_DArray&);

     S_C_DArray*  _Add_Item();
     C_Array*     _Add_Item(DWORD nItem, DWORD cItem);

     S_C_DArray*  _Get_pItem(DWORD Offset) const;

     DWORD  _Del_Item(DWORD Offset);
     DWORD  _Clear();

     DWORD  _Get_nItems() const {return(_nItems);}

   private:
     
     DWORD _nItems;

     S_C_DArray* _pHead;
     S_C_DArray* _pTail;
 };

#endif // _C_DARRAY_H_
