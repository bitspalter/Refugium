//////////////////////////////////////////////////////////////////////////////////
// [ Array_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_Array.hpp"

//////////////////////////////////////////////////////////////////////////////////
// [Constructor]  
//////////////////////////////////////////////////////////////////////////////////
C_Array::C_Array(){
   _Status  = C_ARRAY_STATUS_ERROR;
   _cItem   = 0;
   _nItem   = 0;
   _cBuffer = 0;
   _pBuffer = 0;
}
//////////////////////////////////////////////////////////////////////////////////
// [Constructor]  
//////////////////////////////////////////////////////////////////////////////////
C_Array::C_Array(DWORD nItem, DWORD cItem){
   _Status  = C_ARRAY_STATUS_ERROR;
   _Create(nItem, cItem);
}
//////////////////////////////////////////////////////////////////////////////////
// [Destructor]
//////////////////////////////////////////////////////////////////////////////////
C_Array::~C_Array(){
   _Destroy();
}
//////////////////////////////////////////////////////////////////////////////////
// [Copy Constructor]
//////////////////////////////////////////////////////////////////////////////////
C_Array::C_Array(const C_Array& rCArray){
   char* pTempBuffer = 0;

   if(_Status != C_ARRAY_STATUS_ERROR)
      _Destroy(); 

   _Status  = C_ARRAY_STATUS_ERROR;

   _cItem   = rCArray._Get_cItem();
   _nItem   = rCArray._Get_nItem();

   _cBuffer = _cItem * _nItem;

   if(_cBuffer){
      _pBuffer = new char[_cBuffer];
      _Status  = C_ARRAY_STATUS_READY;
      
      pTempBuffer = rCArray._Get_pBuffer();
      
      if(_pBuffer && pTempBuffer)
         memcpy(_pBuffer, pTempBuffer, _cBuffer);
   }
}
//////////////////////////////////////////////////////////////////////////////////
// [operator =]
//////////////////////////////////////////////////////////////////////////////////
C_Array & C_Array::operator=(const C_Array& rCArray){
   char* pTempBuffer = 0;

   if(this == &rCArray) return(*this);

   if(_Status != C_ARRAY_STATUS_ERROR)
      _Destroy(); 

   _Status  = C_ARRAY_STATUS_ERROR;

   _cItem   = rCArray._Get_cItem();
   _nItem   = rCArray._Get_nItem();

   _cBuffer = _cItem * _nItem;

   if(_cBuffer){
      _pBuffer = new char[_cBuffer];
      _Status  = C_ARRAY_STATUS_READY;
      pTempBuffer = rCArray._Get_pBuffer();
      if(_pBuffer && pTempBuffer)
         memcpy(_pBuffer, pTempBuffer, _cBuffer);
   }

   return(*this);
}
//////////////////////////////////////////////////////////////////////////////////
// [_Create]
//////////////////////////////////////////////////////////////////////////////////
DWORD C_Array::_Create(DWORD nItem, DWORD cItem)
{
   if(_Status == C_ARRAY_STATUS_READY)
   {
      return(C_ARRAY_STATUS_ERROR ^ C_ARRAY_SUB_EXIST);
   }

   //////////////////////////////////////////////

   _pBuffer = new char[nItem * cItem];

   //////////////////////////////////////////////

   if( !_pBuffer ) return(C_ARRAY_STATUS_ERROR ^ C_ARRAY_SUB_PTR);
      
   _Status  = C_ARRAY_STATUS_READY;

   _nItem   = nItem;
   _cItem   = cItem;
   _cBuffer = nItem * cItem;

   memset(_pBuffer, 0, _cBuffer);

   return(C_ARRAY_STATUS_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [_Destroy]
//////////////////////////////////////////////////////////////////////////////////
DWORD C_Array::_Destroy()
{
   if(_Status == C_ARRAY_STATUS_ERROR)
   {
      return(C_ARRAY_STATUS_ERROR ^ C_ARRAY_SUB_EXIST); 
   }
	  
   //////////////////////////////////////////////

   delete [] _pBuffer;

   //////////////////////////////////////////////

   _Status  = C_ARRAY_STATUS_ERROR;

   _nItem   = 0;
   _cItem   = 0;
   _cBuffer = 0;
   _pBuffer = 0;

   return(C_ARRAY_STATUS_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [_Get_pItem]
//////////////////////////////////////////////////////////////////////////////////
char* C_Array::_Get_pItem(DWORD offset)
{
   if(_Status == C_ARRAY_STATUS_ERROR)
   {
      return((char*)(C_ARRAY_STATUS_ERROR)); 
   }

   return( _pBuffer + (offset * _cItem) );
}
//////////////////////////////////////////////////////////////////////////////////
// [_Get_Item]
//////////////////////////////////////////////////////////////////////////////////
DWORD C_Array::_Get_Item(DWORD offset, void* pDataDst)
{
   char* pDataSrc = 0;

   if(_Status == C_ARRAY_STATUS_ERROR)
   {
      return(C_ARRAY_STATUS_ERROR ^ C_ARRAY_SUB_EXIST); 
   }

   if( !pDataDst ) return(C_ARRAY_STATUS_ERROR ^ C_ARRAY_SUB_PTR);

   if( offset > _nItem ) return(C_ARRAY_STATUS_ERROR ^ C_ARRAY_SUB_OVER);
	 
   pDataSrc  = (char*) _pBuffer;
   pDataSrc += offset * _cItem;

   //////////////////////////////////////////////

   memcpy(pDataDst, pDataSrc, _cItem);

   //////////////////////////////////////////////

   return(C_ARRAY_STATUS_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [_Set_Item]
//////////////////////////////////////////////////////////////////////////////////
DWORD C_Array::_Set_Item(DWORD offset, void* pDataSrc)
{
   char*  pDataDst =  0;

   if( !pDataSrc ) return(C_ARRAY_STATUS_ERROR ^ C_ARRAY_SUB_PTR); 

   if( offset > _nItem ) return(C_ARRAY_STATUS_ERROR ^ C_ARRAY_SUB_OVER); 
	 
   pDataDst  = (char*) _pBuffer;
   pDataDst += offset * _cItem;

   //////////////////////////////////////////////

   memcpy(pDataDst, pDataSrc, _cItem);

   //////////////////////////////////////////////

   return(C_ARRAY_STATUS_READY);
}

