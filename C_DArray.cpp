//////////////////////////////////////////////////////////////////////////////////
// [ DArray_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_DArray.hpp"

//////////////////////////////////////////////////////////////////////////////////
// [Constructor]  
//////////////////////////////////////////////////////////////////////////////////
C_DArray::C_DArray()
{
   _pHead  = new S_C_DArray;
   _pTail  = new S_C_DArray;
   _nItems = 0;
}
//////////////////////////////////////////////////////////////////////////////////
// [Destructor]
//////////////////////////////////////////////////////////////////////////////////
C_DArray::~C_DArray()
{
   _Clear();

   delete _pHead;
   delete _pTail;
}
//////////////////////////////////////////////////////////////////////////////////
// [Copy Constructor]
//////////////////////////////////////////////////////////////////////////////////
C_DArray::C_DArray(const C_DArray& rCDArray)
{
   S_C_DArray* pNewItem  = 0;
   S_C_DArray* prCDArray = 0;

   _pHead  = new S_C_DArray;
   _pTail  = new S_C_DArray;
   _nItems = 0;

   for(DWORD rnItems = 0; rnItems < rCDArray._Get_nItems(); rnItems++)
   {
      if( !_nItems ) // First Item
	  {
         pNewItem = new S_C_DArray;

	     if(!pNewItem) break;

	     pNewItem->_pData = new C_Array;

	     if(!pNewItem->_pData) break;

		 ///////////////////////////////////////////

		 prCDArray = rCDArray._Get_pItem(rnItems);

		 if( !prCDArray ) break;

		 *pNewItem->_pData = *prCDArray->_pData;

		 ///////////////////////////////////////////

	     pNewItem->_pNext = _pTail;
         pNewItem->_pPrev = _pHead;

	     _pHead->_pNext = pNewItem;
         _pTail->_pPrev = pNewItem;
	  }
      else // Next Item 
	  {
         pNewItem = new S_C_DArray;

	     if(!pNewItem) break;

	     pNewItem->_pData = new C_Array;

	     if(!pNewItem->_pData) break;

		 ///////////////////////////////////////////

		 prCDArray = rCDArray._Get_pItem(rnItems);

		 if( !prCDArray ) break;

		 *pNewItem->_pData = *prCDArray->_pData;

		 ///////////////////////////////////////////

	     pNewItem->_pNext = _pTail;
         pNewItem->_pPrev = _pTail->_pPrev;

	     _pTail->_pPrev->_pNext = pNewItem;
         _pTail->_pPrev = pNewItem;
	  }
	
      _nItems++;
   }
}
//////////////////////////////////////////////////////////////////////////////////
// [operator =]
//////////////////////////////////////////////////////////////////////////////////
C_DArray & C_DArray::operator=(const C_DArray& rCDArray)
{
   if(this == &rCDArray) return(*this);

   S_C_DArray* pNewItem  = 0;
   S_C_DArray* prCDArray = 0;

   if( _nItems ) _Clear();

   for(DWORD rnItems = 0; rnItems < rCDArray._Get_nItems(); rnItems++)
   {
      if( !_nItems ) // First Item
	  {
         pNewItem = new S_C_DArray;

	     if(!pNewItem) break;

	     pNewItem->_pData = new C_Array;

	     if(!pNewItem->_pData) break;

		 ///////////////////////////////////////////

		 prCDArray = rCDArray._Get_pItem(rnItems);

		 if( !prCDArray ) break;

		 *pNewItem->_pData = *prCDArray->_pData;

		 ///////////////////////////////////////////

	     pNewItem->_pNext = _pTail;
         pNewItem->_pPrev = _pHead;

	     _pHead->_pNext = pNewItem;
         _pTail->_pPrev = pNewItem;
	  }
      else // Next Item 
	  {
         pNewItem = new S_C_DArray;

	     if(!pNewItem) break;

	     pNewItem->_pData = new C_Array;

	     if(!pNewItem->_pData) break;

		 ///////////////////////////////////////////

		 prCDArray = rCDArray._Get_pItem(rnItems);

		 if( !prCDArray ) break;

		 *pNewItem->_pData = *prCDArray->_pData;

		 ///////////////////////////////////////////

	     pNewItem->_pNext = _pTail;
         pNewItem->_pPrev = _pTail->_pPrev;

	     _pTail->_pPrev->_pNext = pNewItem;
         _pTail->_pPrev = pNewItem;
	  }
	
      _nItems++;
   }

   return(*this);
}
//////////////////////////////////////////////////////////////////////////////////
// [_Add_Item]
//////////////////////////////////////////////////////////////////////////////////
S_C_DArray* C_DArray::_Add_Item()
{
   S_C_DArray* pNewItem = 0;

   if( !_nItems ) // First Item
   {
      pNewItem = new S_C_DArray;

	  if(!pNewItem) return(C_DARRAY_ERROR);

	  pNewItem->_pData = new C_Array;

	  if(!pNewItem->_pData) return(C_DARRAY_ERROR);

	  pNewItem->_pNext = _pTail;
      pNewItem->_pPrev = _pHead;

	  _pHead->_pNext = pNewItem;
      _pTail->_pPrev = pNewItem;
   }
   else // Next Item 
   {
      pNewItem = new S_C_DArray;

	  if(!pNewItem) return(C_DARRAY_ERROR);

	  pNewItem->_pData = new C_Array;

	  if(!pNewItem->_pData) return(C_DARRAY_ERROR);

	  pNewItem->_pNext = _pTail;
      pNewItem->_pPrev = _pTail->_pPrev;

	  _pTail->_pPrev->_pNext = pNewItem;
      _pTail->_pPrev = pNewItem;
   }
	
   _nItems++;

   return(pNewItem);
}
//////////////////////////////////////////////////////////////////////////////////
// [_Add_Item]
//////////////////////////////////////////////////////////////////////////////////
C_Array* C_DArray::_Add_Item(DWORD nItem, DWORD cItem)
{
   S_C_DArray* pNewItem = 0;

   if( !_nItems ) // First Item
   {
      pNewItem = new S_C_DArray;

	  if(!pNewItem) return(C_DARRAY_ERROR);

	  pNewItem->_pData = new C_Array;

	  if(!pNewItem->_pData) return(C_DARRAY_ERROR);

	  ////////////////////////////////////////////////////////////

	  if(pNewItem->_pData->_Create(nItem, cItem) != 1) return(0);

	  ////////////////////////////////////////////////////////////

	  pNewItem->_pNext = _pTail;
      pNewItem->_pPrev = _pHead;

	  _pHead->_pNext = pNewItem;
      _pTail->_pPrev = pNewItem;
   }
   else // Next Item 
   {
      pNewItem = new S_C_DArray;

	  if(!pNewItem) return(C_DARRAY_ERROR);

	  pNewItem->_pData = new C_Array;

	  if(!pNewItem->_pData) return(C_DARRAY_ERROR);

	  ////////////////////////////////////////////////////////////

	  if(pNewItem->_pData->_Create(nItem, cItem) != 1) return(0);

	  ////////////////////////////////////////////////////////////

	  pNewItem->_pNext = _pTail;
      pNewItem->_pPrev = _pTail->_pPrev;

	  _pTail->_pPrev->_pNext = pNewItem;
      _pTail->_pPrev = pNewItem;
   }
	
   _nItems++;

   return(pNewItem->_pData);
}
//////////////////////////////////////////////////////////////////////////////////
// [_Get_pItem]
//////////////////////////////////////////////////////////////////////////////////
S_C_DArray* C_DArray::_Get_pItem(DWORD Offset) const
{
   S_C_DArray* pItem = 0;

   if( (!_nItems) || (Offset > _nItems - 1) ) return(C_DARRAY_ERROR);

   if(Offset <= (_nItems / 2)) // Go over Head
   {
      pItem = _pHead->_pNext;

	  for(DWORD nItem = 0; nItem < Offset; nItem++)
	  {
	     pItem = pItem->_pNext;
	  }
   }
   else // Go over Tail
   {
      pItem = _pTail->_pPrev;

	  for(DWORD nItem = C_DArray::_nItems - 1; nItem > Offset; nItem--)
	  {
	     pItem = pItem->_pPrev;
	  }
   }

   return(pItem);
}
//////////////////////////////////////////////////////////////////////////////////
// [_Del_Item]
//////////////////////////////////////////////////////////////////////////////////
DWORD C_DArray::_Del_Item(DWORD Offset)
{
   S_C_DArray* pItem = 0;

   if( (!_nItems) || (Offset > _nItems - 1) ) return(C_DARRAY_ERROR);

   if(Offset <= (_nItems / 2)) // Go over Head
   {
      pItem = _pHead->_pNext;

      for(DWORD nItem = 0; nItem < Offset; nItem++)
	  {
	     pItem = pItem->_pNext;
	  }
   }
   else // Go over Tail
   {
      pItem = _pTail->_pPrev;

	  for(DWORD nItem = C_DArray::_nItems - 1; nItem > Offset; nItem--)
	  {
	     pItem = pItem->_pPrev;
	  }
   }

   if(pItem->_pData) delete pItem->_pData;

   pItem->_pPrev->_pNext = pItem->_pNext;
   pItem->_pNext->_pPrev = pItem->_pPrev;

   delete pItem;

   _nItems--;

   return(C_DARRAY_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [_Clear]
//////////////////////////////////////////////////////////////////////////////////
DWORD C_DArray::_Clear()
{
   S_C_DArray* pItem = 0;

   if(_nItems)
   {
      pItem = _pHead->_pNext;

      for(DWORD nItems = 0; nItems < C_DArray::_nItems; nItems++)
	  {
	     if(pItem->_pData) delete pItem->_pData;

		 pItem = pItem->_pNext;

		 if(!pItem) return(C_DARRAY_ERROR);

		 if(pItem->_pPrev) delete pItem->_pPrev;
	  }

      _nItems = 0;
   }

   return(C_DARRAY_READY);
}
