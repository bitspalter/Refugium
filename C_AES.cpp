//////////////////////////////////////////////////////////////////////////////////
// [ AES_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_AES.hpp"

//////////////////////////////////////////////////////////////////////////////////
// [_Encrypt]
//////////////////////////////////////////////////////////////////////////////////
int C_AES::encrypt(char* pInput, unsigned long cInput, C_Array* pAOutput){
   if(!pInput || !cInput || !pAOutput) return(C_AES_ERROR);

   unsigned long cRest   = cInput % 16;
   unsigned long cRounds = cInput / 16;
   unsigned long cData   = cInput;

   if(cRest){
      cRounds++;
      cData += (16 - cRest);
   }

   pAOutput->_Destroy();
   pAOutput->_Create(1, cData);

   char* pBuffer = pAOutput->_Get_pBuffer();

   C_Rijndael.Encrypt(pInput, pBuffer, cData, 1);

   return(C_AES_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [_Decrypt]
//////////////////////////////////////////////////////////////////////////////////
int C_AES::decrypt(char* pInput, unsigned long cInput, C_Array* pAOutput){
   if(!pInput || !cInput || !pAOutput) return(C_AES_ERROR);

   DWORD cRest   = cInput % 16;
   DWORD cRounds = cInput / 16;

   if(cRest) return(C_AES_ERROR);

   pAOutput->_Destroy();
   pAOutput->_Create(cRounds, 16);

   char* pBuffer = pAOutput->_Get_pBuffer();

   C_Rijndael.Decrypt(pInput, pBuffer, cInput, 1);

   return(C_AES_READY);
}
