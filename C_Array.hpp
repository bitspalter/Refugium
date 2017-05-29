//////////////////////////////////////////////////////////////////////////////////
// [ Array_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
//
// [STATIC ARRAY CLASS]
//
// Last modi: 07.08.07 L.ey (�~)
//
#ifndef _C_ARRAY_H_
 #define _C_ARRAY_H_

 #include <iostream>
 #include <string.h>
 
 using namespace std;

 typedef unsigned long DWORD;

//////////////////////////////////////////////////////////////////////////////////

 const DWORD C_ARRAY_STATUS_READY = 0x00000001;
 const DWORD C_ARRAY_STATUS_ERROR = 0x00000000;
 
 const DWORD C_ARRAY_SUB_PTR      = 0x00000100;
 const DWORD C_ARRAY_SUB_EXIST    = 0x00000200;
 const DWORD C_ARRAY_SUB_OVER     = 0x00000300;

//////////////////////////////////////////////////////////////////////////////////

 class C_Array
 {
   public:

     C_Array();
     C_Array(DWORD nItem, DWORD cItem);
     C_Array(const C_Array&);
    ~C_Array(); 
     C_Array& operator=(const C_Array&);
 
     DWORD   _Create(DWORD nItem, DWORD cItem);
     DWORD   _Destroy();
 
     // (!) offset[0] = Item nr.1
     //
     DWORD   _Get_Item(DWORD offset, void* pDataDst); 
     DWORD   _Set_Item(DWORD offset, void* pDataSrc);

     char*   _Get_pItem(DWORD offset);

     //////////////////////////////////
     // Inline
     //
     DWORD   _Get_Status() const {return(_Status);}

     char*   _Get_pBuffer() const {return(_pBuffer);}
     DWORD   _Get_cBuffer() const {return(_cBuffer);}

     DWORD   _Get_nItem() const {return(_nItem);}
     DWORD   _Get_cItem() const {return(_cItem);}
   
   private:

     DWORD   _Status;

     char*   _pBuffer;    // DeltaPointer

     DWORD   _cBuffer;    // FullSize
     DWORD   _nItem;      // Item count
     DWORD   _cItem;      // Item size 
 };

#endif // _C_ARRAY_H_
