//////////////////////////////////////////////////////////////////////////////////
// [ AES_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
// Last modi: 06.08.07 L.ey (µ~)
//
#ifndef _C_AES_H_
 #define _C_AES_H_

   #include <iostream>

   using namespace std;

   #include "C_Array.hpp"

   #include "Rijndael.hpp"
  
   static const int C_AES_ERROR = 0x00;
   static const int C_AES_READY = 0x01;

//////////////////////////////////////////////////////////////////////////////////

 class C_AES {
   public:

     C_AES(){};
    ~C_AES(){};  

     int encrypt(char* pInput, unsigned long cInput, C_Array* pAOutput);
     int decrypt(char* pInput, unsigned long cInput, C_Array* pAOutput);

     CRijndael C_Rijndael;

   private:

 };

#endif // _C_AES_H_


