//////////////////////////////////////////////////////////////////////////////////
// [ File_System_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_File_System.hpp"

//////////////////////////////////////////////////////////////////////////////////
// [dir]
//////////////////////////////////////////////////////////////////////////////////
int C_File_System::dir(char* psPath, C_DArray* pDA_Folder, C_DArray* pDA_File){
  
   const unsigned char bFile   = 0x08;
   const unsigned char bFolder = 0x04;
   DIR*    dp = 0;
   dirent* de = 0;
   C_Array* pCA_Folder = 0;
   C_Array* pCA_File   = 0;
   
   if(!psPath || !pDA_Folder || !pDA_File) return(C_FILE_SYSTEM_ERROR);

   dp = opendir(psPath);
  
   if(dp){
      while(true){

         if((de = readdir(dp)) == NULL) break;

         switch(de->d_type){
            case bFile:

               if(!(pCA_File = pDA_File->_Add_Item(1, strlen(de->d_name) + 1))) return(C_FILE_SYSTEM_ERROR);
               memcpy(pCA_File->_Get_pBuffer(), de->d_name, strlen(de->d_name) + 1);
               break;

            case bFolder: 

               if((de->d_name[0x00] == '.' && de->d_name[0x01] == 0x00) ||
                  (de->d_name[0x00] == '.' && de->d_name[0x01] == '.' && de->d_name[0x02] == 0x00)) break;

               if(!(pCA_Folder = pDA_Folder->_Add_Item(1, strlen(de->d_name) + 1))) return(C_FILE_SYSTEM_ERROR);
               memcpy(pCA_Folder->_Get_pBuffer(), de->d_name, strlen(de->d_name) + 1);
               break;

            default:      
               break;
         }
      }
      closedir(dp);
   }

   return(C_FILE_SYSTEM_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [makedir]
//////////////////////////////////////////////////////////////////////////////////
int C_File_System::makedir(const char* psPath, mode_t mode){
   if(!psPath) return(C_FILE_SYSTEM_ERROR);
   if(mkdir(psPath, mode) != 0) return(C_FILE_SYSTEM_ERROR);
   return(C_FILE_SYSTEM_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [deldir]
//////////////////////////////////////////////////////////////////////////////////
int C_File_System::deldir(const char* psPath){
   if(!psPath) return(C_FILE_SYSTEM_ERROR);
   if(rmdir(psPath) != 0) return(C_FILE_SYSTEM_ERROR);
   return(C_FILE_SYSTEM_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [copyfile]
//////////////////////////////////////////////////////////////////////////////////
int C_File_System::copyfile(const char* psSource, const char* psDest){
   if(!psSource || !psDest) return(C_FILE_SYSTEM_ERROR);
   ifstream src(psSource, ios::binary);
   ofstream dst(psDest,   ios::binary);
   dst << src.rdbuf();
   return(C_FILE_SYSTEM_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [delfile]
//////////////////////////////////////////////////////////////////////////////////
int C_File_System::delfile(const char* psPath){
   if(!psPath) return(C_FILE_SYSTEM_ERROR);
   if(remove(psPath) != 0) return(C_FILE_SYSTEM_ERROR);
   return(C_FILE_SYSTEM_READY); 
}
//////////////////////////////////////////////////////////////////////////////////
// [movefile]
//////////////////////////////////////////////////////////////////////////////////
int C_File_System::movefile(const char* psSource, const char* psDest){
   if(this->copyfile(psSource, psDest) != C_FILE_SYSTEM_READY) return(C_FILE_SYSTEM_ERROR);
   if(this->delfile(psSource)          != C_FILE_SYSTEM_READY) return(C_FILE_SYSTEM_ERROR);
   return(C_FILE_SYSTEM_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [Rekursiv Function]
//////////////////////////////////////////////////////////////////////////////////
// [rdir] return DArray with (char)Path\FolderName and 
//               DArray with (char)Path\FileName
//////////////////////////////////////////////////////////////////////////////////
int C_File_System::rdir(char* psPath, C_DArray* pDA_Folder, C_DArray* pDA_File){
  
   const unsigned char bFile   = 0x08;
   const unsigned char bFolder = 0x04;
   DIR*    dp = 0;
   dirent* de = 0;
   C_Array* pCA_Folder = 0;
   C_Array* pCA_File   = 0;
   
   string spath;
   
   if(!psPath || !pDA_Folder || !pDA_File) return(C_FILE_SYSTEM_ERROR);
   
   spath.append(psPath);

   if(spath[spath.size() - 1] != '/') spath.append("/");
   
   //////////////////////////////////////////////////////////////
   
   dp = opendir(spath.c_str());
  
   if(dp){
      while(true){

         if((de = readdir(dp)) == NULL) break;

         switch(de->d_type){
            case bFile:

               if(!(pCA_File = pDA_File->_Add_Item(1, spath.size() + strlen(de->d_name) + 1))) return(C_FILE_SYSTEM_ERROR);
               memcpy(pCA_File->_Get_pBuffer(), spath.c_str(), spath.size());
               memcpy(pCA_File->_Get_pBuffer() + spath.size(), de->d_name, strlen(de->d_name) + 1);
               break;

            case bFolder: 

               if((de->d_name[0x00] == '.' && de->d_name[0x01] == 0x00) ||
                  (de->d_name[0x00] == '.' && de->d_name[0x01] == '.' && de->d_name[0x02] == 0x00)) break;

               if(!(pCA_Folder = pDA_Folder->_Add_Item(1, spath.size() + strlen(de->d_name) + 1))) return(C_FILE_SYSTEM_ERROR);
	       memcpy(pCA_Folder->_Get_pBuffer(), spath.c_str(), spath.size());
               memcpy(pCA_Folder->_Get_pBuffer() + spath.size(), de->d_name, strlen(de->d_name) + 1);
	       
	       this->rdir(pCA_Folder->_Get_pBuffer(), pDA_Folder, pDA_File); // rekursion
	       
               break;

            default:      
               break;
         }
      }
      closedir(dp);
   }
   
   return(C_FILE_SYSTEM_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [rcopydir]
//////////////////////////////////////////////////////////////////////////////////
int C_File_System::rcopydir(char* psSource, char* psDest){
  
   C_DArray DA_Folder;
   C_DArray DA_File;
   C_DArray DA_MkDir;
   C_DArray DA_CpFile;
   
   S_C_DArray* pTemp = 0;
   
   C_Array* pCA_Folder = 0;
   C_Array* pCA_MkDir  = 0;

   C_Array* pCA_Files  = 0;
   C_Array* pCA_CPFile = 0;
   
   char* psResult = 0;
   char* psMkDir  = 0;
   char* psFileD  = 0;

   string spath, dpath;
   
   ///////////////////////////////////////////////////////////////////////
   
   if(!psSource || !psDest) return(C_FILE_SYSTEM_ERROR);
  
   spath.append(psSource);
   if(spath[spath.size() - 1] != '/') spath.append("/");
   
   dpath.append(psDest);
   if(dpath[dpath.size() - 1] != '/') dpath.append("/");
   
   ///////////////////////////////////////////////////////////////////////
   
   if(this->rdir(psSource, &DA_Folder, &DA_File) != C_FILE_SYSTEM_READY) return(C_FILE_SYSTEM_ERROR);
  
   pCA_MkDir = DA_MkDir._Add_Item(1, dpath.size() + 1);
   psMkDir   = pCA_MkDir->_Get_pBuffer();
   
   memcpy(psMkDir, dpath.c_str(), dpath.size());
   
   for(unsigned long nFolder = 0; nFolder < DA_Folder._Get_nItems(); nFolder++){
      pTemp     = DA_Folder._Get_pItem(nFolder);
      psResult  = pTemp->_pData->_Get_pBuffer();
      pCA_MkDir = DA_MkDir._Add_Item(1, dpath.size() + spath.size() - strlen(psResult) + 1);
      psMkDir   = pCA_MkDir->_Get_pBuffer();
      memcpy(psMkDir, dpath.c_str(), dpath.size());
      memcpy(&psMkDir[dpath.size()], &psResult[spath.size()], strlen(&psResult[spath.size()]) + 1);
   }
   
   if(this->rmakedir(&DA_MkDir) != C_FILE_SYSTEM_READY) return(C_FILE_SYSTEM_ERROR);

   ///////////////////////////////////////////////////////////////////////

   for(unsigned long nFiles = 0; nFiles < DA_File._Get_nItems(); nFiles++){
      S_C_DArray* pCA_F = DA_File._Get_pItem(nFiles);
      psResult   = pCA_F->_pData->_Get_pBuffer(); 
      pCA_CPFile = DA_CpFile._Add_Item(1, dpath.size() + strlen(psResult) + 1);
      psFileD    = pCA_CPFile->_Get_pBuffer();
      memcpy(psFileD, dpath.c_str(), dpath.size());
      memcpy(&psFileD[dpath.size()], &psResult[spath.size()], strlen(&psResult[spath.size()]) + 1);
   }

   if(this->rcopyfile(&DA_File, &DA_CpFile) != C_FILE_SYSTEM_READY) return(C_FILE_SYSTEM_ERROR);
   
   return(C_FILE_SYSTEM_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [rmakedir]
//////////////////////////////////////////////////////////////////////////////////
int C_File_System::rmakedir(C_DArray* pDAFolder){
  
   S_C_DArray* pTemp = 0;
  
   if(!pDAFolder) return(C_FILE_SYSTEM_ERROR);
  
   for(unsigned long nFolder = 0; nFolder < pDAFolder->_Get_nItems(); nFolder++){
      pTemp = pDAFolder->_Get_pItem(nFolder);
      if(mkdir(pTemp->_pData->_Get_pBuffer(), S_IRWXU) != 0) return(C_FILE_SYSTEM_ERROR);
   }
  
   return(C_FILE_SYSTEM_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [rcopyfile]
//////////////////////////////////////////////////////////////////////////////////
int C_File_System::rcopyfile(C_DArray* pDA_S, C_DArray* pDA_D){
  
   S_C_DArray* pCA_S = 0;
   S_C_DArray* pCA_D = 0;
   
   if(!pDA_S || !pDA_D) return(C_FILE_SYSTEM_ERROR);
   
   for(DWORD nFiles = 0; nFiles < pDA_S->_Get_nItems(); nFiles++){
     
      pCA_S = pDA_S->_Get_pItem(nFiles);
      pCA_D = pDA_D->_Get_pItem(nFiles);
      
      if(this->copyfile(pCA_S->_pData->_Get_pBuffer(), pCA_D->_pData->_Get_pBuffer()) != C_FILE_SYSTEM_READY) 
	 return(C_FILE_SYSTEM_ERROR);
   }

   return(C_FILE_SYSTEM_READY); 
}
//////////////////////////////////////////////////////////////////////////////////
// [rmovedir]
//////////////////////////////////////////////////////////////////////////////////
int C_File_System::rmovedir(char* psSource, char* psDest){
  
   if(!psSource || !psDest) return(C_FILE_SYSTEM_ERROR);
   
   if(this->rcopydir(psSource, psDest) != C_FILE_SYSTEM_READY) return(C_FILE_SYSTEM_ERROR);
   
   if(this->rdeldir(psSource) != C_FILE_SYSTEM_READY) return(C_FILE_SYSTEM_ERROR);
  
   return(C_FILE_SYSTEM_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [rmovedir]
//////////////////////////////////////////////////////////////////////////////////
int C_File_System::rdeldir(char* psSource){
  
   C_DArray DA_Folder, DA_File;
   C_Array* pCA_Folder = 0;
   
   if(!psSource) return(C_FILE_SYSTEM_ERROR);
   
   pCA_Folder = DA_Folder._Add_Item(1, strlen(psSource) + 1);
   if(!pCA_Folder) return(C_FILE_SYSTEM_ERROR);

   memcpy(pCA_Folder->_Get_pBuffer(), psSource, strlen(psSource) + 1);
   
   if(this->rdir(psSource, &DA_Folder, &DA_File) != C_FILE_SYSTEM_READY) return(C_FILE_SYSTEM_ERROR);
   
   if(this->rdelfile(&DA_File) != C_FILE_SYSTEM_READY) return(C_FILE_SYSTEM_ERROR);
   
   if(this->rdeletedir(&DA_Folder) != C_FILE_SYSTEM_READY) return(C_FILE_SYSTEM_ERROR);
   
   return(C_FILE_SYSTEM_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [rdelfile]
//////////////////////////////////////////////////////////////////////////////////
int C_File_System::rdelfile(C_DArray* pDA_File){
  
   S_C_DArray* pTemp = 0;
  
   if(!pDA_File) return(C_FILE_SYSTEM_ERROR);
  
   for(unsigned long nFile = 0; nFile < pDA_File->_Get_nItems(); nFile++){
     
      pTemp = pDA_File->_Get_pItem(nFile);
     
      if(this->delfile(pTemp->_pData->_Get_pBuffer()) != C_FILE_SYSTEM_READY) return(C_FILE_SYSTEM_ERROR);
   }

   return(C_FILE_SYSTEM_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [rdeletedir]
//////////////////////////////////////////////////////////////////////////////////
int C_File_System::rdeletedir(C_DArray* pDA_Folder){
  
   S_C_DArray* pTemp = 0;
 
   if(!pDA_Folder) return(C_FILE_SYSTEM_ERROR);  
  
   unsigned long cFolder = pDA_Folder->_Get_nItems();
  
   for(unsigned long nFolder = cFolder - 1; nFolder > 0; nFolder--){
     
      pTemp = pDA_Folder->_Get_pItem(nFolder);
     
      if(this->deldir(pTemp->_pData->_Get_pBuffer()) != C_FILE_SYSTEM_READY) return(C_FILE_SYSTEM_ERROR);
   }
  
   pTemp = pDA_Folder->_Get_pItem(0); // Root Folder
   
   if(this->deldir(pTemp->_pData->_Get_pBuffer()) != C_FILE_SYSTEM_READY) return(C_FILE_SYSTEM_ERROR);
  
   return(C_FILE_SYSTEM_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [getrdirsize]
//////////////////////////////////////////////////////////////////////////////////
int C_File_System::getrdirsize(char* psPath, unsigned long* pSize){
  
   const unsigned char bFile   = 0x08;
   const unsigned char bFolder = 0x04;
   DIR*    dp = 0;
   dirent* de = 0;

   string spath, dpath;
   
   if(!psPath || !pSize) return(C_FILE_SYSTEM_ERROR);
   
   spath.append(psPath);

   if(spath[spath.size() - 1] != '/') spath.append("/");
   
   //////////////////////////////////////////////////////////////
   
   dp = opendir(spath.c_str());
  
   if(dp){
      while(true){

         if((de = readdir(dp)) == NULL) break;

         switch(de->d_type){
            case bFile:

	       dpath.clear();
	       dpath.append(spath);
	       dpath.append(de->d_name);
	       
	       struct stat st;
               stat(dpath.c_str(), &st);

	       (*pSize) += st.st_size;
	       
               break;

            case bFolder: 

               if((de->d_name[0x00] == '.' && de->d_name[0x01] == 0x00) ||
                  (de->d_name[0x00] == '.' && de->d_name[0x01] == '.' && de->d_name[0x02] == 0x00)) break;

	       dpath.clear();
	       dpath.append(spath);
	       dpath.append(de->d_name);
	       
	       this->getrdirsize((char*)dpath.c_str(), pSize); // rekursion
	       
               break;

            default:      
               break;
         }
      }
      closedir(dp);
   }
   return(C_FILE_SYSTEM_READY);   
}
//////////////////////////////////////////////////////////////////////////////////
// [getfreespace]
//////////////////////////////////////////////////////////////////////////////////
int C_File_System::getfreespace(char* psPath, unsigned long* pFree){
  
   struct statvfs stat;
  
   if(!psPath || !pFree) return(C_FILE_SYSTEM_ERROR);
  
   if(statvfs(psPath, &stat) != 0) return(C_FILE_SYSTEM_ERROR);
  
   (*pFree) = (unsigned long)stat.f_bavail * stat.f_frsize;
  
   return(C_FILE_SYSTEM_READY); 
}
