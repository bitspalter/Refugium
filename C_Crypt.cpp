//////////////////////////////////////////////////////////////////////////////////
// [ Crypt_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_Crypt.hpp"

//////////////////////////////////////////////////////////////////////////////////
// [setKey]  
//////////////////////////////////////////////////////////////////////////////////
int C_Crypt::setKey(string* pkey){
  
   if(!pkey) return(C_CRYPT_ERROR);
   
   CAES.C_Rijndael.MakeKey(pkey->c_str(), pkey->c_str(), 16, 16);
   
   this->key = *pkey;

   this->status = C_CRYPT_READY;
   
   return(C_CRYPT_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [getType]  
//////////////////////////////////////////////////////////////////////////////////
int C_Crypt::getType(fstream* pfile_i){
   C_Array CA_Buffer;
   S_Crypt_Main scmain;
   
   if(this->status != C_CRYPT_READY || !pfile_i) return(C_CRYPT_ERROR);

   ////////////////////////////
   // Get File Size
   pfile_i->seekg(0, pfile_i->end);
   int cdata = pfile_i->tellg();
   pfile_i->seekg(0, pfile_i->beg);
   
   if(cdata < sizeof(S_Crypt_Main)) return(C_CRYPT_ERROR);

   ///////////////////////
   // Read Main Head
   pfile_i->read((char*)&scmain, sizeof(S_Crypt_Main));
   // Decrypt Main Head
   CAES.decrypt((char*)&scmain, sizeof(S_Crypt_Main), &CA_Buffer);
   
   memcpy(&scmain, CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer());

   ////////////////////////////
   // Check Version
   if(scmain.Version != C_CRYPT_VERSION) return(C_CRYPT_ERROR);

   ////////////////////////////
   // Check Type
   if(scmain.Type == C_CRYPT_TYPE_FILE)   return(C_CRYPT_TYPE_FILE);
   else
   if(scmain.Type == C_CRYPT_TYPE_FOLDER) return(C_CRYPT_TYPE_FOLDER);

   return(C_CRYPT_ERROR);
}
//////////////////////////////////////////////////////////////////////////////////
// [getData]  
//////////////////////////////////////////////////////////////////////////////////
int C_Crypt::getData(fstream* pfile, C_DArray* pDA_Folder, C_DArray* pDA_File){
   S_Crypt_Main scmain;
   S_Crypt_Data scdata;
   C_Array CA_Buffer, CA_Buffer2;
   streampos Offset = 0;
   
   if(this->status != C_CRYPT_READY || !pfile || !pDA_Folder || !pDA_File) return(C_CRYPT_ERROR);

   ///////////////////////
   // Set File Pointer START
   pfile->seekg(Offset, pfile->beg); 
  
   ///////////////////////
   // Read Main Head
   if(!pfile->read((char*)&scmain, sizeof(S_Crypt_Main))) return(C_CRYPT_ERROR);
   // Decrypt Main Head
   this->decrypt((char*)&scmain, sizeof(S_Crypt_Main), &CA_Buffer, C_CRYPT_NEW_KEY);
   
   memcpy(&scmain, CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer());

   ////////////////////////////
   // Check Version
   if(scmain.Version != C_CRYPT_VERSION) return(C_CRYPT_ERROR);

   Offset += sizeof(S_Crypt_Main);

   ////////////////////////////
   // Get Folder
   for(unsigned long nFolder = 0; nFolder < scmain.nFolder; nFolder++){
      ///////////////////////
      // Read Data Head
      if(!pfile->read((char*)&scdata, sizeof(S_Crypt_Data))) return(C_CRYPT_ERROR);
      // Decrypt Data Head
      this->decrypt((char*)&scdata, sizeof(S_Crypt_Data), &CA_Buffer, C_CRYPT_NEW_KEY);
   
      memcpy(&scdata, CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer());
      
      ////////////////////////////
      // Check Version
      if(scdata.Version != C_CRYPT_VERSION) return(C_CRYPT_ERROR);

      ///////////////////////
      // Set File Pointer
      pfile->seekg(Offset, pfile->beg);
      
      ////////////////////////////
      // Save 
      C_Array* pCA_Temp = pDA_Folder->_Add_Item(1, scdata.pData);
      if(!pfile->read(pCA_Temp->_Get_pBuffer(), scdata.pData)) return(C_CRYPT_ERROR);

      Offset += scdata.pData;
   }
   
   ////////////////////////////
   // Get Files
   for(unsigned long nFiles = 0; nFiles < scmain.nData; nFiles++){
      ///////////////////////
      // Read Data Head
      if(!pfile->read((char*)&scdata, sizeof(S_Crypt_Data))) return(C_CRYPT_ERROR);
      // Decrypt Data Head
      this->decrypt((char*)&scdata, sizeof(S_Crypt_Data), &CA_Buffer, C_CRYPT_NEW_KEY);

      memcpy(&scdata, CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer());

      ////////////////////////////
      // Check Version
      if(scdata.Version != C_CRYPT_VERSION) return(C_CRYPT_ERROR);
      
      ///////////////////////
      // Set File Pointer
      pfile->seekg(Offset, pfile->beg);
      
      while(scdata.cData % 16) scdata.cData++;
      
      ////////////////////////////
      // Save 
      C_Array* pCA_Temp = pDA_File->_Add_Item(1, (scdata.pData + scdata.cData));
      if(!pfile->read(pCA_Temp->_Get_pBuffer(), (scdata.pData + scdata.cData))) return(C_CRYPT_ERROR);

      Offset += (scdata.pData + scdata.cData);
   }
   
   return(C_CRYPT_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [getDataHeads]  
//////////////////////////////////////////////////////////////////////////////////
int C_Crypt::getDataHeads(fstream* pfile, C_DArray* pDA_Folder, C_DArray* pDA_File){
   S_Crypt_Main scmain;
   S_Crypt_Data scdata;
   C_Array CA_Buffer, CA_Buffer2;
   streampos Offset = 0;
   
   if(this->status != C_CRYPT_READY || !pfile || !pDA_Folder || !pDA_File) return(C_CRYPT_ERROR);

   ///////////////////////
   // Set File Pointer START
   pfile->seekg(Offset, pfile->beg); 
   
   ///////////////////////
   // Read Main Head
   if(!pfile->read((char*)&scmain, sizeof(S_Crypt_Main))) return(C_CRYPT_ERROR);
   // Decrypt Main Head
   this->decrypt((char*)&scmain, sizeof(S_Crypt_Main), &CA_Buffer, C_CRYPT_NEW_KEY);
   
   memcpy(&scmain, CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer());

   ////////////////////////////
   // Check Version
   if(scmain.Version != C_CRYPT_VERSION) return(C_CRYPT_ERROR);

   Offset += sizeof(S_Crypt_Main);
   
   ////////////////////////////
   // Get Folder
   for(unsigned long nFolder = 0; nFolder < scmain.nFolder; nFolder++){

      ///////////////////////
      // Set File Pointer
      pfile->seekg(Offset, pfile->beg);
     
      ///////////////////////
      // Read Data Head
      if(!pfile->read((char*)&scdata, sizeof(S_Crypt_Data))) return(C_CRYPT_ERROR);
      // Decrypt Data Head
      this->decrypt((char*)&scdata, sizeof(S_Crypt_Data), &CA_Buffer, C_CRYPT_NEW_KEY);
   
      memcpy(&scdata, CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer());
      
      ////////////////////////////
      // Check Version
      if(scdata.Version != C_CRYPT_VERSION) return(C_CRYPT_ERROR);
      
      ////////////////////////////
      // Save 
      C_Array* pCA_Temp = pDA_Folder->_Add_Item(1, sizeof(S_Crypt_Data));
      memcpy(pCA_Temp->_Get_pBuffer(), (char*)&scdata, sizeof(S_Crypt_Data));
      
      Offset += scdata.pData;
   }

   ////////////////////////////
   // Get Files
   for(unsigned long nFiles = 0; nFiles < scmain.nData; nFiles++){

      ///////////////////////
      // Set File Pointer
      pfile->seekg(Offset, pfile->beg);
	
      ///////////////////////
      // Read Data Head
      if(!pfile->read((char*)&scdata, sizeof(S_Crypt_Data))) return(C_CRYPT_ERROR);
      // Decrypt Data Head
      this->decrypt((char*)&scdata, sizeof(S_Crypt_Data), &CA_Buffer, C_CRYPT_NEW_KEY);

      memcpy(&scdata, CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer());

      ////////////////////////////
      // Check Version
      if(scdata.Version != C_CRYPT_VERSION) return(C_CRYPT_ERROR);
      
      ////////////////////////////
      // Save 
      C_Array* pCA_Temp = pDA_File->_Add_Item(1, sizeof(S_Crypt_Data));
      memcpy(pCA_Temp->_Get_pBuffer(), (char*)&scdata, sizeof(S_Crypt_Data));
      
      while(scdata.cData % 16) scdata.cData++;
      
      Offset += (scdata.pData + scdata.cData);
   }

   return(C_CRYPT_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [encrypt]  
//////////////////////////////////////////////////////////////////////////////////
int C_Crypt::encrypt(char* pData, unsigned long cData, C_Array* pCA_out, int modus){
  
   if(this->status != C_CRYPT_READY || !pData || !cData || !pCA_out) return(C_CRYPT_ERROR);
   
   if(modus == C_CRYPT_NEW_KEY) 
      CAES.C_Rijndael.MakeKey(this->key.c_str(), this->key.c_str(), 16, 16);
   
   if(CAES.encrypt(pData, cData, pCA_out) != C_AES_READY) return(C_CRYPT_ERROR);
   
   return(C_CRYPT_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [decrypt]  
//////////////////////////////////////////////////////////////////////////////////
int C_Crypt::decrypt(char* pData, unsigned long cData, C_Array* pCA_out, int modus){

   if(this->status != C_CRYPT_READY || !pData || !cData || !pCA_out) return(C_CRYPT_ERROR);
   
   if(modus == C_CRYPT_NEW_KEY) 
      CAES.C_Rijndael.MakeKey(this->key.c_str(), this->key.c_str(), 16, 16);
   
   if(CAES.decrypt(pData, cData, pCA_out) != C_AES_READY) return(C_CRYPT_ERROR);
   
   return(C_CRYPT_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [encrypt_file]  
//////////////////////////////////////////////////////////////////////////////////
int C_Crypt::encrypt_file(fstream* pfile_i, fstream* pfile_o, string* pname_in, int modus){
   C_Array CA_Clear, CA_Crypt, CA_Buffer;
   S_Crypt_Main scmain;
   S_Crypt_Data scdata;

   if(this->status != C_CRYPT_READY || !pfile_i || !pfile_o || !pname_in) return(C_CRYPT_ERROR);

   ////////////////////////////
   // Get File Size
   pfile_i->seekg(0, pfile_i->end);
   int cdata = pfile_i->tellg();
   pfile_i->seekg(0, pfile_i->beg);

   ////////////////////////////
   // Create File Buffer
   CA_Clear._Create(1, cdata);
   char* pdata = CA_Clear._Get_pBuffer();

   ////////////////////////////
   // Read File to Buffer
   if(!pfile_i->read(pdata, cdata)) return(C_CRYPT_ERROR);

   ////////////////////////////////////////
   // single File
   if(modus == C_CRYPT_TYPE_FILE){
      ////////////////////////////
      // Fill Main Head
      scmain.Version = C_CRYPT_VERSION;
      scmain.Type    = C_CRYPT_TYPE_FILE;
      scmain.nData   = 0x01;
      scmain.cData   = cdata;
   
      ////////////////////////////
      // Crypt Main Head
      CAES.encrypt((char*)&scmain, sizeof(S_Crypt_Main), &CA_Buffer);
      // Write Main Head
      if(!pfile_o->write(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);
   }
   
   ////////////////////////////
   // Get Name
   int   cName = pname_in->length();
   char* pName = (char*)pname_in->c_str();

   while(cName % 16) cName++; // align the Name

   ////////////////////////////
   // Fill Data Head
   scdata.Version = C_CRYPT_VERSION;
   scdata.Type    = C_CRYPT_TYPE_FILE;
   scdata.pData   = sizeof(S_Crypt_Data) + cName;
   scdata.cData   = cdata;
 
   ////////////////////////////
   // Crypt Data Head
   CAES.encrypt((char*)&scdata, sizeof(S_Crypt_Data), &CA_Buffer);
   // Write Data Head
   if(!pfile_o->write(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);

   ////////////////////////////
   // Crypt Name
   CAES.encrypt(pName, cName, &CA_Buffer);
   // Write Name
   if(!pfile_o->write(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);

   ////////////////////////////
   // Crypt Data
   CAES.encrypt(pdata, cdata, &CA_Crypt);
   // Write Data
   if(!pfile_o->write(CA_Crypt._Get_pBuffer(), CA_Crypt._Get_cBuffer())) return(C_CRYPT_ERROR);

   return(C_CRYPT_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [decrypt_file]  
//////////////////////////////////////////////////////////////////////////////////
int C_Crypt::decrypt_file(fstream* pfile_i, fstream* pfile_o, string* pname_in, string* pname_out){
   C_Array CA_Clear, CA_Crypt, CA_Buffer, CA_Buffer2;
   S_Crypt_Main scmain;
   S_Crypt_Data scdata;
   
   if(this->status != C_CRYPT_READY || !pfile_i || !pfile_o || !pname_in || !pname_out) return(C_CRYPT_ERROR);

   ////////////////////////////
   // Get File Size
   pfile_i->seekg(0, pfile_i->end);
   int cdata = pfile_i->tellg();
   pfile_i->seekg(0, pfile_i->beg);

   if(cdata < sizeof(S_Crypt_Main)) return(C_CRYPT_ERROR);
   
   ///////////////////////
   // Read Main Head
   if(!pfile_i->read((char*)&scmain, sizeof(S_Crypt_Main))) return(C_CRYPT_ERROR);
   // Decrypt Main Head
   CAES.decrypt((char*)&scmain, sizeof(S_Crypt_Main), &CA_Buffer);
   
   memcpy(&scmain, CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer());

   ////////////////////////////
   // Check Version
   if(scmain.Version != C_CRYPT_VERSION) return(C_CRYPT_ERROR);

   ////////////////////////////
   // Check Type
   if(scmain.Type == C_CRYPT_TYPE_FILE){
      //message("C_CRYPT_TYPE_FILE");
   }else
   if(scmain.Type == C_CRYPT_TYPE_FOLDER){
      if(this->decrypt_folder(&scmain, pfile_i, cdata, pname_in, pname_out) != C_CRYPT_READY) 
         return(C_CRYPT_ERROR); // DECRYPT DIR
      return(C_CRYPT_READY);
   }else 
      return(C_CRYPT_ERROR);

   ///////////////////////
   // Read Data Head
   if(!pfile_i->read((char*)&scdata, sizeof(S_Crypt_Data))) return(C_CRYPT_ERROR); 
   // Decrypt Data Head
   CAES.decrypt((char*)&scdata, sizeof(S_Crypt_Data), &CA_Buffer);
   
   memcpy(&scdata, CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer());
   
   ////////////////////////////
   // Check Version
   if(scdata.Version != C_CRYPT_VERSION) return(C_CRYPT_ERROR);

   ///////////////////////
   // Get Name
   char Name[PATH_MAX];
   memset(Name, 0, PATH_MAX);
   
   ///////////////////////
   // Read Name
   if(!pfile_i->read(Name, scdata.pData - sizeof(S_Crypt_Data))) return(C_CRYPT_ERROR);
   // Decrypt Name
   CAES.decrypt(Name, scdata.pData - sizeof(S_Crypt_Data), &CA_Buffer);

   CA_Buffer2._Destroy();
   CA_Buffer2._Create(1, (scdata.pData - sizeof(S_Crypt_Data)) + 1);
   
   memcpy(CA_Buffer2._Get_pBuffer(), CA_Buffer._Get_pBuffer(), scdata.pData - sizeof(S_Crypt_Data));
   
   ///////////////////////
   // Build Output Path
   string fullname;
   
   fullname = *pname_out;
   fullname += "/";   
   
   string tempstr = CA_Buffer2._Get_pBuffer();
   
   size_t found_in = tempstr.rfind("/");
   string str2 = tempstr.substr(found_in + 1, tempstr.length() - found_in - 1);
   
   fullname += str2;

   cdata = scdata.cData;
   
   while(cdata % 16) cdata++; // align the Name
   
   ///////////////////////
   // Get Data
   CA_Clear._Create(1, cdata);
   char* pdata = CA_Clear._Get_pBuffer();

   ///////////////////////
   // Read Data
   if(!pfile_i->read(pdata, cdata)) return(C_CRYPT_ERROR);
   // Decrypt Data
   CAES.decrypt(pdata, cdata, &CA_Buffer);
  
   ///////////////////////
   // Create File
   pfile_o->open(fullname.c_str(), fstream::in | fstream::out | fstream::app);
   if(!pfile_o->is_open()) return(C_CRYPT_ERROR);

   ///////////////////////
   // Write Data
   if(!pfile_o->write(CA_Buffer._Get_pBuffer(), scdata.cData)) return(C_CRYPT_ERROR);

   return(C_CRYPT_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [decrypt_filev]  
//////////////////////////////////////////////////////////////////////////////////
int C_Crypt::decrypt_filev(fstream* pfile_i, C_Array* pcarray_out){

   C_Array CA_Clear, CA_Crypt, CA_Buffer;
   S_Crypt_Main scmain;
   S_Crypt_Data scdata;
   
   if(this->status != C_CRYPT_READY || !pfile_i || !pcarray_out) return(C_CRYPT_ERROR);
   
   ////////////////////////////
   // Get File Size
   pfile_i->seekg(0, pfile_i->end);
   int cdata = pfile_i->tellg();
   pfile_i->seekg(0, pfile_i->beg);

   if(cdata < sizeof(S_Crypt_Main)) return(C_CRYPT_ERROR);
   
   ///////////////////////
   // Read Main Head
   if(!pfile_i->read((char*)&scmain, sizeof(S_Crypt_Main))) return(C_CRYPT_ERROR);
   // Decrypt Main Head
   CAES.decrypt((char*)&scmain, sizeof(S_Crypt_Main), &CA_Buffer);
   
   memcpy(&scmain, CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer());

   ////////////////////////////
   // Check Version
   if(scmain.Version != C_CRYPT_VERSION) return(C_CRYPT_ERROR);
   
   ////////////////////////////
   // Check Type
   if(scmain.Type == C_CRYPT_TYPE_FILE){
      //message("C_CRYPT_TYPE_FILE");
   }else
   if(scmain.Type == C_CRYPT_TYPE_FOLDER){
      //Decrypt_Dir(&scmain, tpinput, cInput); // DECRYPT DIR
      return(C_CRYPT_READY);
   }else 
      return(C_CRYPT_ERROR);

   ///////////////////////
   // Read Data Head
   if(!pfile_i->read((char*)&scdata, sizeof(S_Crypt_Data))) return(C_CRYPT_ERROR);
   // Decrypt Data Head
   CAES.decrypt((char*)&scdata, sizeof(S_Crypt_Data), &CA_Buffer);
   
   memcpy(&scdata, CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer());
   
   ////////////////////////////
   // Check Version
   if(scdata.Version != C_CRYPT_VERSION) return(C_CRYPT_ERROR);

   ///////////////////////
   // Get Name
   char Name[PATH_MAX];
   memset(Name, 0, PATH_MAX);
   
   ///////////////////////
   // Read Name
   if(!pfile_i->read(Name, scdata.pData - sizeof(S_Crypt_Data))) return(C_CRYPT_ERROR);
   // Decrypt Name
   CAES.decrypt(Name, scdata.pData - sizeof(S_Crypt_Data), &CA_Buffer);

   cdata = scdata.cData;
   
   while(cdata % 16) cdata++; // align the Name
   
   ///////////////////////
   // Get Data
   CA_Clear._Create(1, cdata);
   char* pdata = CA_Clear._Get_pBuffer();

   ///////////////////////
   // Read Data
   if(!pfile_i->read(pdata, cdata)) return(C_CRYPT_ERROR);
   // Decrypt Data
   CAES.decrypt(pdata, cdata, pcarray_out);

   return(C_CRYPT_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [decrypt_filev]  
//////////////////////////////////////////////////////////////////////////////////
int C_Crypt::decrypt_filev(fstream* pfile_in, unsigned long pData, C_Array* pCA_out){
   C_Array CA_Clear, CA_Buffer;
   S_Crypt_Data scdata;

   if(this->status != C_CRYPT_READY || !pfile_in || !pData || !pCA_out) return(C_CRYPT_ERROR);
   
   ////////////////////////////
   // Set File Pointer
   if(!pfile_in->seekg(pData, pfile_in->beg)) return(C_CRYPT_ERROR);
   
   ////////////////////////////
   // TEST
   CAES.C_Rijndael.MakeKey(this->key.c_str(), this->key.c_str(), 16, 16);
   ///////////////////////
   // Read Main Head
   if(!pfile_in->read((char*)&scdata, sizeof(S_Crypt_Data))) return(C_CRYPT_ERROR);
   // Decrypt Main Head
   CAES.decrypt((char*)&scdata, sizeof(S_Crypt_Data), &CA_Buffer);
   
   memcpy(&scdata, CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer());

   ////////////////////////////
   // Check Version
   if(scdata.Version != C_CRYPT_VERSION) return(C_CRYPT_ERROR);
   
   ///////////////////////
   // Get Name
   char Name[PATH_MAX];
   memset(Name, 0, PATH_MAX);
   
   ///////////////////////
   // Read Name
   if(!pfile_in->read(Name, scdata.pData - sizeof(S_Crypt_Data))) return(C_CRYPT_ERROR);
   // Decrypt Name
   CAES.decrypt(Name, scdata.pData - sizeof(S_Crypt_Data), &CA_Buffer);

   unsigned long cdata = scdata.cData;
   
   while(cdata % 16) cdata++; // align the Name
   
   ///////////////////////
   // Get Data
   CA_Clear._Create(1, cdata);
   char* pdata = CA_Clear._Get_pBuffer();

   ///////////////////////
   // Read Data
   if(!pfile_in->read(pdata, cdata)) return(C_CRYPT_ERROR);
   // Decrypt Data
   CAES.decrypt(pdata, cdata, &CA_Buffer);
   
   // TEST
   pCA_out->_Destroy();
   pCA_out->_Create(1, scdata.cData + 1);
   
   memcpy(pCA_out->_Get_pBuffer(), CA_Buffer._Get_pBuffer(), scdata.cData);

   return(C_CRYPT_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [encrypt_folder]  
//////////////////////////////////////////////////////////////////////////////////
int C_Crypt::encrypt_folder(fstream* pfile_o, string* pname_in){
   C_DArray CDA_Folder, CDA_Files;
   unsigned long cData = 0;
   S_Crypt_Main scmain;
   S_Crypt_Data scdata;
   C_Array CA_Buffer;
   S_C_DArray* pTemp = 0;
   
   if(this->status != C_CRYPT_READY || !pfile_o || !pname_in) return(C_CRYPT_ERROR);

   /////////////////////////////////////////////////////////////////////
   // Get Files and Folders
   // Get the Size 
   if(CFSystem.rdir((char*)pname_in->c_str(), &CDA_Folder, &CDA_Files) != C_FILE_SYSTEM_READY) 
      return(C_CRYPT_ERROR);
   if(CFSystem.getrdirsize((char*)pname_in->c_str(), &cData) != C_FILE_SYSTEM_READY) 
      return(C_CRYPT_ERROR);

   /////////////////////////////////////////////////////////////////////
   // Write MainHead
   scmain.Version = C_CRYPT_VERSION;
   scmain.Type    = C_CRYPT_TYPE_FOLDER;
   scmain.cData   = cData;
   scmain.nData   = CDA_Files._Get_nItems();
   scmain.nFolder = CDA_Folder._Get_nItems() + 1; // + Root Folder
   
   ////////////////////////////
   // TEST
   CAES.C_Rijndael.MakeKey(this->key.c_str(), this->key.c_str(), 16, 16);
   ////////////////////////////
   // Crypt Main Head
   CAES.encrypt((char*)&scmain, sizeof(S_Crypt_Main), &CA_Buffer);
   // Write Main Head
   if(!pfile_o->write(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);

   /////////////////////////////////////////////////////////////////////
   // Encrypt Root Folder
   
   char psFolder[PATH_MAX] = {0x00};
   
   size_t found = pname_in->rfind("/");
   string str2  = pname_in->substr(found, pname_in->size() - found);

   unsigned long cName = str2.size();
   if(cName > PATH_MAX - 1) return(C_CRYPT_ERROR);
   
   memset(psFolder, 0x00, sizeof(psFolder));
   memcpy(psFolder, str2.c_str(), cName);
   
   while(cName % 16) cName++; // align the Name

   /////////////////////////////////////////////////////////////////////
   // Write Data Head
   scdata.Version = C_CRYPT_VERSION;
   scdata.Type    = C_CRYPT_TYPE_FOLDER;
   scdata.cData   = 0x00;
   scdata.pData   = sizeof(S_Crypt_Data) + cName;
   
   ////////////////////////////
   // TEST
   CAES.C_Rijndael.MakeKey(this->key.c_str(), this->key.c_str(), 16, 16);
   ////////////////////////////
   // Crypt Data Head
   CAES.encrypt((char*)&scdata, sizeof(S_Crypt_Data), &CA_Buffer);
   // Write Data Head
   if(!pfile_o->write(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);

   ////////////////////////////
   // Crypt Data
   CAES.encrypt(psFolder, cName, &CA_Buffer);
   // Write Data
   if(!pfile_o->write(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);

   /////////////////////////////////////////////////////////////////////
   // Encrypt Folder
   for(unsigned long nFolder = 0; nFolder < CDA_Folder._Get_nItems(); nFolder++){
      pTemp = CDA_Folder._Get_pItem(nFolder);
  
      string str3 = pTemp->_pData->_Get_pBuffer();
      string str2 = str3.substr(found, str3.size() - found);

      unsigned long cName = str2.size();
      if(cName > PATH_MAX - 1) return(C_CRYPT_ERROR);
      
      memset(psFolder, 0x00, sizeof(psFolder)); 
      memcpy(psFolder, str2.c_str(), cName);
      
      while(cName % 16) cName++; // align the Name
 
      /////////////////////////////////////////////////////////////////////
      // Write Data Head
      scdata.Version = C_CRYPT_VERSION;
      scdata.Type    = C_CRYPT_TYPE_FOLDER;
      scdata.cData   = 0x00;
      scdata.pData   = sizeof(S_Crypt_Data) + cName;
      
      ////////////////////////////
      // TEST
      CAES.C_Rijndael.MakeKey(this->key.c_str(), this->key.c_str(), 16, 16);
      ////////////////////////////
      // Crypt Data Head
      CAES.encrypt((char*)&scdata, sizeof(S_Crypt_Data), &CA_Buffer);
      // Write Data Head
      if(!pfile_o->write(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);

      ////////////////////////////
      // Crypt Data
      CAES.encrypt(psFolder, cName, &CA_Buffer);
      // Write Data
      if(!pfile_o->write(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);
   }

   /////////////////////////////////////////////////////////////////////
   // Encrypt Files
   fstream file_in;
   
   for(unsigned long nFiles = 0; nFiles < CDA_Files._Get_nItems(); nFiles++){
      pTemp = CDA_Files._Get_pItem(nFiles);
 
      file_in.open(pTemp->_pData->_Get_pBuffer(), fstream::in);
      if(!file_in.is_open()) return(C_CRYPT_ERROR);

      string str3 = pTemp->_pData->_Get_pBuffer();
      string str2 = str3.substr(found, str3.size() - found);

      ////////////////////////////
      // TEST
      CAES.C_Rijndael.MakeKey(this->key.c_str(), this->key.c_str(), 16, 16);
      ////////////////////////////
      // Crypt File
      if(this->encrypt_file(&file_in, pfile_o, &str2, C_CRYPT_TYPE_FOLDER) != C_CRYPT_READY) return(C_CRYPT_ERROR);
      
      file_in.close();
   }

   return(C_CRYPT_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [decrypt_folder]  
//////////////////////////////////////////////////////////////////////////////////
int C_Crypt::decrypt_folder(S_Crypt_Main* pSCMainHead, fstream* pfile_i, unsigned long cData, string* pname_in, string* pname_out){
   S_Crypt_Data scdata;
   C_Array CA_Buffer, CA_Buffer2, CA_Buffer3;
   fstream file_o;
      
   if(this->status != C_CRYPT_READY || !pSCMainHead || !pfile_i || !cData || !pname_in || !pname_out) return(C_CRYPT_ERROR);

   /////////////////////////////////////////////////////////////////////////////////
   // Test: Create the RootFolder 
   // Test: Save the Path and append "\"
  
   ////////////////////////////
   // Get Name
   char pName[PATH_MAX];
   memset(pName, 0x00, PATH_MAX);
   
   size_t found_in  = pname_in->rfind("/");
   string str2 = pname_in->substr(found_in + 1, pname_in->length() - found_in - 1);
   int cName = str2.length();
   
   memcpy(pName, str2.c_str(), str2.length());
   
   string str3 = pname_out->c_str();
   
   str3.append("/");
   str3.append(str2);

   if(CFSystem.makedir((char*)str3.c_str(), S_IRWXU) != C_FILE_SYSTEM_READY) return(C_CRYPT_ERROR);

   /////////////////////////////////////////////////////////////////////////////////
   // Test: Read the Folder Data Heads
   // Test: Create the Folders
   for(unsigned long nFolder = 0; nFolder < pSCMainHead->nFolder; nFolder++){
     
      ////////////////////////////
      // TEST
      CAES.C_Rijndael.MakeKey(this->key.c_str(), this->key.c_str(), 16, 16);
      ///////////////////////
      // Read Data Head
      if(!pfile_i->read((char*)&scdata, sizeof(S_Crypt_Data))) return(C_CRYPT_ERROR);
      // Decrypt Data Head
      CAES.decrypt((char*)&scdata, sizeof(S_Crypt_Data), &CA_Buffer);
      
      memcpy(&scdata, CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer());

      if(scdata.Version != C_CRYPT_VERSION) return(C_CRYPT_ERROR);

      string str4 = str3;

      CA_Buffer._Destroy();
      CA_Buffer._Create(1, scdata.pData - sizeof(S_Crypt_Data));
      
      ///////////////////////
      // Read Data
      if(!pfile_i->read(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);
      // Decrypt Data
      CAES.decrypt(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer(), &CA_Buffer2);
      
      CA_Buffer3._Destroy();
      CA_Buffer3._Create(1, (scdata.pData - sizeof(S_Crypt_Data)) + 1);
      
      memcpy(CA_Buffer3._Get_pBuffer(), CA_Buffer2._Get_pBuffer(), (scdata.pData - sizeof(S_Crypt_Data)));
      
      str4.append(CA_Buffer3._Get_pBuffer());

      if(CFSystem.makedir((char*)str4.c_str(), S_IRWXU) != C_FILE_SYSTEM_READY) return(C_CRYPT_ERROR);
   }

   /////////////////////////////////////////////////////////////////////////////////
   // Test: Read the File Data Heads
   // Test: Create the Files
   for(unsigned long nFile = 0; nFile < pSCMainHead->nData; nFile++){

      ////////////////////////////
      // TEST
      CAES.C_Rijndael.MakeKey(this->key.c_str(), this->key.c_str(), 16, 16);
      ///////////////////////
      // Read Data Head
      if(!pfile_i->read((char*)&scdata, sizeof(S_Crypt_Data))) return(C_CRYPT_ERROR);
      // Decrypt Data Head
      CAES.decrypt((char*)&scdata, sizeof(S_Crypt_Data), &CA_Buffer);

      memcpy(&scdata, CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer());

      CA_Buffer._Destroy();
      CA_Buffer._Create(1, scdata.pData - sizeof(S_Crypt_Data));

      ///////////////////////
      // Read Data
      if(!pfile_i->read(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);
      // Decrypt Data
      CAES.decrypt(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer(), &CA_Buffer2);
      
      ///////////////////////
      // Append File Name
      string str4 = str3;
      str4.append(CA_Buffer2._Get_pBuffer());

      ///////////////////////
      // Create File
      file_o.open(str4.c_str(), fstream::in | fstream::out | fstream::app);
      if(!file_o.is_open()) return(C_CRYPT_ERROR);
      
      unsigned long cData = scdata.cData;
      
      while(cData % 16) cData++;
      
      ////////////////////////////
      // Read File to Buffer
      CA_Buffer._Destroy();
      CA_Buffer._Create(1, cData);
      
      ///////////////////////
      // Read Data
      if(!pfile_i->read(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);
      // Decrypt Data
      CAES.decrypt(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer(), &CA_Buffer2);

      ///////////////////////
      // Write Data
      if(!file_o.write(CA_Buffer2._Get_pBuffer(), scdata.cData)) return(C_CRYPT_ERROR);
      
      ///////////////////////
      // Close File
      file_o.close();
   }
   
   return(C_CRYPT_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [decrypt_folderv]  
//////////////////////////////////////////////////////////////////////////////////
int C_Crypt::decrypt_folderv(fstream* pfile_i, C_DArray* pDA_Folder, C_DArray* pDA_File){
   S_Crypt_Main scmain; 
   S_Crypt_Data scdata;
   C_Array CA_Buffer, CA_Buffer2;
   C_Array* pCA_Folder = 0;
   C_Array* pCA_File   = 0;
   
   if(this->status != C_CRYPT_READY || !pDA_Folder || !pDA_File || !pfile_i) return(C_CRYPT_ERROR);
  
   ////////////////////////////
   // Get File Size
   pfile_i->seekg(0, pfile_i->end);
   int cdata = pfile_i->tellg();
   pfile_i->seekg(0, pfile_i->beg);

   if(cdata < sizeof(S_Crypt_Main)) return(C_CRYPT_ERROR);
   
   ////////////////////////////
   // TEST
   CAES.C_Rijndael.MakeKey(this->key.c_str(), this->key.c_str(), 16, 16);
   ///////////////////////
   // Read Main Head
   if(!pfile_i->read((char*)&scmain, sizeof(S_Crypt_Main))) return(C_CRYPT_ERROR);
   // Decrypt Main Head
   CAES.decrypt((char*)&scmain, sizeof(S_Crypt_Main), &CA_Buffer);
   
   memcpy(&scmain, CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer());

   ////////////////////////////
   // Check Version
   if(scmain.Version != C_CRYPT_VERSION) return(C_CRYPT_ERROR);

   /////////////////////////////////////////////////////////////////////////////////
   // Test: Read the Folder Data Heads
   // Test: Create the Folders
   for(unsigned long nFolder = 0; nFolder < scmain.nFolder; nFolder++){
     
      int DH_Offset = pfile_i->tellg();
     
      ////////////////////////////
      // TEST
      CAES.C_Rijndael.MakeKey(this->key.c_str(), this->key.c_str(), 16, 16);
      ///////////////////////
      // Read Data Head
      if(!pfile_i->read((char*)&scdata, sizeof(S_Crypt_Data))) return(C_CRYPT_ERROR);
      // Decrypt Data Head
      CAES.decrypt((char*)&scdata, sizeof(S_Crypt_Data), &CA_Buffer);
      
      memcpy(&scdata, CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer());

      if(scdata.Version != C_CRYPT_VERSION) return(C_CRYPT_ERROR);

      CA_Buffer._Destroy();
      CA_Buffer._Create(1, scdata.pData - sizeof(S_Crypt_Data));
      
      ///////////////////////
      // Read Name
      if(!pfile_i->read(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);
      // Decrypt Name
      CAES.decrypt(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer(), &CA_Buffer2);

      if(!(pCA_Folder = pDA_Folder->_Add_Item(1, scdata.pData - sizeof(S_Crypt_Data) + 1))) return(C_CRYPT_ERROR);
      memcpy(pCA_Folder->_Get_pBuffer(), CA_Buffer2._Get_pBuffer(), scdata.pData - sizeof(S_Crypt_Data));
   }
   
   /////////////////////////////////////////////////////////////////////////////////
   // Test: Read the File Data Heads
   // Test: Create the Files
   for(unsigned long nFile = 0; nFile < scmain.nData; nFile++){
     
      int DH_Offset = pfile_i->tellg();
      
      ////////////////////////////
      // TEST
      CAES.C_Rijndael.MakeKey(this->key.c_str(), this->key.c_str(), 16, 16);
      ///////////////////////
      // Read Data Head
      if(!pfile_i->read((char*)&scdata, sizeof(S_Crypt_Data))) return(C_CRYPT_ERROR);
      // Decrypt Data Head
      CAES.decrypt((char*)&scdata, sizeof(S_Crypt_Data), &CA_Buffer);

      memcpy(&scdata, CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer());

      CA_Buffer._Destroy();
      CA_Buffer._Create(1, scdata.pData - sizeof(S_Crypt_Data));

      ///////////////////////
      // Read Name
      if(!pfile_i->read(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);
      // Decrypt Name
      CAES.decrypt(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer(), &CA_Buffer2);

      ///////////////////////
      // Append File Name

      S_Crypt_VFile SCVFile;
      
      SCVFile.cData = scdata.cData;
      SCVFile.pData = DH_Offset;
      
      memset(SCVFile.Name, 0, sizeof(SCVFile.Name));
      memset(SCVFile.Path, 0, sizeof(SCVFile.Path));
      
      if(CA_Buffer2._Get_cBuffer() > sizeof(SCVFile.Path) - 1) return(C_CRYPT_ERROR);
      memcpy(SCVFile.Path, CA_Buffer2._Get_pBuffer(), CA_Buffer2._Get_cBuffer());
      
      string tstring = SCVFile.Path;
      size_t found = tstring.rfind("/");
      found++;
      string str2 = tstring.substr(found, tstring.size() - found);
      
      if(str2.size() > sizeof(SCVFile.Name) - 1) return(C_CRYPT_ERROR);
      memcpy(SCVFile.Name, str2.c_str(), str2.size());
      
      if(!(pCA_File = pDA_File->_Add_Item(1, sizeof(SCVFile)))) return(C_CRYPT_ERROR);
      memcpy(pCA_File->_Get_pBuffer(), (void*)&SCVFile, sizeof(SCVFile));

      unsigned long cData = scdata.cData;
      
      while(cData % 16) cData++;

      pfile_i->seekg(cData, pfile_i->cur);
   }
   
   return(C_CRYPT_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [extract_file_encrypt]  
//////////////////////////////////////////////////////////////////////////////////
int C_Crypt::extract_file_encrypt(S_Crypt_VFile* pSCVFile, fstream* pfile_in, fstream* pfile_out){
   S_Crypt_Main scmain;
   S_Crypt_Data scdata;
   C_Array CA_Buffer, CA_Buffer2, CA_Buffer3;
   
   if(this->status != C_CRYPT_READY || !pSCVFile || !pfile_in || !pfile_out) return(C_CRYPT_ERROR);
   
   /////////////////////////////
   // Set File Pointer
   if(!pfile_in->seekg(pSCVFile->pData, pfile_in->beg)) return(C_CRYPT_ERROR);
   
   ///////////////////////
   // Read Data Head
   if(!pfile_in->read((char*)&scdata, sizeof(S_Crypt_Data))) return(C_CRYPT_ERROR);
   // Decrypt Data Head
   this->decrypt((char*)&scdata, sizeof(S_Crypt_Data), &CA_Buffer, C_CRYPT_NEW_KEY);
   
   memcpy(&scdata, CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer());
   
   //////////////////////
   
   CA_Buffer._Destroy();
   CA_Buffer._Create(1, scdata.pData - sizeof(S_Crypt_Data));

   ///////////////////////
   // Read Name
   if(!pfile_in->read(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);
   // Decrypt Name
   this->decrypt(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer(), &CA_Buffer2, C_CRYPT_OLD_KEY);
   
   ///////////////////////

   string str5 = CA_Buffer2._Get_pBuffer();
   size_t found = str5.rfind("/");
   string str2 = str5.substr(found, str5.size() - found);

   //////////////////////
   
   unsigned long cData = scdata.cData;
   
   while(cData % 16) cData++;
   
   CA_Buffer3._Destroy();
   CA_Buffer3._Create(1, cData);

   ///////////////////////
   // Read Data
   if(!pfile_in->read(CA_Buffer3._Get_pBuffer(), cData)) return(C_CRYPT_ERROR);
   // Decrypt Data
   this->decrypt(CA_Buffer3._Get_pBuffer(), CA_Buffer3._Get_cBuffer(), &CA_Buffer2, C_CRYPT_OLD_KEY);

   ////////////////////////////
   // Fill Main Head
   scmain.Version = C_CRYPT_VERSION;
   scmain.Type    = C_CRYPT_TYPE_FILE;
   scmain.nData   = 0x01;
   scmain.cData   = pSCVFile->cData;
   
   ////////////////////////////
   // Crypt Main Head
   this->encrypt((char*)&scmain, sizeof(S_Crypt_Main), &CA_Buffer, C_CRYPT_NEW_KEY);
   // Write Main Head
   if(!pfile_out->write(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);
   
   ////////////////////////////
   // Get Name
   int   cName = str2.length();
   char* pName = (char*)str2.c_str();

   while(cName % 16) cName++; // align the Name
   
   ////////////////////////////
   // Fill Data Head
   scdata.pData = sizeof(S_Crypt_Data) + cName;
   
   ////////////////////////////
   // Crypt Data Head
   this->encrypt((char*)&scdata, sizeof(S_Crypt_Data), &CA_Buffer, C_CRYPT_OLD_KEY);
   // Write Data Head
   if(!pfile_out->write(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);
   
   ////////////////////////////
   // Crypt Name
   this->encrypt(pName, cName, &CA_Buffer, C_CRYPT_OLD_KEY);
   // Write Name
   if(!pfile_out->write(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);

   ////////////////////////////
   // Crypt Data
   this->encrypt(CA_Buffer2._Get_pBuffer(), CA_Buffer2._Get_cBuffer(), &CA_Buffer, C_CRYPT_OLD_KEY);
   // Write Data
   if(!pfile_out->write(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);

   return(C_CRYPT_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [extract_file_encrypt]  
//////////////////////////////////////////////////////////////////////////////////
int C_Crypt::extract_file_decrypt(S_Crypt_VFile* pSCVFile, fstream* pfile_in, string* pPath){
   S_Crypt_Data scdata;
   C_Array CA_Buffer, CA_Buffer2;
   fstream os;
   
   if(this->status != C_CRYPT_READY || !pSCVFile || !pfile_in || !pPath) return(C_CRYPT_ERROR);
   
   //////////////////////////////////////////////
   // Set File Pointer
   if(!pfile_in->seekg(pSCVFile->pData, pfile_in->beg)) return(C_CRYPT_ERROR);
   
   ///////////////////////
   // Read Data Head
   if(!pfile_in->read((char*)&scdata, sizeof(S_Crypt_Data))) return(C_CRYPT_ERROR);
   // Decrypt Data Head
   this->decrypt((char*)&scdata, sizeof(S_Crypt_Data), &CA_Buffer, C_CRYPT_NEW_KEY);
   
   memcpy(&scdata, CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer());
   
   //////////////////////
   
   CA_Buffer._Destroy();
   CA_Buffer._Create(1, scdata.pData - sizeof(S_Crypt_Data));

   ///////////////////////
   // Read Name
   if(!pfile_in->read(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);
   // Decrypt Name
   this->decrypt(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer(), &CA_Buffer2, C_CRYPT_OLD_KEY);
   
   ///////////////////////
   // TEST
   char psName[PATH_MAX] = {0x00};
   memset(psName, 0x00, sizeof(psName));
   memcpy(psName, CA_Buffer2._Get_pBuffer(), CA_Buffer2._Get_cBuffer());

   string str4 = *pPath;
   string str5 = psName;
   size_t found = str5.rfind("/");
   string str2 = str5.substr(found, str5.size() - found);
   str4.append(str2);

   ///////////////////////
   // Create File
   os.open(str4.c_str(), fstream::in | fstream::out | fstream::app);
   if(!os.is_open()) return(C_CRYPT_ERROR);
   
   unsigned long cData = scdata.cData;
      
   while(cData % 16) cData++;

   ////////////////////////////
   // Read File to Buffer
   CA_Buffer._Destroy();
   CA_Buffer._Create(1, cData);
   
   ///////////////////////
   // Read Data
   if(!pfile_in->read(CA_Buffer._Get_pBuffer(), cData)) return(C_CRYPT_ERROR);
   // Decrypt Data
   this->decrypt(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer(), &CA_Buffer2, C_CRYPT_OLD_KEY);

   ///////////////////////
   // Write Data
   if(!os.write(CA_Buffer2._Get_pBuffer(), scdata.cData)) return(C_CRYPT_ERROR);

   ///////////////////////
   // Close File
   os.close();

   return(C_CRYPT_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [extract_folder_encrypt]  
//////////////////////////////////////////////////////////////////////////////////
int C_Crypt::extract_folder_encrypt(C_DArray* pCDA_Folder, C_DArray* pCDA_Files, fstream* pfile_in, fstream* pfile_out, string* pFolder){
   S_Crypt_Main scmain;
   S_Crypt_Data scdata;
   C_Array CA_Buffer, CA_Buffer2, CA_Buffer3;
   
   if(this->status != C_CRYPT_READY || !pCDA_Folder || !pCDA_Files || !pfile_in || !pfile_out || !pFolder) return(C_CRYPT_ERROR);
   
   /////////////////////////////////////////////////////////////////////////////////////
   // Write Main Head
   scmain.Version = C_CRYPT_VERSION;
   scmain.Type    = C_CRYPT_TYPE_FOLDER;
   scmain.cData   = 0x01; // TEST
   scmain.nData   = pCDA_Files->_Get_nItems();
   scmain.nFolder = pCDA_Folder->_Get_nItems() + 1; // + Root Folder
   
   ////////////////////////////
   // Crypt Main Head
   this->encrypt((char*)&scmain, sizeof(S_Crypt_Main), &CA_Buffer, C_CRYPT_NEW_KEY);
   // Write Main Head
   if(!pfile_out->write(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);
   
   /////////////////////////////////////////////////////////////////////
   // Encrypt Root Folder
   
   char psFolder[PATH_MAX] = {0x00};
   
   string str5  = *pFolder;
   size_t found = str5.rfind("/");
   string str4  = str5.substr(found, str5.size() - found);

   unsigned long cName = str4.size();
   if(cName > PATH_MAX - 1) return(C_CRYPT_ERROR);
   
   memset(psFolder, 0x00, sizeof(psFolder)); 
   memcpy(psFolder, str4.c_str(), cName);
      
   while(cName % 16) cName++; // align the Name
   
   /////////////////////////////////////////////////////////////////////
   // Write DataHead
   scdata.Version = C_CRYPT_VERSION;
   scdata.Type    = C_CRYPT_TYPE_FOLDER;
   scdata.cData   = 0x00;
   scdata.pData   = sizeof(S_Crypt_Data) + cName;
   
   ////////////////////////////
   // Crypt Data Head
   this->encrypt((char*)&scdata, sizeof(S_Crypt_Data), &CA_Buffer, C_CRYPT_NEW_KEY);
   // Write Data Head
   if(!pfile_out->write(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);
   
   ////////////////////////////
   // Crypt Data
   this->encrypt(psFolder, cName, &CA_Buffer, C_CRYPT_OLD_KEY);
   // Write Data
   if(!pfile_out->write(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);
   
   /////////////////////////////////////////////////////////////////////
   // Encrypt Folder
   for(int n = 0; n < pCDA_Folder->_Get_nItems(); n++){
      S_C_DArray* pTemp = pCDA_Folder->_Get_pItem(n);
      S_Crypt_VFolder* pSCVFolder = (S_Crypt_VFolder*)pTemp->_pData->_Get_pBuffer();    
      
      string str5 = pSCVFolder->Path;
      string str4 = str5.substr(found, str5.size() - found);
   
      cName = str4.size();
      if(cName > PATH_MAX - 1) return(C_CRYPT_ERROR);
      
      memset(psFolder, 0x00, sizeof(psFolder)); 
      memcpy(psFolder, str4.c_str(), cName);
      
      while(cName % 16) cName++; // align the Name
      
      /////////////////////////////////////////////////////////////////////
      // Write DataHead
      scdata.Version = C_CRYPT_VERSION;
      scdata.Type    = C_CRYPT_TYPE_FOLDER;
      scdata.cData   = 0x00;
      scdata.pData   = sizeof(S_Crypt_Data) + cName;
      
      ////////////////////////////
      // Crypt Data Head
      this->encrypt((char*)&scdata, sizeof(S_Crypt_Data), &CA_Buffer, C_CRYPT_NEW_KEY);
      // Write Data Head
      if(!pfile_out->write(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);
      
      ////////////////////////////
      // Crypt Data
      this->encrypt(psFolder, cName, &CA_Buffer, C_CRYPT_OLD_KEY);
      // Write Data
      if(!pfile_out->write(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);
   }
   
   /////////////////////////////////////////////////////////////////////
   // Encrypt Files
   for(int m = 0; m < pCDA_Files->_Get_nItems(); m++){
      S_C_DArray* pTemp = pCDA_Files->_Get_pItem(m);
      S_Crypt_VFile* pSCVFile = (S_Crypt_VFile*)pTemp->_pData->_Get_pBuffer();  

      ///////////////////////////// 
      // Set File Pointer
      if(!pfile_in->seekg(pSCVFile->pData, pfile_in->beg)) return(C_CRYPT_ERROR); 
      
      ///////////////////////
      // Read Data Head
      if(!pfile_in->read((char*)&scdata, sizeof(S_Crypt_Data))) return(C_CRYPT_ERROR);
      // Decrypt Data Head
      this->decrypt((char*)&scdata, sizeof(S_Crypt_Data), &CA_Buffer, C_CRYPT_NEW_KEY);

      memcpy(&scdata, CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer());

      //////////////////////

      CA_Buffer._Destroy();
      CA_Buffer._Create(1, scdata.pData - sizeof(S_Crypt_Data));

      ///////////////////////
      // Read Name
      if(!pfile_in->read(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);
      // Decrypt Name
      this->decrypt(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer(), &CA_Buffer2, C_CRYPT_OLD_KEY);

      ///////////////////////

      unsigned long cData = scdata.cData;

      while(cData % 16) cData++; // TEST

      CA_Buffer3._Destroy();
      CA_Buffer3._Create(1, cData);
      
      ///////////////////////
      // Read Data
      if(!pfile_in->read(CA_Buffer3._Get_pBuffer(), cData)) return(C_CRYPT_ERROR);
      // Decrypt Data
      this->decrypt(CA_Buffer3._Get_pBuffer(), CA_Buffer3._Get_cBuffer(), &CA_Buffer2, C_CRYPT_OLD_KEY);
      
      ////////////////////////////
      // Get Name
      string str5 = pSCVFile->Path;
      string str4 = str5.substr(found, str5.size() - found);
      
      int   cName = str4.size();

      while(cName % 16) cName++; // align the Name
      
      ////////////////////////////
      // Fill Data Head
      scdata.pData = sizeof(S_Crypt_Data) + cName;
      
      ////////////////////////////
      // Crypt Data Head
      this->encrypt((char*)&scdata, sizeof(S_Crypt_Data), &CA_Buffer, C_CRYPT_NEW_KEY);
      // Write Data Head
      if(!pfile_out->write(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);
      
      ////////////////////////////
      // Crypt Name
      this->encrypt((char*)str4.c_str(), cName, &CA_Buffer, C_CRYPT_OLD_KEY);
      // Write Name
      if(!pfile_out->write(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);
      
      ////////////////////////////
      // Crypt Data
      this->encrypt(CA_Buffer2._Get_pBuffer(), scdata.cData, &CA_Buffer, C_CRYPT_OLD_KEY);
      // Write Data
      if(!pfile_out->write(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);
   }
   
   return(C_CRYPT_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [extract_folder_decrypt]  
//////////////////////////////////////////////////////////////////////////////////
int C_Crypt::extract_folder_decrypt(C_DArray* pCDA_Folder, C_DArray* pCDA_Files, fstream* pfile_in, string* pPath, string* pFolder){
   S_Crypt_Data scdata;
   C_Array CA_Buffer, CA_Buffer2;
   fstream os;
   
   if(this->status != C_CRYPT_READY || !pCDA_Folder || !pCDA_Files || !pfile_in || !pPath || !pFolder) return(C_CRYPT_ERROR);

   //////////////////////////////////////////////
   // Make Root Folder
   string str2  = *pPath;
   string str5  = *pFolder;
   size_t found = str5.rfind("/");
   string str4  = str5.substr(found, str5.size() - found);
   str2.append(str4);
  
   if(CFSystem.makedir((char*)str2.c_str(), S_IRWXU) != C_FILE_SYSTEM_READY) return(C_CRYPT_ERROR);

   /////////////////////////////////////
   // Make Folder
   for(int n = 0; n < pCDA_Folder->_Get_nItems(); n++){
      S_C_DArray* pTemp = pCDA_Folder->_Get_pItem(n);
      S_Crypt_VFolder* pSCVFolder = (S_Crypt_VFolder*)pTemp->_pData->_Get_pBuffer();
      string str2 = *pPath;
      string str5 = pSCVFolder->Path;
      string str4 = str5.substr(found, str5.size() - found);
      str2.append(str4);
      if(CFSystem.makedir((char*)str2.c_str(), S_IRWXU) != C_FILE_SYSTEM_READY) return(C_CRYPT_ERROR);
   }

   /////////////////////////////////////////////////////
   // Extract Files
   for(int m = 0; m < pCDA_Files->_Get_nItems(); m++){
      S_C_DArray* pTemp = pCDA_Files->_Get_pItem(m);
      S_Crypt_VFile* pSCVFile = (S_Crypt_VFile*)pTemp->_pData->_Get_pBuffer();  
      
      ///////////////////////////////
      string str3  = *pPath;
      string str5  = pSCVFile->Path;
      string str4  = str5.substr(found, str5.size() - found);
      str3.append(str4);
      
      //////////////////////////////////////////////
      // Set File Pointer
      if(!pfile_in->seekg(pSCVFile->pData, pfile_in->beg)) return(C_CRYPT_ERROR);
      
      ///////////////////////
      // Read Data Head
      if(!pfile_in->read((char*)&scdata, sizeof(S_Crypt_Data))) return(C_CRYPT_ERROR);
      // Decrypt Data Head
      this->decrypt((char*)&scdata, sizeof(S_Crypt_Data), &CA_Buffer, C_CRYPT_NEW_KEY);
   
      memcpy(&scdata, CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer());
      
      //////////////////////
      CA_Buffer._Destroy();
      CA_Buffer._Create(1, scdata.pData - sizeof(S_Crypt_Data));
      
      ///////////////////////
      // Read Name
      if(!pfile_in->read(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);
      // Decrypt Name
      this->decrypt(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer(), &CA_Buffer2, C_CRYPT_OLD_KEY);
      
      ///////////////////////
      // Create File
      os.open(str3.c_str(), fstream::in | fstream::out | fstream::app);
      if(!os.is_open()) return(C_CRYPT_ERROR);
      
      unsigned long cData = scdata.cData;
      
      while(cData % 16) cData++;

      ////////////////////////////
      // Read File to Buffer
      CA_Buffer._Destroy();
      CA_Buffer._Create(1, cData);
      
      ///////////////////////
      // Read Data
      if(!pfile_in->read(CA_Buffer._Get_pBuffer(), cData)) return(C_CRYPT_ERROR);
      // Decrypt Data
      this->decrypt(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer(), &CA_Buffer2, C_CRYPT_OLD_KEY);

      ///////////////////////
      // Write Data
      if(!os.write(CA_Buffer2._Get_pBuffer(), scdata.cData)) return(C_CRYPT_ERROR);

      ///////////////////////
      // Close File
      os.close();
   }

   return(C_CRYPT_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [add_file]  
//////////////////////////////////////////////////////////////////////////////////
int C_Crypt::add_file(fstream* pfile_in, fstream* pfile_out, string* pPath, string* pFile){
   S_Crypt_Main scmain;
   C_Array CA_Buffer;
   
   if(this->status != C_CRYPT_READY || !pfile_in || !pfile_out || !pPath || !pFile) return(C_CRYPT_ERROR);

   ///////////////////////////////////////
   // Get File Size
   pfile_in->seekg(0, pfile_in->end);
   int cData = pfile_in->tellg();
   pfile_in->seekg(0, pfile_in->beg);

   ///////////////////////////////////////
   
   string str2  = *pPath;
   string str5  = *pFile;
   size_t found = str5.rfind("/");
   string str4  = str5.substr(found, str5.size() - found);
   str2.append(str4);

   ///////////////////////////////////////
   
   pfile_out->seekg(0, pfile_out->beg); // Set File Pointer START
   
   ///////////////////////
   // Read Main Head
   if(!pfile_out->read((char*)&scmain, sizeof(S_Crypt_Main))) return(C_CRYPT_ERROR);
   // Decrypt Main Head
   this->decrypt((char*)&scmain, sizeof(S_Crypt_Main), &CA_Buffer, C_CRYPT_NEW_KEY);
   
   memcpy(&scmain, CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer());

   ////////////////////////////
   // Change Main Head
   scmain.cData += cData;
   scmain.nData++;
   
   ///////////////////////////////////////
   
   pfile_out->seekg(0, pfile_out->beg); // Set File Pointer START
   
   ////////////////////////////
   // Crypt Main Head
   this->encrypt((char*)&scmain, sizeof(S_Crypt_Main), &CA_Buffer, C_CRYPT_NEW_KEY);
   // Write Main Head
   if(!pfile_out->write(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);

   ///////////////////////////////////////
   
   pfile_out->seekg(0, pfile_out->end); // Set File Pointer ENDE
   
   ///////////////////////////////////////
   // TEST
   CAES.C_Rijndael.MakeKey(this->key.c_str(), this->key.c_str(), 16, 16);
   
   if(this->encrypt_file(pfile_in, pfile_out, &str2, C_CRYPT_TYPE_FOLDER) != C_CRYPT_READY) return(C_CRYPT_ERROR);

   return(C_CRYPT_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [add_folder]  
//////////////////////////////////////////////////////////////////////////////////
int C_Crypt::add_folder(fstream* pfile_out, string* pPath, string* pFolder){
   S_Crypt_Main scmain;
   S_Crypt_Data scdata; 
   C_DArray CDA_Folder, CDA_Files, CDA_CFolder, CDA_CFiles;
   C_Array CA_Temp, CA_Buffer;
   unsigned long cData = 0;
   S_C_DArray* pTemp = 0;
   streampos Offset = sizeof(S_Crypt_Main); // Offset -> First DataHead(Folder)
   
   if(this->status != C_CRYPT_READY || !pfile_out || !pPath || !pFolder) return(C_CRYPT_ERROR);

   /////////////////////////////////////////////////////////////////////
   
   if(this->getDataHeads(pfile_out, &CDA_CFolder, &CDA_CFiles) != C_CRYPT_READY) return(C_CRYPT_ERROR);

   for(unsigned long nFolder = 0; nFolder < CDA_CFolder._Get_nItems(); nFolder++){
      pTemp = CDA_CFolder._Get_pItem(nFolder);
      Offset += ((S_Crypt_Data*)pTemp->_pData->_Get_pBuffer())->pData; // !?!?!
   }
   
   // Offset -> First DataHead(File)

   ///////////////////////////////////////
   // Get File Size
   pfile_out->seekg(0, pfile_out->end);
   int cFData = pfile_out->tellg();
   pfile_out->seekg(0, pfile_out->beg);
   
   ////////////////////////////////////////
   // Read Encrypted Files -> CA_Temp
   
   int cFiles = cFData - Offset; // cFiles = Size(Encrypted Files)
   CA_Temp._Create(1, cFiles);
   pfile_out->seekg(Offset, pfile_out->beg); // File Pointer -> First DataHead(File)
   if(!pfile_out->read(CA_Temp._Get_pBuffer(), CA_Temp._Get_cBuffer())) return(C_CRYPT_ERROR);
   pfile_out->seekg(Offset, pfile_out->beg); // File Pointer -> First DataHead(File)

   /////////////////////////////////////////////////////////////////////
   // Get New Files and Folders
   // Get the Size 
   if(CFSystem.rdir((char*)pFolder->c_str(), &CDA_Folder, &CDA_Files) != C_FILE_SYSTEM_READY) 
      return(C_CRYPT_ERROR);
   if(CFSystem.getrdirsize((char*)pFolder->c_str(), &cData) != C_FILE_SYSTEM_READY) 
      return(C_CRYPT_ERROR);
   
   /////////////////////////////////////////////////////////////////////
   // Append Root Folder
   
   char psFolder[PATH_MAX] = {0x00};
	 
   size_t found = pFolder->rfind("/");
   
   string str2 = *pFolder;
   string str4 = str2.substr(found, str2.size() - found);
   string str3 = *pPath;
   
   str3.append(str4);
   
   unsigned long cName = str3.size();
   if(cName > PATH_MAX - 1) return(C_CRYPT_ERROR);
   
   memset(psFolder, 0x00, sizeof(psFolder));
   memcpy(psFolder, str3.c_str(), cName);
   
   while(cName % 16) cName++; // align the Name
   
   /////////////////////////////////////////////////////////////////////
   // Write Data Head
   scdata.Version = C_CRYPT_VERSION;
   scdata.Type    = C_CRYPT_TYPE_FOLDER;
   scdata.cData   = 0x00;
   scdata.pData   = sizeof(S_Crypt_Data) + cName;
   
   ////////////////////////////
   // TEST
   CAES.C_Rijndael.MakeKey(this->key.c_str(), this->key.c_str(), 16, 16);
   ////////////////////////////
   // Crypt Data Head
   CAES.encrypt((char*)&scdata, sizeof(S_Crypt_Data), &CA_Buffer);
   // Write Data Head
   if(!pfile_out->write(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);

   ////////////////////////////
   // Crypt Data
   CAES.encrypt(psFolder, cName, &CA_Buffer);
   // Write Data
   if(!pfile_out->write(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);

   /////////////////////////////////////////////////////////////////////
   // Append Folder
   for(unsigned long nFolder = 0; nFolder < CDA_Folder._Get_nItems(); nFolder++){
      pTemp = CDA_Folder._Get_pItem(nFolder);
      
      string str2 = pTemp->_pData->_Get_pBuffer();
      string str4 = str2.substr(found, str2.size() - found);
      string str3 = *pPath;
      
      str3.append(str4);

      unsigned long cName = str3.size();
      if(cName > PATH_MAX - 1) return(C_CRYPT_ERROR);
   
      memset(psFolder, 0x00, sizeof(psFolder));
      memcpy(psFolder, str3.c_str(), cName);
   
      while(cName % 16) cName++; // align the Name

      /////////////////////////////////////////////////////////////////////
      // Write Data Head
      scdata.Version = C_CRYPT_VERSION;
      scdata.Type    = C_CRYPT_TYPE_FOLDER;
      scdata.cData   = 0x00;
      scdata.pData   = sizeof(S_Crypt_Data) + cName;
      
      ////////////////////////////
      // TEST
      CAES.C_Rijndael.MakeKey(this->key.c_str(), this->key.c_str(), 16, 16);
      ////////////////////////////
      // Crypt Data Head
      CAES.encrypt((char*)&scdata, sizeof(S_Crypt_Data), &CA_Buffer);
      // Write Data Head
      if(!pfile_out->write(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);

      ////////////////////////////
      // Crypt Data
      CAES.encrypt(psFolder, cName, &CA_Buffer);
      // Write Data
      if(!pfile_out->write(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);
   }
   
   /////////////////////////////
   // Write Encrypted Files Back to File
   
   if(!pfile_out->write(CA_Temp._Get_pBuffer(), CA_Temp._Get_cBuffer())) return(C_CRYPT_ERROR);   
   
   /////////////////////////////////////////////////////////////////////
   // Apend New Files
   fstream file_in;
   
   for(unsigned long nFiles = 0; nFiles < CDA_Files._Get_nItems(); nFiles++){
      pTemp = CDA_Files._Get_pItem(nFiles);
      
      string str2 = pTemp->_pData->_Get_pBuffer();
      string str4 = str2.substr(found, str2.size() - found);
      string str3 = *pPath;
      
      str3.append(str4);
 
      file_in.open(pTemp->_pData->_Get_pBuffer(), fstream::in);
      if(!file_in.is_open()) return(C_CRYPT_ERROR);
      
      ////////////////////////////
      // TEST
      CAES.C_Rijndael.MakeKey(this->key.c_str(), this->key.c_str(), 16, 16);
      ////////////////////////////
      // Crypt File
      if(this->encrypt_file(&file_in, pfile_out, &str3, C_CRYPT_TYPE_FOLDER) != C_CRYPT_READY) return(C_CRYPT_ERROR);
      
      file_in.close();
   }
   
   /////////////////////////////////////////////////////////////////////////////////
   // Set File Pointer START
   pfile_out->seekg(0, pfile_out->beg); 
   
   ///////////////////////
   // Read Main Head
   if(!pfile_out->read((char*)&scmain, sizeof(S_Crypt_Main))) return(C_CRYPT_ERROR);
   // Decrypt Main Head
   this->decrypt((char*)&scmain, sizeof(S_Crypt_Main), &CA_Buffer, C_CRYPT_NEW_KEY);
   
   memcpy(&scmain, CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer());
   
   ////////////////////////////
   // Change Main Head
   scmain.cData   += cData;
   scmain.nData   += CDA_Files._Get_nItems();
   scmain.nFolder += CDA_Folder._Get_nItems() + 1;
   
   ///////////////////////////////////////
   // Set File Pointer START
   pfile_out->seekg(0, pfile_out->beg); 
   
   ////////////////////////////
   // Crypt Main Head
   this->encrypt((char*)&scmain, sizeof(S_Crypt_Main), &CA_Buffer, C_CRYPT_NEW_KEY);
   // Write Main Head
   if(!pfile_out->write(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);

   return(C_CRYPT_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [del_file]  
//////////////////////////////////////////////////////////////////////////////////
int C_Crypt::del_file(S_Crypt_VFile* pSCVFile, fstream* pfile, string* pname, int modus){
   S_Crypt_Main scmain;
   S_Crypt_Data scdata;
   C_Array CA_Temp, CA_Buffer;

   if(this->status != C_CRYPT_READY || !pSCVFile || !pfile || !pname) return(C_CRYPT_ERROR);

   ///////////////////////////////////////
   // Get File Size
   pfile->seekg(0, pfile->end);
   int cFData = pfile->tellg();
   pfile->seekg(0, pfile->beg);
   
   ////////////////////////////////////////
   
   pfile->seekg(pSCVFile->pData, pfile->beg); // File Pointer -> DataHead(pSCVFile)
   
   ///////////////////////
   // Read Data Head
   if(!pfile->read((char*)&scdata, sizeof(S_Crypt_Data))) return(C_CRYPT_ERROR);
   // Decrypt Data Head
   this->decrypt((char*)&scdata, sizeof(S_Crypt_Data), &CA_Buffer, C_CRYPT_NEW_KEY);
   
   memcpy(&scdata, CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer());
   
   int cData = scdata.cData;
   
   while(cData % 16) cData++;
   
   ////////////////////////////////////////
   // Read Encrypted Files to Buffer
   pfile->seekg(pSCVFile->pData + cData + scdata.pData, pfile->beg); // File Pointer -> Next DataHead(pSCVFile)
   int cFData2 = pfile->tellg();
   
   int cFiles = cFData - cFData2; // cFiles = Size(Next Encrypted Files)
   CA_Temp._Create(1, cFiles);
   
   if(!pfile->read(CA_Temp._Get_pBuffer(), CA_Temp._Get_cBuffer())) return(C_CRYPT_ERROR);
   
   ////////////////////////////////////////
   
   pfile->seekg(pSCVFile->pData, pfile->beg); // File Pointer -> DataHead(pSCVFile)
   
   /////////////////////////////
   // Write Encrypted Files Back to File
   
   if(!pfile->write(CA_Temp._Get_pBuffer(), CA_Temp._Get_cBuffer())) return(C_CRYPT_ERROR);   
   
   int cFDataNew = pfile->tellg(); // New File Size
   
   if(modus){
      /////////////////////////////////////////////////////////////////////////////////
      // Set File Pointer START
      pfile->seekg(0, pfile->beg); 
   
      ///////////////////////
      // Read Main Head
      if(!pfile->read((char*)&scmain, sizeof(S_Crypt_Main))) return(C_CRYPT_ERROR);
      // Decrypt Main Head
      this->decrypt((char*)&scmain, sizeof(S_Crypt_Main), &CA_Buffer, C_CRYPT_NEW_KEY);
   
      memcpy(&scmain, CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer());
   
      ////////////////////////////
      // Change Main Head
      scmain.cData -= scdata.cData;
      scmain.nData--;
   
      ///////////////////////////////////////
      // Set File Pointer START
      pfile->seekg(0, pfile->beg); 
   
      ////////////////////////////
      // Crypt Main Head
      this->encrypt((char*)&scmain, sizeof(S_Crypt_Main), &CA_Buffer, C_CRYPT_NEW_KEY);
      // Write Main Head
      if(!pfile->write(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);
   }
   
   /////////////////////////////
   // TEST int truncate(const char *path, off_t length);
   // truncate -> linux | _chsize -> windows
   
   if(truncate(pname->c_str(), cFDataNew)) return(C_CRYPT_ERROR);
   
   return(C_CRYPT_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [del_folder]  
//////////////////////////////////////////////////////////////////////////////////
int C_Crypt::del_folder(C_DArray* pCDA_Folder, C_DArray* pCDA_Files, string* pPath, fstream* pfile, string* pname){
   C_DArray CDA_Files, CDA_Folder;
   S_Crypt_Main scmain;
   S_Crypt_Data scdata;
   C_Array CA_Temp, CA_Buffer;
   
   if(this->status != C_CRYPT_READY || !pCDA_Folder || !pCDA_Files || !pPath || !pfile || !pname) return(C_CRYPT_ERROR);

   //////////////////////////////////////////////
   // Read File to Buffer
   if(this->getData(pfile, &CDA_Folder, &CDA_Files) != C_CRYPT_READY) return(C_CRYPT_ERROR);
   
   //////////////////////////////////////////////
   // Delete Old File, Create New File
   pfile->close();
   
   if(CFSystem.delfile(pname->c_str()) != C_FILE_SYSTEM_READY) return(C_CRYPT_ERROR);
   
   pfile->open(pname->c_str(), fstream::in | fstream::out | fstream::app);
   if(!pfile->is_open()) return(C_CRYPT_ERROR);

   /////////////////////////////////////////////////////////////////////
   // Write MainHead
   scmain.Version = C_CRYPT_VERSION;
   scmain.Type    = C_CRYPT_TYPE_FOLDER;
   scmain.cData   = 0x01;
   scmain.nData   = CDA_Files._Get_nItems()  - pCDA_Files->_Get_nItems();
   scmain.nFolder = CDA_Folder._Get_nItems() - pCDA_Folder->_Get_nItems() - 1;
   
   ////////////////////////////
   // Crypt Main Head
   this->encrypt((char*)&scmain, sizeof(S_Crypt_Main), &CA_Buffer, C_CRYPT_NEW_KEY);
   // Write Main Head
   if(!pfile->write(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);

   ////////////////////////////
   // Check Folder
   for(unsigned long nFolder = 0; nFolder < CDA_Folder._Get_nItems(); nFolder++){
      S_C_DArray* pTemp = CDA_Folder._Get_pItem(nFolder);

      ////////////////////////////
      // Decrypt Data Head
      this->decrypt(pTemp->_pData->_Get_pBuffer(), sizeof(S_Crypt_Data), &CA_Buffer, C_CRYPT_NEW_KEY);
      memcpy(&scdata, CA_Buffer._Get_pBuffer(), sizeof(S_Crypt_Data));
      
      // Check Data Head
      if(scdata.Version != C_CRYPT_VERSION) return(C_CRYPT_ERROR);
      
      ////////////////////////////
      // Create Name Buffer
      CA_Temp._Destroy();
      CA_Temp._Create(1, scdata.pData - sizeof(S_Crypt_Data));
      
      ////////////////////////////
      // Copy Name
      memcpy(CA_Temp._Get_pBuffer(), pTemp->_pData->_Get_pBuffer() + sizeof(S_Crypt_Data), scdata.pData - sizeof(S_Crypt_Data));
      // Decrypt Name
      this->decrypt(CA_Temp._Get_pBuffer(), scdata.pData - sizeof(S_Crypt_Data), &CA_Buffer, C_CRYPT_OLD_KEY);

      string str = CA_Buffer._Get_pBuffer();
      bool bkeep = true;
      
      /////////////////////////////
      // Check if delete or not
      for(unsigned long n = 0; n < pCDA_Folder->_Get_nItems(); n++){
	 S_C_DArray* pTemp = pCDA_Folder->_Get_pItem(n);
	 S_Crypt_VFolder* pSCVFolder = (S_Crypt_VFolder*)pTemp->_pData->_Get_pBuffer();
	 
	 string str2 = pSCVFolder->Path;

	 if(str == str2 || str == *pPath){
	    bkeep = false;
	    break;
	 }
      }
      
      if(!pCDA_Folder->_Get_nItems()) if(str == *pPath) bkeep = false;

      /////////////////////////////
      // Copy to New File
      if(bkeep) if(!pfile->write(pTemp->_pData->_Get_pBuffer(), pTemp->_pData->_Get_cBuffer())) return(C_CRYPT_ERROR);
   }

   ////////////////////////////
   // Check Files
   for(unsigned long nFiles = 0; nFiles < CDA_Files._Get_nItems(); nFiles++){
      S_C_DArray* pTemp = CDA_Files._Get_pItem(nFiles);
      
      ////////////////////////////
      // Decrypt Data Head
      this->decrypt(pTemp->_pData->_Get_pBuffer(), sizeof(S_Crypt_Data), &CA_Buffer, C_CRYPT_NEW_KEY);
      memcpy(&scdata, CA_Buffer._Get_pBuffer(), sizeof(S_Crypt_Data));
      
      // Check Data Head
      if(scdata.Version != C_CRYPT_VERSION) return(C_CRYPT_ERROR);
      
      ////////////////////////////
      // Create Name Buffer
      CA_Temp._Destroy();
      CA_Temp._Create(1, scdata.pData - sizeof(S_Crypt_Data));
      
      ////////////////////////////
      // Copy Name
      memcpy(CA_Temp._Get_pBuffer(), pTemp->_pData->_Get_pBuffer() + sizeof(S_Crypt_Data), scdata.pData - sizeof(S_Crypt_Data));
      // Decrypt Name
      this->decrypt(CA_Temp._Get_pBuffer(), scdata.pData - sizeof(S_Crypt_Data), &CA_Buffer, C_CRYPT_OLD_KEY);

      string str = CA_Buffer._Get_pBuffer();
      bool bkeep = true;
      
      /////////////////////////////
      // Check if delete or not
      for(unsigned long n = 0; n < pCDA_Files->_Get_nItems(); n++){
	 S_C_DArray* pTemp = pCDA_Files->_Get_pItem(n);
	 S_Crypt_VFile* pSCVFiles = (S_Crypt_VFile*)pTemp->_pData->_Get_pBuffer();
	 
	 string str2 = pSCVFiles->Path;

	 if(str == str2){
	    bkeep = false;
	    break;
	 }
      }
      /////////////////////////////
      // Copy to New File
      if(bkeep) if(!pfile->write(pTemp->_pData->_Get_pBuffer(), pTemp->_pData->_Get_cBuffer())) return(C_CRYPT_ERROR);
   }
   
   /////////////////////////////////
   // Close and Reopen the File
   pfile->close();
   pfile->open(pname->c_str(), fstream::in | fstream::out);
   if(!pfile->is_open()) return(C_CRYPT_ERROR);
   
   return(C_CRYPT_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [new_file]  
//////////////////////////////////////////////////////////////////////////////////
int C_Crypt::new_file(fstream* pfile, string* pname){
   S_Crypt_Main scmain;
   S_Crypt_Data scdata;
   C_Array CA_Buffer, CA_Temp;
   
   if(this->status != C_CRYPT_READY || !pfile || !pname) return(C_CRYPT_ERROR);
  
   ///////////////////////
   // Set File Pointer START
   pfile->seekg(0, pfile->beg);
   
   ///////////////////////
   // Read Main Head
   if(!pfile->read((char*)&scmain, sizeof(S_Crypt_Main))) return(C_CRYPT_ERROR);
   // Decrypt Main Head
   this->decrypt((char*)&scmain, sizeof(S_Crypt_Main), &CA_Buffer, C_CRYPT_NEW_KEY);
   
   memcpy(&scmain, CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer());

   ////////////////////////////
   // Check Version
   if(scmain.Version != C_CRYPT_VERSION) return(C_CRYPT_ERROR);
   
   /////////////////////////////////////////////////////////////////////////////////
   // Set File Pointer START
   pfile->seekg(0, pfile->beg);
   
   ////////////////////////////
   // Change Main Head
   scmain.nData++;
   
   ////////////////////////////
   // Crypt Main Head
   this->encrypt((char*)&scmain, sizeof(S_Crypt_Main), &CA_Buffer, C_CRYPT_NEW_KEY);
   // Write Main Head
   if(!pfile->write(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);
   
   /////////////////////////////////////////////////////////////////////////////////
   // Set File Pointer END
   pfile->seekg(0, pfile->end);
   
   /////////////////////////////////////////////////////////////////////
   // Append File
   char psFile[PATH_MAX] = {0x00};
   
   unsigned long cName = pname->size();
   if(cName > PATH_MAX - 1) return(C_CRYPT_ERROR);
   
   memset(psFile, 0x00, sizeof(psFile));
   memcpy(psFile, pname->c_str(), cName);
   
   while(cName % 16) cName++; // align the Name
   
   ////////////////////////////
   // Fill Data Head
   scdata.Version = C_CRYPT_VERSION;
   scdata.Type    = C_CRYPT_TYPE_FILE;
   scdata.pData   = sizeof(S_Crypt_Data) + cName;
   scdata.cData   = 0x00;
   
   ////////////////////////////
   // Crypt Data Head
   this->encrypt((char*)&scdata, sizeof(S_Crypt_Data), &CA_Buffer, C_CRYPT_NEW_KEY);
   // Write Data Head
   if(!pfile->write(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);
   
   ////////////////////////////
   // Crypt Name
   this->encrypt(psFile, cName, &CA_Buffer, C_CRYPT_OLD_KEY);
   // Write Name
   if(!pfile->write(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);
   
   return(C_CRYPT_READY); 
}
//////////////////////////////////////////////////////////////////////////////////
// [new_folder]  
//////////////////////////////////////////////////////////////////////////////////
int C_Crypt::new_folder(fstream* pfile, string* pname){
   S_Crypt_Main scmain;
   S_Crypt_Data scdata;
   C_Array CA_Buffer, CA_Temp;
   streampos Offset = 0;
   
   if(this->status != C_CRYPT_READY || !pfile || !pname) return(C_CRYPT_ERROR);

   ///////////////////////
   // Set File Pointer START
   pfile->seekg(Offset, pfile->beg);
   
   ///////////////////////
   // Read Main Head
   if(!pfile->read((char*)&scmain, sizeof(S_Crypt_Main))) return(C_CRYPT_ERROR);
   // Decrypt Main Head
   this->decrypt((char*)&scmain, sizeof(S_Crypt_Main), &CA_Buffer, C_CRYPT_NEW_KEY);
   
   memcpy(&scmain, CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer());

   ////////////////////////////
   // Check Version
   if(scmain.Version != C_CRYPT_VERSION) return(C_CRYPT_ERROR);

   Offset += sizeof(S_Crypt_Main);
   
   ////////////////////////////
   // Read Folder
   for(unsigned long nFolder = 0; nFolder < scmain.nFolder; nFolder++){

      ///////////////////////
      // Set File Pointer
      pfile->seekg(Offset, pfile->beg);
     
      ///////////////////////
      // Read Data Head
      if(!pfile->read((char*)&scdata, sizeof(S_Crypt_Data))) return(C_CRYPT_ERROR);
      // Decrypt Data Head
      this->decrypt((char*)&scdata, sizeof(S_Crypt_Data), &CA_Buffer, C_CRYPT_NEW_KEY);
   
      memcpy(&scdata, CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer());
      
      ////////////////////////////
      // Check Version
      if(scdata.Version != C_CRYPT_VERSION) return(C_CRYPT_ERROR);
      
      ////////////////////////////
      Offset += scdata.pData;
   }
   
   ///////////////////////////////////////
   // Get File Size
   pfile->seekg(0, pfile->end);
   int cFData = pfile->tellg();
   pfile->seekg(0, pfile->beg);
  
   ////////////////////////////////////////
   // Read Encrypted Files -> CA_Temp
   int cFiles = cFData - Offset; // cFiles = Size(Encrypted Files)
   CA_Temp._Create(1, cFiles);
   pfile->seekg(Offset, pfile->beg); // File Pointer -> First DataHead(File)
   if(!pfile->read(CA_Temp._Get_pBuffer(), CA_Temp._Get_cBuffer())) return(C_CRYPT_ERROR);
   pfile->seekg(Offset, pfile->beg); // File Pointer -> First DataHead(File)
   
   /////////////////////////////////////////////////////////////////////
   // Append Folder
   char psFolder[PATH_MAX] = {0x00};
   
   unsigned long cName = pname->size();
   if(cName > PATH_MAX - 1) return(C_CRYPT_ERROR);
   
   memset(psFolder, 0x00, sizeof(psFolder));
   memcpy(psFolder, pname->c_str(), cName);
   
   while(cName % 16) cName++; // align the Name
   
   /////////////////////////////////////////////////////////////////////
   // Write Data Head
   scdata.Version = C_CRYPT_VERSION;
   scdata.Type    = C_CRYPT_TYPE_FOLDER;
   scdata.cData   = 0x00;
   scdata.pData   = sizeof(S_Crypt_Data) + cName;
   
   ////////////////////////////
   // Crypt Data Head
   this->encrypt((char*)&scdata, sizeof(S_Crypt_Data), &CA_Buffer, C_CRYPT_NEW_KEY);
   // Write Data Head
   if(!pfile->write(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);
   
   ////////////////////////////
   // Crypt Data
   this->encrypt(psFolder, cName, &CA_Buffer, C_CRYPT_OLD_KEY);
   // Write Data
   if(!pfile->write(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);

   /////////////////////////////
   // Write Encrypted Files Back to File
   if(!pfile->write(CA_Temp._Get_pBuffer(), CA_Temp._Get_cBuffer())) return(C_CRYPT_ERROR);  
   
   /////////////////////////////////////////////////////////////////////////////////
   // Set File Pointer START
   pfile->seekg(0, pfile->beg);
   
   ////////////////////////////
   // Change Main Head
   scmain.nFolder++;
   
   ////////////////////////////
   // Crypt Main Head
   this->encrypt((char*)&scmain, sizeof(S_Crypt_Main), &CA_Buffer, C_CRYPT_NEW_KEY);
   // Write Main Head
   if(!pfile->write(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);
   
   return(C_CRYPT_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [edit_file]  
//////////////////////////////////////////////////////////////////////////////////
int C_Crypt::edit_file(fstream* pfile, unsigned long pData, string* pStrData, string* pname){
   S_Crypt_Data scdata;
   C_Array CA_Buffer, CA_Buffer2;
   
   if(this->status != C_CRYPT_READY || !pfile || !pData || !pStrData || !pname) return(C_CRYPT_ERROR);

   //////////////////////////////////////////////
   // Set File Pointer
   if(!pfile->seekg(pData, pfile->beg)) return(C_CRYPT_ERROR);
   
   ///////////////////////
   // Read Data Head
   if(!pfile->read((char*)&scdata, sizeof(S_Crypt_Data))) return(C_CRYPT_ERROR);
   // Decrypt Data Head
   this->decrypt((char*)&scdata, sizeof(S_Crypt_Data), &CA_Buffer, C_CRYPT_NEW_KEY);
   
   memcpy(&scdata, CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer());
   
   //////////////////////
   
   CA_Buffer._Destroy();
   CA_Buffer._Create(1, scdata.pData - sizeof(S_Crypt_Data));

   ///////////////////////
   // Read Name
   if(!pfile->read(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);
   // Decrypt Name
   this->decrypt(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer(), &CA_Buffer2, C_CRYPT_OLD_KEY);
   
   ///////////////////////
   // TEST
   char psPath[PATH_MAX] = {0x00};
   memset(psPath, 0x00, sizeof(psPath));
   memcpy(psPath, CA_Buffer2._Get_pBuffer(), CA_Buffer2._Get_cBuffer());
   
   string name = psPath;
   
   //cout << "Path: " << psPath << endl;
   //cout << "Data: " << *pStrData << endl;
   
   //////////////////////////////////////////////////////////
   // TEST
   S_Crypt_VFile SCVFile;
   SCVFile.pData = pData;
   
   this->del_file(&SCVFile, pfile, pname, 0);
   
   /////////////////////////////////////////////////////////////////////////////////
   // Set File Pointer END
   pfile->seekg(0, pfile->end);
   
   /////////////////////////////////////////////////////////////////////
   // Append File
   char psFile[PATH_MAX] = {0x00};
   
   unsigned long cName = name.size();
   if(cName > PATH_MAX - 1) return(C_CRYPT_ERROR);
   
   memset(psFile, 0x00, sizeof(psFile));
   memcpy(psFile, name.c_str(), cName);
   
   while(cName % 16) cName++; // align the Name
   
   ////////////////////////////
   // Fill Data Head
   scdata.Version = C_CRYPT_VERSION;
   scdata.Type    = C_CRYPT_TYPE_FILE;
   scdata.pData   = sizeof(S_Crypt_Data) + cName;
   scdata.cData   = pStrData->size();
   
   ////////////////////////////
   // Crypt Data Head
   this->encrypt((char*)&scdata, sizeof(S_Crypt_Data), &CA_Buffer, C_CRYPT_NEW_KEY);
   // Write Data Head
   if(!pfile->write(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);
   
   ////////////////////////////
   // Crypt Name
   this->encrypt(psFile, cName, &CA_Buffer, C_CRYPT_OLD_KEY);
   // Write Name
   if(!pfile->write(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);
   
   ////////////////////////////
   // Crypt Data
   this->encrypt((char*)pStrData->c_str(), pStrData->size(), &CA_Buffer, C_CRYPT_OLD_KEY);
   // Write Data
   if(!pfile->write(CA_Buffer._Get_pBuffer(), CA_Buffer._Get_cBuffer())) return(C_CRYPT_ERROR);
   
   return(C_CRYPT_READY);
}