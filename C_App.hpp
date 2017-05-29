//////////////////////////////////////////////////////////////////////////////////
// [ APP_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
// APP
//
// [::Last modi: 26.10.16 L.ey (µ~)::]
//
//
#ifndef _C_APP_H_
 #define _C_APP_H_
 
   #include <iostream>
   #include <fstream>
   #include <string>
   #include <sstream>

   using namespace std;

   #include "C_GTK.hpp"
 
   #include "C_Crypt.hpp"
   
   #include "C_Thread.hpp"
   
//////////////////////////////////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////////////////////////////////
 
   const int C_APP_ERROR = 0x00;
   const int C_APP_READY = 0x01;
   
   const int C_APP_TXT   = 0x00;
   const int C_APP_HEX   = 0x01;
   
   const int C_APP_REKURSION = 0x01;
   
   const int C_APP_TYPE_MORPH         = 0x00;
   const int C_APP_TYPE_ENC_FILE      = 0x01;
   const int C_APP_TYPE_DEC_FILE      = 0x02;
   const int C_APP_TYPE_ENC_FOLDER    = 0x03;
   const int C_APP_TYPE_DEC_FOLDER    = 0x04;
   const int C_APP_TYPE_EX_ENC_FILE   = 0x05;
   const int C_APP_TYPE_EX_DEC_FILE   = 0x06;
   const int C_APP_TYPE_EX_ENC_FOLDER = 0x07;
   const int C_APP_TYPE_EX_DEC_FOLDER = 0x08;
   const int C_APP_TYPE_ADD_FILE      = 0x09;
   const int C_APP_TYPE_DEL_FILE      = 0x0A;
   const int C_APP_TYPE_ADD_FOLDER    = 0x0B;
   const int C_APP_TYPE_DEL_FOLDER    = 0x0C;
   const int C_APP_TYPE_ADD_DROP      = 0x0D;
   
   const char WORKING[] = "Working...";
   
   const char ERROR_DECRYPT_FILE_V[] = "Error: Decrypt File V";
   const char READY_DECRYPT_FILE_V[] = "Ready: Decrypt File V";
   const char ERROR_FILE_NOT_FOUND[] = "Error: File Not Found";
   
   const char ERROR_THREAD[]     = "Error: CThread";
   const char ERROR_NO_FILE[]    = "Error: No File";
   const char ERROR_NO_FOLDER[]  = "Error: No File or Folder";
   
   const char ERROR_SHOW_HEX[]   = "Error: Show Hex";
   const char READY_SHOW_HEX[]   = "Ready: Show Hex";
   
   const char ERROR_UNKNOWN[]    = "Error: Unknown";
   const char READY_UNKNOWN[]    = "Ready: Unknown";
   const char ERROR_OPEN_FILE[]  = "Error: Open File";
   const char READY_OPEN_FILE[]  = "Ready: Open File";
   const char ERROR_EDIT_FILE[]  = "Error: Edit File";
   const char READY_EDIT_FILE[]  = "Ready: Edit File";
   const char ERROR_SAVE_FILE[]  = "Error: Save File";
   const char READY_SAVE_FILE[]  = "Ready: Save File";
   const char ERROR_ENC_FILE[]   = "Error: Encrypt File";
   const char READY_ENC_FILE[]   = "Ready: Encrypt File";
   const char ERROR_DEC_FILE[]   = "Error: Decrypt File";
   const char READY_DEC_FILE[]   = "Ready: Decrypt File";
   const char ERROR_ENC_FOLDER[] = "Error: Encrypt Folder";
   const char READY_ENC_FOLDER[] = "Ready: Encrypt Folder";
   const char ERROR_DEC_FOLDER[] = "Error: Decrypt Folder";
   const char READY_DEC_FOLDER[] = "Ready: Decrypt Folder";
   const char ERROR_EX_FILE[]    = "Error: Extract File";
   const char READY_EX_FILE[]    = "Ready: Extract File";
   const char ERROR_EX_FOLDER[]  = "Error: Extract Folder";
   const char READY_EX_FOLDER[]  = "Ready: Extract Folder";
   const char ERROR_ADD_FILE[]   = "Error: Add File";
   const char READY_ADD_FILE[]   = "Ready: Add File";
   const char ERROR_DEL_FILE[]   = "Error: Del File";
   const char READY_DEL_FILE[]   = "Ready: Del File";
   const char ERROR_NEW_FILE[]   = "Error: New File";
   const char READY_NEW_FILE[]   = "Ready: New File";
   const char ERROR_ADD_FOLDER[] = "Error: Add Folder";
   const char READY_ADD_FOLDER[] = "Ready: Add Folder";
   const char ERROR_DEL_FOLDER[] = "Error: Del Folder";
   const char READY_DEL_FOLDER[] = "Ready: Del Folder";
   const char ERROR_NEW_FOLDER[] = "Error: New Folder";
   const char READY_NEW_FOLDER[] = "Ready: New Folder";
   const char ERROR_ADD_DROP[]   = "Error: Add Drop";
   const char READY_ADD_DROP[]   = "Ready: Add Drop";
   
