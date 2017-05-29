//////////////////////////////////////////////////////////////////////////////////
// [ File_System_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
//
// [::Last modi: 03.10.16 L.ey (µ~)::]  
//
// 
// [::alpha 0.1]
//
#ifndef _C_FILE_SYSTEM_H_
 #define _C_FILE_SYSTEM_H_
 
  #include <iostream>
  #include <fstream>
  
  using namespace std;

  #include <sys/stat.h>
  #include <sys/statvfs.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <dirent.h>

  #include "C_DArray.hpp"

//////////////////////////////////////////////////////////////////////////////////

  static const int C_FILE_SYSTEM_READY  = 0x01;
  static const int C_FILE_SYSTEM_ERROR  = 0x00;

//////////////////////////////////////////////////////////////////////////////////

 class C_File_System {
    public:

       C_File_System(){};  
      ~C_File_System(){};  

       //////////////////////////////////

       int dir(char* psPath, C_DArray* pDA_Folder, C_DArray* pDA_File);
       int makedir(const char* psPath, mode_t mode);
       int deldir(const char* psPath);
       
       int copyfile(const char* psSource, const char* psDest);
       int delfile(const char* psPath);
       int movefile(const char* psSource, const char* psDest);
       
       //////////////////////////////////
       // (r) == recursion
       
       int rdir(char* psPath, C_DArray* pDA_Folder, C_DArray* pDA_File);
       int rcopydir(char* psSource, char* psDest);
       int rmovedir(char* psSource, char* psDest);
       int rdeldir(char* psSource);
       
       int getrdirsize(char* psPath, unsigned long* pSize);

    protected:

       int rmakedir(C_DArray* pDAFolder);
       int rcopyfile(C_DArray* pDA_Source, C_DArray* pDA_Dest);
       int rdelfile(C_DArray* pDA_File);
       int rdeletedir(C_DArray* pDA_Folder);
       
       int getfreespace(char* psPath, unsigned long* pFree);
 };

#endif // _C_FILE_SYSTEM_H_