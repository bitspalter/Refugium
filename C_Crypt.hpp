//////////////////////////////////////////////////////////////////////////////////
// [ Crypt_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
// CRYPT
//
// [::Last modi: 23.11.16 L.ey (µ~)::]
//
// [::beta 0.1]
//
#ifndef _C_CRYPT_H_
 #define _C_CRYPT_H_
 
   #include <iostream>
   #include <fstream>
 
   using namespace std;

   #include <gtk/gtk.h>
 
   #include "C_AES.hpp"
 
   #include "C_File_System.hpp"

//////////////////////////////////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////////////////////////////////
 
   const int C_CRYPT_ERROR   = 0x00;
   const int C_CRYPT_READY   = 0x01;
   
   const int C_CRYPT_OLD_KEY = 0x00;
   const int C_CRYPT_NEW_KEY = 0x01;
   
   const unsigned long  C_CRYPT_SIZE        = 1024 * 1024 * 2;
   
   const unsigned long  C_CRYPT_ENCRYPT     = 0x01;
   const unsigned long  C_CRYPT_DECRYPT     = 0x02;

   const unsigned short C_CRYPT_VERSION     = 0xF5F5;

   const unsigned short C_CRYPT_TYPE_FILE   = 0x01;
   const unsigned short C_CRYPT_TYPE_FOLDER = 0x02;
  
//////////////////////////////////////////////////////////////////////////////////
// STRUCT
//////////////////////////////////////////////////////////////////////////////////
  
   struct S_Crypt_Main{
      unsigned short Version;
      unsigned short Type;
      unsigned int   nData;
      unsigned long  cData;
      unsigned int   nFolder;
      unsigned int   dwReserve;
      unsigned long  ulReserve;
   }; // 32 bytes

   struct S_Crypt_Data{
      unsigned short Version;
      unsigned short Type;
      unsigned int   pData;
      unsigned long  cData;
      // char psName[n * 16]
   }; // 16 bytes + psname
   
   ////////////////////////////////////////////////////
   // VIRTUAL
   ////////////////////////////////////////////////////
   
   struct S_Crypt_VFolder{
      char Name[NAME_MAX];
      char Path[PATH_MAX];
      GtkTreeIter TreeIter;
   };
   
   struct S_Crypt_VFile{
      char Name[NAME_MAX];
      char Path[PATH_MAX];
      unsigned long cData;
      unsigned long pData;
   };
  
//////////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////////

class C_Crypt {

   public:

      C_Crypt(){this->status = C_CRYPT_ERROR;};
     ~C_Crypt(){};

      int setKey(string* pkey);
      int getType(fstream* pfile_in);

      int getDataHeads(fstream* pfile, C_DArray* pDA_Folder, C_DArray* pDA_File);
      int getData(fstream* pfile, C_DArray* pDA_Folder, C_DArray* pDA_File);

      int encrypt(char* pData, unsigned long cData, C_Array* pCA_out, int modus);
      int decrypt(char* pData, unsigned long cData, C_Array* pCA_out, int modus);

      int encrypt_file(fstream* pfile_in, fstream* pfile_out, string* pname_in, int modus);
      int decrypt_file(fstream* pfile_in, fstream* pfile_out, string* pname_in, string* pname_out);

      int decrypt_filev(fstream* pfile_in, C_Array* pCA_out);
      int decrypt_filev(fstream* pfile_in, unsigned long pData, C_Array* pCA_out);

      int decrypt_folderv(fstream* pfile_in, C_DArray* pDA_Folder, C_DArray* pDA_File);

      int encrypt_folder(fstream* pfile_out, string* pname_in);
      int decrypt_folder(S_Crypt_Main* pSCMainHead, fstream* pfile_i, unsigned long cData, string* pname_in, string* pname_out);

      int extract_file_encrypt(S_Crypt_VFile* pSCVFile, fstream* pfile_in, fstream* pfile_out);
      int extract_file_decrypt(S_Crypt_VFile* pSCVFile, fstream* pfile_in, string* pPath);

      int extract_folder_encrypt(C_DArray* pCDA_Folder, C_DArray* pCDA_Files, fstream* pfile_in, fstream* pfile_out, string* pFolder);
      int extract_folder_decrypt(C_DArray* pCDA_Folder, C_DArray* pCDA_Files, fstream* pfile_in, string* pPath, string* pFolder);

      int edit_file(fstream* pfile, unsigned long pData, string* pStrData, string* pname);

      int add_file(fstream* pfile_in, fstream* pfile_out, string* pPath, string* pFile);
      int del_file(S_Crypt_VFile* pSCVFile, fstream* pfile, string* pname, int modus);
      int new_file(fstream* pfile, string* pname);

      int add_folder(fstream* pfile_out, string* pPath, string* pFolder);
      int del_folder(C_DArray* pCDA_Folder, C_DArray* pCDA_Files, string* pPath, fstream* pFile, string* pName);
      int new_folder(fstream* pfile, string* pname);

   private:

      C_AES CAES;

      C_File_System CFSystem;

      string key;

      int status;  
};

#endif // _C_CRYPT_H_