//////////////////////////////////////////////////////////////////////////////////
// STRUCT
//////////////////////////////////////////////////////////////////////////////////
   
   struct S_Edit_File{
      GtkWidget*     pWindow;
      GtkTextBuffer* pTextBuffer;
      unsigned long  pData;
      GtkWidget*     pQuit;
      GtkWidget*     pSave;
   };
   
   
   struct S_Thread_Result{
      int   type;
      int   result;
      void* pClass;
   };
   
//////////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////////
   
 class C_App : public C_GTK {
   
     public:

        C_App();
       ~C_App();

	//////////////////////////////////////////////////////////////////////////////
       
        int  open_file();
        int  open_folder();

        void encrypt();
        void decrypt();
	
        int  encrypt_file();
        int  decrypt_file();
	
        int  encrypt_folder();

	//////////////////////////////////////////////////////////////////////////////
	
        int  decrypt_filev();
        int  decrypt_filevF(unsigned long pData, int modus);
   
        //////////////////////////////////////////////////////////////////////////////

        int  getTreeByPath(char* psName, GtkTreeIter* pTreeIter);
        int  getPathByTree(GtkTreeIter* pTreeIter, string* pPath);
        int  getNameByTree(GtkTreeIter* pTreeIter, string* pName);

        int  getFilesByPath(char* psPath, C_DArray* pDA_Files, int modus);
        int  getFileByOffset(unsigned long pData, S_Crypt_VFile* pSCVFile);

        int  getFolderByPath(char* psPath, C_DArray* pDA_Folder, int modus);

	//////////////////////////////////////////////////////////////////////////////
	
        int  extract_file_encrypt(unsigned long pData);
        int  extract_file_decrypt(unsigned long pData, char* pDestination);
	
        int  extract_folder_encrypt(string* pStrPath);
        int  extract_folder_decrypt(string* pStrPath, char* pDestination);
	
	//////////////////////////////////////////////////////////////////////////////
	
        int  del_file(unsigned long pData);
        int  add_file(string* pStrPath);
        int  new_file(string* pStrPath);
	
        int  del_folder(string* pStrPath);
        int  add_folder(string* pStrPath);
        int  new_folder(string* pStrPath);
   
	//////////////////////////////////////////////////////////////////////////////
	
	int  edit_file(unsigned long pData);
	int  save_file(unsigned long pData, string* pStrData);
	
	//////////////////////////////////////////////////////////////////////////////
	
        void dmessage(const char* pMessage);
	int  morph_data(char* pData, unsigned long cData, GtkTextBuffer* pTBuffer);
	
	S_Edit_File SEditFile;

	/////////////////////////////////////////////////////////////////////////////
	
	S_Thread_Result STResult;
	
	void encrypt_file_thread();
	void decrypt_file_thread();
	
	void encrypt_folder_thread();
	
	/////////////////////////////////////////
	
	S_Crypt_VFile SCVFile;
	
	void extract_file_enc_thread();
	void extract_file_dec_thread();
	
	/////////////////////////////////////////
	
	C_DArray CDA_Folder;
	C_DArray CDA_Files;
	
	string StrPath;
	
	void extract_folder_enc_thread();
	void extract_folder_dec_thread();
	
	/////////////////////////////////////////
	
        fstream is_new;
        string  in_new;
   
	void add_file_thread();
	void del_file_thread();
	
	void add_folder_thread();
	void del_folder_thread();
	
	/////////////////////////////////////////
	
	C_Array CAMorphBuffer;
	GtkTextBuffer* pTextBuffer;
	
	void morph_data_thread();
	
	/////////////////////////////////////////
	
	string StrExpandPath;
	bool bExpand;
	GtkTreeIter gTreeIter;

	/////////////////////////////////////////
	
	C_DArray CDA_AddFolder;
	C_DArray CDA_AddFiles;
	
	int  add_drop();
	void add_drop_thread();
	
	/////////////////////////////////////////
	//
	static void thread_exit(void* data){
	   g_idle_add((GSourceFunc)process, data);
	   
	   ((C_App*)((S_Thread_Result*)data)->pClass)->CThread.set_bRunning(false);
	}
	
	/////////////////////////////////////////
	//
	static gboolean process(void *arg){
	   const char* pMessage = 0;
	   
	   if(((S_Thread_Result*)arg)->result != C_APP_READY){
	      switch(((S_Thread_Result*)arg)->type){
		 case C_APP_TYPE_ENC_FILE:      pMessage = ERROR_ENC_FILE;   break;
		 case C_APP_TYPE_DEC_FILE:      pMessage = ERROR_DEC_FILE;   break;
		 case C_APP_TYPE_ENC_FOLDER:    pMessage = ERROR_ENC_FOLDER; break;
		 case C_APP_TYPE_EX_DEC_FILE:   pMessage = ERROR_EX_FILE;    break;
		 case C_APP_TYPE_EX_ENC_FILE:   pMessage = ERROR_EX_FILE;    break;
		 case C_APP_TYPE_EX_DEC_FOLDER: pMessage = ERROR_EX_FOLDER;  break;
		 case C_APP_TYPE_EX_ENC_FOLDER: pMessage = ERROR_EX_FOLDER;  break;
		 case C_APP_TYPE_ADD_FILE:      pMessage = ERROR_ADD_FILE;   break;
                 case C_APP_TYPE_DEL_FILE:      pMessage = ERROR_DEL_FILE;   break;
		 case C_APP_TYPE_ADD_FOLDER:    pMessage = ERROR_ADD_FOLDER; break;
		 case C_APP_TYPE_DEL_FOLDER:    pMessage = ERROR_DEL_FOLDER; break;
		 case C_APP_TYPE_MORPH:         pMessage = ERROR_SHOW_HEX;   break;
		 case C_APP_TYPE_ADD_DROP:      pMessage = ERROR_ADD_DROP;   break;
		 default:                       pMessage = ERROR_UNKNOWN;    break;
	      }
	   }else{
	      switch(((S_Thread_Result*)arg)->type){
		 case C_APP_TYPE_ENC_FILE:      pMessage = READY_ENC_FILE;   break;
		 case C_APP_TYPE_DEC_FILE:      pMessage = READY_DEC_FILE;   break;
		 case C_APP_TYPE_ENC_FOLDER:    pMessage = READY_ENC_FOLDER; break;
		 case C_APP_TYPE_EX_DEC_FILE:   pMessage = READY_EX_FILE;    break;
		 case C_APP_TYPE_EX_ENC_FILE:   pMessage = READY_EX_FILE;    break;
		 case C_APP_TYPE_EX_DEC_FOLDER: pMessage = READY_EX_FOLDER;  break;
		 case C_APP_TYPE_EX_ENC_FOLDER: pMessage = READY_EX_FOLDER;  break;
                 case C_APP_TYPE_ADD_FILE:      pMessage = READY_ADD_FILE;   break;
		 case C_APP_TYPE_DEL_FILE:      pMessage = READY_DEL_FILE;   break;
		 case C_APP_TYPE_ADD_FOLDER:    pMessage = READY_ADD_FOLDER; break;
		 case C_APP_TYPE_DEL_FOLDER:    pMessage = READY_DEL_FOLDER; break;
		 case C_APP_TYPE_MORPH:         pMessage = READY_SHOW_HEX;   break;
		 case C_APP_TYPE_ADD_DROP:      pMessage = READY_ADD_DROP;   break;
		 default:                       pMessage = READY_UNKNOWN;    break;
	      }
	   }

	   C_App* pCApp = (C_App*)((S_Thread_Result*)arg)->pClass;
	   
           switch(((S_Thread_Result*)arg)->type){
	      case C_APP_TYPE_ADD_DROP:
	      case C_APP_TYPE_DEL_FOLDER:
	      case C_APP_TYPE_ADD_FOLDER:
	      case C_APP_TYPE_DEL_FILE:
              case C_APP_TYPE_ADD_FILE: pCApp->decrypt_filev(); break;
	      case C_APP_TYPE_MORPH:    pCApp->CTextView.setTextBuffer(pCApp->pTextBuffer); break;
	   }
	   
	   if(pCApp->bExpand){
	     
	      // TEST
	      cout << pCApp->StrExpandPath.c_str() << endl;
	     
	      if(pCApp->getTreeByPath((char*)pCApp->StrExpandPath.c_str(), &pCApp->gTreeIter) == C_APP_READY){
                 GtkTreePath* path = pCApp->CTreeView.getTreePathByIter(&pCApp->gTreeIter);
	         pCApp->CTreeView.expandRow(path);
	         pCApp->CTreeView.setSelectByTreePath(path);
		 pCApp->CTreeView.scrollToPath(path);
	         pCApp->CTreeView.setPathFree(path);
	      }
	      pCApp->bExpand = false;
	   }

           pCApp->dmessage(pMessage);
	   
	   gtk_spinner_stop(GTK_SPINNER(pCApp->spinner));
	   
           gtk_widget_queue_draw(pCApp->CWindow.getWindow());  
        }

        ///////////
        // TEST
        
        bool threadRunning(){return(CThread.get_bRunning());};
        
     private: 
       
        int getPassEncrypt(string* pKey);
        int getPassDecrypt(string* pKey);
	
	int getNewName(const char* pCaption, string* pNewName);
	
	int createEditWindow(unsigned long pData, C_Array* pCABuffer);
	
	//////////////////////////////////////////////////////
	
        C_Crypt CCrypt;
       
        bool bfile;
        bool bfolder;
	
        fstream is;
        string  in;
	
	fstream os;
        string  on;
   
	C_Thread <C_App> CThread;
	
        C_DArray CDA_VFolder;
	C_DArray CDA_VFiles;

        int status;
 };

#endif // _C_APP_H_