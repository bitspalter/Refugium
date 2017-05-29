//////////////////////////////////////////////////////////////////////////////////
// [ APP_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_App.hpp"

//////////////////////////////////////////////////////////////////////////////////
// [ Konstructor ]  
//////////////////////////////////////////////////////////////////////////////////
C_App::C_App(){
   this->status  = C_APP_ERROR;
   this->bfile   = false;
   this->bfolder = false;
}
//////////////////////////////////////////////////////////////////////////////////
// [ Destructor ]
//////////////////////////////////////////////////////////////////////////////////
C_App::~C_App(){

  
}
//////////////////////////////////////////////////////////////////////////////////
// [ open_folder ]
//////////////////////////////////////////////////////////////////////////////////
int C_App::open_folder(){

   bfile   = false;
   bfolder = false;
   
   //////////////////////////////////////////////

   if(CFileChooser.getFolder("OPEN FOLDER", GTK_WINDOW(CWindow.getWindow()), &in) != C_GTK_FILECHOOSER_READY)
      return(C_APP_ERROR);

   //////////////////////////////////////////////

   slabel.path.setText(in.c_str());

   bfolder = true;

   return(C_APP_READY); 
}
//////////////////////////////////////////////////////////////////////////////////
// [ open_file ]
//////////////////////////////////////////////////////////////////////////////////
int C_App::open_file(){

   bfile   = false;
   bfolder = false;
  
   if(is.is_open()){
      is.close();
      slabel.path.setText("");
   }
 
   //////////////////////////////////////////////

   if(CFileChooser.getFile("OPEN FILE", GTK_WINDOW(CWindow.getWindow()), &in) != C_GTK_FILECHOOSER_READY)
      return(C_APP_ERROR);  
   
   is.open(in.c_str(), fstream::in | fstream::out);
   
   if(!is.is_open()){
      dmessage(ERROR_OPEN_FILE);
      return(C_APP_ERROR);
   }else
      dmessage(READY_OPEN_FILE);
   
   //////////////////////////////////////////////
   
   slabel.path.setText(in.c_str());
   
   bfile = true;

   return(C_APP_READY); 
}
//////////////////////////////////////////////////////////////////////////////////
// [ encrypt ]
//////////////////////////////////////////////////////////////////////////////////
void C_App::encrypt(){
   if(bfile)   encrypt_file();
   else
   if(bfolder) encrypt_folder();
   else        dmessage(ERROR_NO_FOLDER);
}
//////////////////////////////////////////////////////////////////////////////////
// [ decrypt ]
//////////////////////////////////////////////////////////////////////////////////
void C_App::decrypt(){
   if(bfile) decrypt_file();
   else      dmessage(ERROR_NO_FILE);
}
//////////////////////////////////////////////////////////////////////////////////
// [ encrypt_file ]
//////////////////////////////////////////////////////////////////////////////////
int C_App::encrypt_file(){

   if(!is.is_open()){
      dmessage(ERROR_NO_FILE);
      return(C_APP_ERROR);
   }
	 
   //////////////////////////////////////////////
   
   string pass1;
   
   if(getPassEncrypt(&pass1) != C_APP_READY) return(C_APP_ERROR);

   //////////////////////////////////////////////

   if(CFileChooser.setFile("SAVE FILE", GTK_WINDOW(CWindow.getWindow()), &on) != C_GTK_FILECHOOSER_READY)
      return(C_APP_ERROR);  
   
   os.open(on.c_str(), fstream::in | fstream::out | fstream::app);
   
   if(!os){
      dmessage(ERROR_OPEN_FILE);
      return(C_APP_ERROR);
   }else
      dmessage(READY_OPEN_FILE);
 
   //////////////////////////////////////////////

   CCrypt.setKey(&pass1);

   if(CThread._create(this, &C_App::encrypt_file_thread) != C_THREAD_READY){
      dmessage(ERROR_THREAD);
      return(C_APP_ERROR);
   }
   
   dmessage(WORKING);
   
   gtk_spinner_start(GTK_SPINNER(spinner));
  
   return(C_APP_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [ encrypt_file_thread ]
//////////////////////////////////////////////////////////////////////////////////
void C_App::encrypt_file_thread(){
  
   pthread_cleanup_push(thread_exit, (void*)&this->STResult);
   
   this->STResult.type   = C_APP_TYPE_ENC_FILE;
   this->STResult.pClass = this;
   
   if(CCrypt.encrypt_file(&is, &os, &in, C_CRYPT_TYPE_FILE) != C_CRYPT_READY){
      this->STResult.result = C_APP_ERROR;
   }else{
      this->STResult.result = C_APP_READY;
   }
   
   this->os.close();
   
   pthread_cleanup_pop(1);
   pthread_exit((void*)pthread_self());
}
//////////////////////////////////////////////////////////////////////////////////
// [ decrypt_file ]
//////////////////////////////////////////////////////////////////////////////////
int C_App::decrypt_file(){

   if(!is.is_open()){
      dmessage(ERROR_NO_FILE);
      return(C_APP_ERROR);
   }
   
   //////////////////////////////////////////////

   string pass1;

   if(getPassDecrypt(&pass1) != C_APP_READY) return(C_APP_ERROR);
   
   //////////////////////////////////////////////
   
   if(CFileChooser.getFolder("SELECT FOLDER", GTK_WINDOW(CWindow.getWindow()), &on) != C_GTK_FILECHOOSER_READY)
      return(C_APP_ERROR);
   
   //////////////////////////////////////////////

   CCrypt.setKey(&pass1);

   if(CThread._create(this, &C_App::decrypt_file_thread) != C_THREAD_READY){
      dmessage(ERROR_THREAD);
      return(C_APP_ERROR);
   }
   
   dmessage(WORKING);
   
   gtk_spinner_start(GTK_SPINNER(spinner));
   
   return(C_APP_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [ decrypt_file_thread ]
//////////////////////////////////////////////////////////////////////////////////
void C_App::decrypt_file_thread(){
  
   pthread_cleanup_push(thread_exit, (void*)&this->STResult);
   
   this->STResult.type   = C_APP_TYPE_DEC_FILE;
   this->STResult.pClass = this;
   
   if(CCrypt.decrypt_file(&is, &os, &in, &on) != C_CRYPT_READY){
      this->STResult.result = C_APP_ERROR;
   }else{
      this->STResult.result = C_APP_READY;
   }
   
   this->os.close();
   
   pthread_cleanup_pop(1);
   pthread_exit((void*)pthread_self());
}
//////////////////////////////////////////////////////////////////////////////////
// [ encrypt_folder ]
//////////////////////////////////////////////////////////////////////////////////
int C_App::encrypt_folder(){
  
   string pass1;
   
   if(getPassEncrypt(&pass1) != C_APP_READY) return(C_APP_ERROR);

   //////////////////////////////////////////////

   if(CFileChooser.setFile("SAVE FILE", GTK_WINDOW(CWindow.getWindow()), &on) != C_GTK_FILECHOOSER_READY)
      return(C_APP_ERROR);  
   
   os.open(on.c_str(), fstream::in | fstream::out | fstream::app);
   
   if(!os){
      dmessage(ERROR_OPEN_FILE);
      return(C_APP_ERROR);
   }
   
   //////////////////////////////////////////////

   CCrypt.setKey(&pass1);

   if(CThread._create(this, &C_App::encrypt_folder_thread) != C_THREAD_READY){
      dmessage(ERROR_THREAD);
      return(C_APP_ERROR);
   }

   dmessage(WORKING);
   
   gtk_spinner_start(GTK_SPINNER(spinner));
   
   return(C_APP_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [ encrypt_folder_thread ]
//////////////////////////////////////////////////////////////////////////////////
void C_App::encrypt_folder_thread(){
  
   pthread_cleanup_push(thread_exit, (void*)&this->STResult);
   
   this->STResult.type   = C_APP_TYPE_ENC_FOLDER;
   this->STResult.pClass = this;
   
   if(CCrypt.encrypt_folder(&os, &in) != C_CRYPT_READY){
      this->STResult.result = C_APP_ERROR;
   }else{
      this->STResult.result = C_APP_READY;
   }
   
   this->os.close();

   pthread_cleanup_pop(1);
   pthread_exit((void*)pthread_self());
}
//////////////////////////////////////////////////////////////////////////////
// [ decrypt_filev ]
//////////////////////////////////////////////////////////////////////////////
int C_App::decrypt_filev(){

   if(!is.is_open()){
      dmessage(ERROR_NO_FILE);
      return(C_APP_ERROR);
   }
   
   //////////////////////////////////////////////

   string pass1;

   if(getPassDecrypt(&pass1) != C_APP_READY) return(C_APP_ERROR);

   //////////////////////////////////////////////
   
   CCrypt.setKey(&pass1);

   int type = CCrypt.getType(&is);

   CCrypt.setKey(&pass1);

   if(type == C_CRYPT_TYPE_FILE){
      C_Array CABuffer;
   
      if(CCrypt.decrypt_filev(&is, &CABuffer) != C_CRYPT_READY){
         dmessage(ERROR_DECRYPT_FILE_V);
	 return(C_APP_ERROR);
      }

      string sbuffer = CABuffer._Get_pBuffer();
      
      if(CTextView.setText(&sbuffer) != C_GTK_TEXTVIEW_READY){
	 // NOT UTF-8, show Hex
         CTextView.clear();
         if(CThread._create(this, &C_App::morph_data_thread) != C_THREAD_READY){
            dmessage(ERROR_THREAD);
            return(C_APP_ERROR);
         }
         dmessage(WORKING);
         gtk_spinner_start(GTK_SPINNER(spinner));
      }
      
      //dmessage(READY_DECRYPT_FILE_V);
      CWindow.setSize(850, 800);
      
   }else
   if(type == C_CRYPT_TYPE_FOLDER){
      C_DArray CDA_Folder;
      S_C_DArray* pTemp = 0;

      CDA_VFiles._Clear();

      if(CCrypt.decrypt_folderv(&is, &CDA_Folder, &CDA_VFiles) != C_CRYPT_READY){
         dmessage(ERROR_DECRYPT_FILE_V);
         return(C_APP_ERROR);
      }

      //////////////////////////

      CTreeView.clear();
      CListView.clear();
      
      CDA_VFolder._Clear();

      S_Crypt_VFolder SCVFolder;
      
      ////////////////////////////////////////////////////////////////////////
      // Folder
      ////////////////////////////////////////////////////////////////////////

      GtkTreeModel* TreeModel = CTreeView.getModel();
      
      ////////////////////////////////////////////////////////////////
      // Add Container
      GtkTreeIter TreeIter;
      GtkTreePath* path;

      for(int nFolder = 0; nFolder < CDA_Folder._Get_nItems(); nFolder++){
         pTemp = CDA_Folder._Get_pItem(nFolder);

         char* pstrTemp = pTemp->_pData->_Get_pBuffer();
         int cSlash = 0;

         for(int n = 0; n < strlen(pTemp->_pData->_Get_pBuffer()); n++)
            if(pstrTemp[n] == '/') cSlash++;

         if(cSlash == 1){ // First Folder
            char* pStr = pTemp->_pData->_Get_pBuffer();
            pStr++;

	    CTreeView.addItem(pStr, 0, &SCVFolder.TreeIter);
	    
            memset(SCVFolder.Name, 0, sizeof(SCVFolder.Name));
            memset(SCVFolder.Path, 0, sizeof(SCVFolder.Path));
            memcpy(SCVFolder.Name, pStr, strlen(pStr));
            memcpy(SCVFolder.Path, pTemp->_pData->_Get_pBuffer(), strlen(pTemp->_pData->_Get_pBuffer()));

            C_Array* pItem = CDA_VFolder._Add_Item(1, sizeof(SCVFolder));
            memcpy(pItem->_Get_pBuffer(), (void*)&SCVFolder, sizeof(SCVFolder));
	    
	    path = CTreeView.getTreePathByIter(&SCVFolder.TreeIter);

         }else{ // Next Folder

            GtkTreeIter TIter;

	    //////////////////////////////7
	    // TEST
	    string tstring = pTemp->_pData->_Get_pBuffer();
            size_t found   = tstring.rfind("/");
            string str3    = tstring.substr(0, found); 
   
   
            if(getTreeByPath((char*)str3.c_str(), &TIter)){

               string tstring = pTemp->_pData->_Get_pBuffer();
               size_t found = tstring.rfind("/");
               found++;
               string str2 = tstring.substr(found, tstring.size() - found);

               CTreeView.addItem((char*)str2.c_str(), &TIter, &SCVFolder.TreeIter);

               memset(SCVFolder.Name, 0, sizeof(SCVFolder.Name));
               memset(SCVFolder.Path, 0, sizeof(SCVFolder.Path));
               memcpy(SCVFolder.Path, pTemp->_pData->_Get_pBuffer(), strlen(pTemp->_pData->_Get_pBuffer()));
               memcpy(SCVFolder.Name, str2.c_str(), strlen(str2.c_str()));

               C_Array* pItem = CDA_VFolder._Add_Item(1, sizeof(SCVFolder));
               memcpy(pItem->_Get_pBuffer(), (void*)&SCVFolder, sizeof(SCVFolder));
            }
         }
      }

      ////////////////////////////////////////////////////////////////////////////////
      // Select and Expand
      CTreeView.expandRow(path);
      CTreeView.setSelectByTreePath(path);
      CTreeView.setPathFree(path);
      
      ///////////////////////////////////////////////
      
      dmessage(READY_DECRYPT_FILE_V);
      CWindow.setSize(850, 150);

      CTreeView.show();
      CListView.show();
      
   }else{
      dmessage(ERROR_DECRYPT_FILE_V);
      return(C_APP_ERROR);
   }
   
   return(C_APP_READY);
}
//////////////////////////////////////////////////////////////////////////////
// [ decrypt_filevF ]
//////////////////////////////////////////////////////////////////////////////
int C_App::decrypt_filevF(unsigned long pData, int modus){
  
   if(!is.is_open()){
      dmessage(ERROR_NO_FILE);
      return(C_APP_ERROR);
   }
   
   //////////////////////////////////////////////
   
   if(CCrypt.decrypt_filev(&is, pData, &CAMorphBuffer) != C_CRYPT_READY){
      dmessage(ERROR_DECRYPT_FILE_V);
      return(C_APP_ERROR);
   }

   if(modus == C_APP_HEX){
      CTextView.clear();
      if(CThread._create(this, &C_App::morph_data_thread) != C_THREAD_READY){
         dmessage(ERROR_THREAD);
         return(C_APP_ERROR);
      }
      dmessage(WORKING);
      gtk_spinner_start(GTK_SPINNER(spinner));
   }else
   if(modus == C_APP_TXT){
      string sbuffer = CAMorphBuffer._Get_pBuffer();
      if(CTextView.setText(&sbuffer) != C_GTK_TEXTVIEW_READY){
	 // NOT UTF-8, show Hex
         CTextView.clear();
         if(CThread._create(this, &C_App::morph_data_thread) != C_THREAD_READY){
            dmessage(ERROR_THREAD);
            return(C_APP_ERROR);
         }
         dmessage(WORKING);
         gtk_spinner_start(GTK_SPINNER(spinner));
      }
   }else{
      dmessage(ERROR_DECRYPT_FILE_V);
      return(C_APP_ERROR);
   }

   CWindow.setSize(850, 800);
   
   return(C_APP_READY);
}
//////////////////////////////////////////////////////////////////////////////
// [ extract_file_decrypt ]
//////////////////////////////////////////////////////////////////////////////
int C_App::extract_file_decrypt(unsigned long pData, char* pDestination){

   if(!getFileByOffset(pData, &SCVFile)){
      dmessage(ERROR_FILE_NOT_FOUND);  
      return(C_APP_ERROR);
   }
   
   //////////////////////////////////////////////
   if(!pDestination){
      if(CFileChooser.getFolder("SELECT FOLDER", GTK_WINDOW(CWindow.getWindow()), &on) != C_GTK_FILECHOOSER_READY)
         return(C_APP_ERROR);
   }else{
      on = pDestination;
   }
   //////////////////////////////////////////////
   
   if(CThread._create(this, &C_App::extract_file_dec_thread) != C_THREAD_READY){
      dmessage(ERROR_THREAD);
      return(C_APP_ERROR);
   }

   dmessage(WORKING);
   
   gtk_spinner_start(GTK_SPINNER(spinner));
   
   return(C_APP_READY);
}
//////////////////////////////////////////////////////////////////////////////
// [ extract_file_dec_thread ]
//////////////////////////////////////////////////////////////////////////////
void C_App::extract_file_dec_thread(){
  
   pthread_cleanup_push(thread_exit, (void*)&this->STResult);
   
   this->STResult.type   = C_APP_TYPE_EX_DEC_FILE;
   this->STResult.pClass = this;
   
   if(CCrypt.extract_file_decrypt(&SCVFile, &is, &on) != C_CRYPT_READY){
      this->STResult.result = C_APP_ERROR;
   }else{
      this->STResult.result = C_APP_READY;
   }
   
   pthread_cleanup_pop(1);
   pthread_exit((void*)pthread_self());
}
//////////////////////////////////////////////////////////////////////////////
// [ extract_file_encrypt ]
//////////////////////////////////////////////////////////////////////////////
int C_App::extract_file_encrypt(unsigned long pData){

   if(!getFileByOffset(pData, &SCVFile)){
      dmessage(ERROR_FILE_NOT_FOUND);  
      return(C_APP_ERROR);
   }
   
   //////////////////////////////////////////////

   if(CFileChooser.setFile("SAVE FILE", GTK_WINDOW(CWindow.getWindow()), &on) != C_GTK_FILECHOOSER_READY)
      return(C_APP_ERROR);  
   
   os.open(on.c_str(), fstream::in | fstream::out | fstream::app);
   
   if(!os){
      dmessage(ERROR_OPEN_FILE);
      return(C_APP_ERROR);
   }
   
   //////////////////////////////////////////////
   
   if(CThread._create(this, &C_App::extract_file_enc_thread) != C_THREAD_READY){
      dmessage(ERROR_THREAD);
      return(C_APP_ERROR);
   }
   
   dmessage(WORKING);
   
   gtk_spinner_start(GTK_SPINNER(spinner));
   
   return(C_APP_READY); 
}
//////////////////////////////////////////////////////////////////////////////
// [ extract_file_enc_thread ]
//////////////////////////////////////////////////////////////////////////////
void C_App::extract_file_enc_thread(){
  
   pthread_cleanup_push(thread_exit, (void*)&this->STResult);
   
   this->STResult.type   = C_APP_TYPE_EX_ENC_FILE;
   this->STResult.pClass = this;
   
   if(CCrypt.extract_file_encrypt(&SCVFile, &is, &os) != C_CRYPT_READY){
      this->STResult.result = C_APP_ERROR;
   }else{
      this->STResult.result = C_APP_READY;
   }
   
   this->os.close();
   
   pthread_cleanup_pop(1);
   pthread_exit((void*)pthread_self());
}
//////////////////////////////////////////////////////////////////////////////
// [ extract_folder_encrypt ]
//////////////////////////////////////////////////////////////////////////////
int C_App::extract_folder_encrypt(string* pStrPath){

   if(!pStrPath) return(C_APP_ERROR);
   
   /////////////////////////////////////////
   
   if(pStrPath->size() == 1){
      dmessage("Just copy the File");
      return(C_APP_ERROR);
   }
   
   /////////////////////////////////////////
   
   CDA_Folder._Clear();
   CDA_Files._Clear();
   
   StrPath = *pStrPath;
   
   ///////////////////////////////////////////////////////////////////////////////////

   if(CFileChooser.setFile("SAVE FILE", GTK_WINDOW(CWindow.getWindow()), &on) != C_GTK_FILECHOOSER_READY)
      return(C_APP_ERROR);  
   
   os.open(on.c_str(), fstream::in | fstream::out | fstream::app);
   
   if(!os){
      dmessage(ERROR_OPEN_FILE);
      return(C_APP_ERROR);
   }
   
   ////////////////////////////////////////////////////////////////////////////////////
   
   getFilesByPath((char*)pStrPath->c_str(),  &CDA_Files,  C_APP_REKURSION);
   getFolderByPath((char*)pStrPath->c_str(), &CDA_Folder, C_APP_REKURSION);

   for(int n = 0; n < CDA_Folder._Get_nItems(); n++){
      S_C_DArray* pTemp = CDA_Folder._Get_pItem(n);
      S_Crypt_VFolder* pSCVFolder = (S_Crypt_VFolder*)pTemp->_pData->_Get_pBuffer();    
      getFilesByPath(pSCVFolder->Path, &CDA_Files, C_APP_REKURSION);
   }
   
   //////////////////////////////////////////////
   
   if(CThread._create(this, &C_App::extract_folder_enc_thread) != C_THREAD_READY){
      dmessage(ERROR_THREAD);
      return(C_APP_ERROR);
   }
 
   dmessage(WORKING);
 
   gtk_spinner_start(GTK_SPINNER(spinner));
 
   return(C_APP_READY); 
}
//////////////////////////////////////////////////////////////////////////////
// [ extract_folder_enc_thread ]
//////////////////////////////////////////////////////////////////////////////
void C_App::extract_folder_enc_thread(){
  
   pthread_cleanup_push(thread_exit, (void*)&this->STResult);
   
   this->STResult.type   = C_APP_TYPE_EX_ENC_FOLDER;
   this->STResult.pClass = this;
   
   if(CCrypt.extract_folder_encrypt(&CDA_Folder, &CDA_Files, &is, &os, &StrPath) != C_CRYPT_READY){
      this->STResult.result = C_APP_ERROR;
   }else{
      this->STResult.result = C_APP_READY;
   }
   
   this->os.close();
   
   pthread_cleanup_pop(1);
   pthread_exit((void*)pthread_self()); 
}
//////////////////////////////////////////////////////////////////////////////
// [ extract_folder_decrypt ]
//////////////////////////////////////////////////////////////////////////////
int C_App::extract_folder_decrypt(string* pStrPath, char* pDestination){
  
   if(!pStrPath) return(C_APP_ERROR);
   
   if(pStrPath->size() == 1){
      dmessage("Just decrypt the File");
      return(C_APP_ERROR);
   }
   
   /////////////////////////////////////////
   
   CDA_Folder._Clear();
   CDA_Files._Clear();
   
   StrPath = *pStrPath;
   
   //////////////////////////////////////////////
   if(!pDestination){
      if(CFileChooser.getFolder("SELECT FOLDER", GTK_WINDOW(CWindow.getWindow()), &on) != C_GTK_FILECHOOSER_READY)
         return(C_APP_ERROR);
   }else{
      on = pDestination;
   }
   //////////////////////////////////////////////
      
   getFilesByPath( (char*)pStrPath->c_str(), &CDA_Files,  C_APP_REKURSION);
   getFolderByPath((char*)pStrPath->c_str(), &CDA_Folder, C_APP_REKURSION);
   
   for(int n = 0; n < CDA_Folder._Get_nItems(); n++){
      S_C_DArray* pTemp = CDA_Folder._Get_pItem(n);
      S_Crypt_VFolder* pSCVFolder = (S_Crypt_VFolder*)pTemp->_pData->_Get_pBuffer();
      
      /////////////////////////////////////
      // Get Files
      getFilesByPath(pSCVFolder->Path, &CDA_Files, C_APP_REKURSION);
   }
   
   //////////////////////////////////////////////
   
   if(CThread._create(this, &C_App::extract_folder_dec_thread) != C_THREAD_READY){
      dmessage(ERROR_THREAD);
      return(C_APP_ERROR);
   }

   dmessage(WORKING);
   
   gtk_spinner_start(GTK_SPINNER(spinner));
   
   return(C_APP_READY); 
}
//////////////////////////////////////////////////////////////////////////////
// [ extract_folder_dec_thread ]
//////////////////////////////////////////////////////////////////////////////
void C_App::extract_folder_dec_thread(){
  
   pthread_cleanup_push(thread_exit, (void*)&this->STResult);
   
   this->STResult.type   = C_APP_TYPE_EX_DEC_FOLDER;
   this->STResult.pClass = this;
   
   if(CCrypt.extract_folder_decrypt(&CDA_Folder, &CDA_Files, &is, &on, &StrPath) != C_CRYPT_READY){
      this->STResult.result = C_APP_ERROR;
   }else{
      this->STResult.result = C_APP_READY;
   }
   
   pthread_cleanup_pop(1);
   pthread_exit((void*)pthread_self()); 
}
//////////////////////////////////////////////////////////////////////////////
// [ add_file ]
//////////////////////////////////////////////////////////////////////////////
int C_App::add_file(string* pStrPath){

   if(!pStrPath) return(C_APP_ERROR);
   
   //////////////////////////////////////////////
   
   StrPath = *pStrPath;

   //////////////////////////////////////////////
   

   if(CFileChooser.getFile("OPEN FILE", GTK_WINDOW(CWindow.getWindow()), &in_new) != C_GTK_FILECHOOSER_READY)
      return(C_APP_ERROR);  

   is_new.open(in_new.c_str(), fstream::in | fstream::out);
   
   if(!is_new.is_open()){
      dmessage(ERROR_OPEN_FILE);
      return(C_APP_ERROR);
   }
   
   ////////////////////////////
   // TEST
   bExpand = true;
   StrExpandPath = *pStrPath;

   //////////////////////////////////////////////
   
   if(CThread._create(this, &C_App::add_file_thread) != C_THREAD_READY){
      dmessage(ERROR_THREAD);
      return(C_APP_ERROR);
   }
   
   dmessage(WORKING);
   
   gtk_spinner_start(GTK_SPINNER(spinner));
   
   return(C_APP_READY);
}
//////////////////////////////////////////////////////////////////////////////
// [ add_file_thread ]
//////////////////////////////////////////////////////////////////////////////
void C_App::add_file_thread(){
  
   pthread_cleanup_push(thread_exit, (void*)&this->STResult);
   
   this->STResult.type   = C_APP_TYPE_ADD_FILE;
   this->STResult.pClass = this;
   
   if(CCrypt.add_file(&is_new, &is, &StrPath, &in_new) != C_CRYPT_READY){
      this->STResult.result = C_APP_ERROR;
   }else{
      this->STResult.result = C_APP_READY;
   }
   
   this->is_new.close();
   
   pthread_cleanup_pop(1);
   pthread_exit((void*)pthread_self()); 
}
//////////////////////////////////////////////////////////////////////////////
// [ add_folder ]
//////////////////////////////////////////////////////////////////////////////
int C_App::add_folder(string* pStrPath){
   
   if(!pStrPath) return(C_APP_ERROR);
   
   //////////////////////////////////////////////
   
   StrPath = *pStrPath;
   
   //////////////////////////////////////////////
   
   if(CFileChooser.getFolder("SELECT FOLDER", GTK_WINDOW(CWindow.getWindow()), &in_new) != C_GTK_FILECHOOSER_READY)
      return(C_APP_ERROR);
   
   ////////////////////////////

   size_t found = in_new.rfind("/");
   string str2  = in_new;
   string str4  = str2.substr(found, str2.size() - found);
   string str3  = *pStrPath;
   str3.append(str4);

   bExpand = true;
   StrExpandPath = str3;

   //////////////////////////////////////////////
   
   if(CThread._create(this, &C_App::add_folder_thread) != C_THREAD_READY){
      dmessage(ERROR_THREAD);
      return(C_APP_ERROR);
   }
   
   dmessage(WORKING);
   
   gtk_spinner_start(GTK_SPINNER(spinner));
   
   return(C_APP_READY);
}
//////////////////////////////////////////////////////////////////////////////
// [ add_folder_thread ]
//////////////////////////////////////////////////////////////////////////////
void C_App::add_folder_thread(){
  
   pthread_cleanup_push(thread_exit, (void*)&this->STResult);
   
   this->STResult.type   = C_APP_TYPE_ADD_FOLDER;
   this->STResult.pClass = this;
   
   if(CCrypt.add_folder(&is, &StrPath, &in_new) != C_CRYPT_READY){
      this->STResult.result = C_APP_ERROR;
   }else{
      this->STResult.result = C_APP_READY;
   }
   
   pthread_cleanup_pop(1);
   pthread_exit((void*)pthread_self()); 
}
//////////////////////////////////////////////////////////////////////////////
// [ del_file ]
//////////////////////////////////////////////////////////////////////////////
int C_App::del_file(unsigned long pData){

   if(!getFileByOffset(pData, &SCVFile)){
      dmessage(ERROR_FILE_NOT_FOUND);  
      return(C_APP_ERROR);
   }
   
   //////////////////////////////////////////////
   // TEST
   string tstring = SCVFile.Path;
   size_t found   = tstring.rfind("/");
    
   bExpand = true;
   StrExpandPath = tstring.substr(0, found); 
	    
   //////////////////////////////////////////////
   
   if(CThread._create(this, &C_App::del_file_thread) != C_THREAD_READY){
      dmessage(ERROR_THREAD);
      return(C_APP_ERROR);
   }
   
   dmessage(WORKING);
   
   gtk_spinner_start(GTK_SPINNER(spinner));
   
   return(C_APP_READY);
}
//////////////////////////////////////////////////////////////////////////////
// [ del_file_thread ]
//////////////////////////////////////////////////////////////////////////////
void C_App::del_file_thread(){
  
   pthread_cleanup_push(thread_exit, (void*)&this->STResult);
   
   this->STResult.type   = C_APP_TYPE_DEL_FILE;
   this->STResult.pClass = this;
   
   if(CCrypt.del_file(&SCVFile, &is, &in, 1) != C_CRYPT_READY){
      this->STResult.result = C_APP_ERROR;
   }else{
      this->STResult.result = C_APP_READY;
   }
   
   pthread_cleanup_pop(1);
   pthread_exit((void*)pthread_self()); 
}
//////////////////////////////////////////////////////////////////////////////
// [ del_folder ]
//////////////////////////////////////////////////////////////////////////////
int C_App::del_folder(string* pStrPath){

   if(!pStrPath) return(C_APP_ERROR);
   
   /////////////////////////////////////////
   
   char* pstrTemp = (char*)pStrPath->c_str();
   int cSlash = 0;

   for(int n = 0; n < pStrPath->size(); n++)
      if(pstrTemp[n] == '/') cSlash++;

   if(cSlash == 1){
      dmessage("Just delete the File");
      return(C_APP_ERROR);
   }

   /////////////////////////////////////////
   
   CDA_Folder._Clear();
   CDA_Files._Clear();
   
   StrPath = *pStrPath;
   
   //////////////////////////////////////////////
      
   getFilesByPath( (char*)pStrPath->c_str(), &CDA_Files,  C_APP_REKURSION);
   getFolderByPath((char*)pStrPath->c_str(), &CDA_Folder, C_APP_REKURSION);
   
   for(int n = 0; n < CDA_Folder._Get_nItems(); n++){
      S_C_DArray* pTemp = CDA_Folder._Get_pItem(n);
      S_Crypt_VFolder* pSCVFolder = (S_Crypt_VFolder*)pTemp->_pData->_Get_pBuffer();
      
      /////////////////////////////////////
      // Get Files
      getFilesByPath(pSCVFolder->Path, &CDA_Files, C_APP_REKURSION);
   }
   
   ////////////////////////////
   // TEST
   string tstring = *pStrPath;
   size_t found   = tstring.rfind("/");
    
   bExpand = true;
   StrExpandPath = tstring.substr(0, found); 

   //////////////////////////////////////////////
   
   if(CThread._create(this, &C_App::del_folder_thread) != C_THREAD_READY){
      dmessage(ERROR_THREAD);
      return(C_APP_ERROR);
   }
   
   dmessage(WORKING);
   
   gtk_spinner_start(GTK_SPINNER(spinner));
   
   return(C_APP_READY);
}
//////////////////////////////////////////////////////////////////////////////
// [ del_folder_thread ]
//////////////////////////////////////////////////////////////////////////////
void C_App::del_folder_thread(){
  
   pthread_cleanup_push(thread_exit, (void*)&this->STResult);
   
   this->STResult.type   = C_APP_TYPE_DEL_FOLDER;
   this->STResult.pClass = this;
   
   if(CCrypt.del_folder(&CDA_Folder, &CDA_Files, &StrPath, &is, &in) != C_CRYPT_READY){
      this->STResult.result = C_APP_ERROR;
   }else{
      this->STResult.result = C_APP_READY;
   }
   
   pthread_cleanup_pop(1);
   pthread_exit((void*)pthread_self()); 
}
//////////////////////////////////////////////////////////////////////////////
// [ new_file ]
//////////////////////////////////////////////////////////////////////////////
int C_App::new_file(string* pStrPath){
  
   if(!pStrPath) return(C_APP_ERROR);
   
   ///////////////////////////////////
   
   string NewName;
   
   if(getNewName("New File", &NewName) != C_APP_READY){
      dmessage(ERROR_NEW_FILE);
      return(C_APP_ERROR);
   }
   
   ///////////////////////////////////
   
   string Path = *pStrPath;
   Path.append("/");
   Path.append(NewName);
   
   if(CCrypt.new_file(&is, &Path) != C_CRYPT_READY){
      dmessage(ERROR_NEW_FILE);
      return(C_APP_ERROR);
   }
   
   ///////////////////////////////////
   
   decrypt_filev(); // Datei neu einlesen
   
   ///////////////////////////////////
   
   GtkTreeIter gTreeIter;
   
   if(getTreeByPath((char*)pStrPath->c_str(), &gTreeIter) == C_APP_READY){
      GtkTreePath* path = CTreeView.getTreePathByIter(&gTreeIter);
      CTreeView.expandRow(path);
      CTreeView.setSelectByTreePath(path);
      CTreeView.scrollToPath(path);
      CTreeView.setPathFree(path);
   }
   
   ///////////////////////////////////
   
   dmessage(READY_NEW_FILE);
   
   return(C_APP_READY);
}
//////////////////////////////////////////////////////////////////////////////
// [ new_folder ]
//////////////////////////////////////////////////////////////////////////////
int C_App::new_folder(string* pStrPath){
  
   if(!pStrPath) return(C_APP_ERROR);
   
   ///////////////////////////////////
   
   string NewName;
   
   if(getNewName("New Folder", &NewName) != C_APP_READY){
      dmessage(ERROR_NEW_FOLDER);
      return(C_APP_ERROR);
   }
   
   ///////////////////////////////////

   string Path = *pStrPath;
   Path.append("/");
   Path.append(NewName);
      
   if(CCrypt.new_folder(&is, &Path) != C_CRYPT_READY){
      dmessage(ERROR_NEW_FOLDER);
      return(C_APP_ERROR);
   }
   
   ///////////////////////////////////
   
   decrypt_filev(); // Datei neu einlesen
   
   ///////////////////////////////////
   
   GtkTreeIter gTreeIter;
   
   if(getTreeByPath((char*)Path.c_str(), &gTreeIter) == C_APP_READY){
      GtkTreePath* path = CTreeView.getTreePathByIter(&gTreeIter);
      CTreeView.expandRow(path);
      CTreeView.setSelectByTreePath(path);
      CTreeView.scrollToPath(path);
      CTreeView.setPathFree(path);
   }
   
   ///////////////////////////////////

   dmessage(READY_NEW_FOLDER);
   
   return(C_APP_READY);
}
//////////////////////////////////////////////////////////////////////////////
// [ edit_file ]
//////////////////////////////////////////////////////////////////////////////
int C_App::edit_file(unsigned long pData){
   C_Array CABuffer;

   if(CCrypt.decrypt_filev(&is, pData, &CABuffer) != C_CRYPT_READY){
      dmessage(ERROR_EDIT_FILE);
      return(C_APP_ERROR);
  }

   if(createEditWindow(pData, &CABuffer) != C_CRYPT_READY){
      dmessage(ERROR_EDIT_FILE);
      return(C_APP_ERROR);
  }

   dmessage(READY_EDIT_FILE);
   
   return(C_APP_READY);
}
//////////////////////////////////////////////////////////////////////////////
// [ save_file ]
//////////////////////////////////////////////////////////////////////////////
int C_App::save_file(unsigned long pData, string* pStrData){
  
   if(!pStrData) return(C_APP_ERROR);
  
   if(CCrypt.edit_file(&is, pData, pStrData, &in) != C_CRYPT_READY){
      dmessage(ERROR_SAVE_FILE); 
      return(C_APP_ERROR);
   }
   
   decrypt_filev(); // Datei neu einlesen
   
   dmessage(READY_SAVE_FILE);
   
   return(C_APP_READY);
}
//////////////////////////////////////////////////////////////////////////////
// [ dmessage ]
//////////////////////////////////////////////////////////////////////////////
void C_App::dmessage(const char* pMessage){
   if(pMessage){
      cout << pMessage << endl;
      slabel.message.setText(pMessage);
   }
}
//////////////////////////////////////////////////////////////////////////////
// [morph_data]  Speed ??? 
//////////////////////////////////////////////////////////////////////////////
int C_App::morph_data(char* pData, unsigned long cData, GtkTextBuffer* pTBuffer){
   unsigned long vaddress = 0x00, paddress = 0x00, i, j;
   char sLine[250] = {0}, ch;
   char BYTESPERLINE = 16;
   vaddress = (DWORD) pData;

   GtkTextIter iter;
   
   if(!pData || !pTBuffer) return(C_APP_ERROR);

   ///////////////////////////////////////////////////////
 
   gtk_text_buffer_get_iter_at_offset(pTBuffer, &iter, 0);
   
   ///////////////////////////////////////////////////////
   
   for(i = 0; i < cData; i += BYTESPERLINE){
      
      ///////////////////////////////////////////////////////////
      // Offset
      sprintf(sLine, "0x%08X || ", paddress + i);
      gtk_text_buffer_insert_with_tags_by_name(pTBuffer, &iter, sLine, -1, "green_fg", "black_bg", NULL);

      ///////////////////////////////////////////////////////////
      // Hex
      for(j = 0; j < BYTESPERLINE; j++){

         if(i + j == cData){

            for(int x = 0; x < (BYTESPERLINE - j - 1); x++){
               sprintf(sLine, "   ");
	       gtk_text_buffer_insert_with_tags_by_name(pTBuffer, &iter, sLine, -1, "black_fg", "white_bg", NULL);
            }
            
            sprintf(sLine, "  ");
	    gtk_text_buffer_insert_with_tags_by_name(pTBuffer, &iter, sLine, -1, "black_fg", "white_bg", NULL);
            
            if((BYTESPERLINE - j) > 7){
               sprintf(sLine, "  ");
	       gtk_text_buffer_insert_with_tags_by_name(pTBuffer, &iter, sLine, -1, "black_fg", "white_bg", NULL);
            }
            
            break;
         }
         
         if(j == BYTESPERLINE / 2){
            sprintf(sLine, "- ");
	    gtk_text_buffer_insert_with_tags_by_name(pTBuffer, &iter, sLine, -1, "black_fg", "white_bg", NULL);
         }
         
         if(j == BYTESPERLINE - 1) sprintf(sLine, "%02X",  *(unsigned char*)(vaddress + i + j));
	 else                      sprintf(sLine, "%02X ", *(unsigned char*)(vaddress + i + j));

	 gtk_text_buffer_insert_with_tags_by_name(pTBuffer, &iter, sLine, -1, "black_fg", "white_bg", NULL);
      }
      
      ///////////////////////////////////////////////////////////
      // Asccii
      sprintf(sLine, " || ");
      gtk_text_buffer_insert_with_tags_by_name(pTBuffer, &iter, sLine, -1, "green_fg", "black_bg", NULL);
      
      for(j = 0; j < BYTESPERLINE; j++){
         if(i + j == cData) break;
         ch = *(unsigned char*)(vaddress + i + j);
	 
	 if(isalnum(ch) || ch == ' ') sprintf(sLine, "%c", ch);
         else                         sprintf(sLine, ".");

         gtk_text_buffer_insert_with_tags_by_name(pTBuffer, &iter, sLine, -1, "green_fg", "black_bg", NULL);
      }

      gtk_text_buffer_insert(pTBuffer, &iter, "\n", -1);
   }

   return(C_APP_READY);
}
//////////////////////////////////////////////////////////////////////////////
// [morph_data_thread]  
//////////////////////////////////////////////////////////////////////////////
void C_App::morph_data_thread(){
  
   pthread_cleanup_push(thread_exit, (void*)&this->STResult);
   
   this->STResult.type   = C_APP_TYPE_MORPH;
   this->STResult.pClass = this;
   
   pTextBuffer = gtk_text_buffer_new(0); // Check for Memory Leak
   
   gtk_text_buffer_create_tag(pTextBuffer, "black_bg", "background",  "black", NULL);
   gtk_text_buffer_create_tag(pTextBuffer, "green_fg", "foreground",  "rgba(  0, 110,   0, 1.0)", NULL);
   gtk_text_buffer_create_tag(pTextBuffer, "white_bg", "background",  "rgba(210, 210, 210, 1.0)", NULL);
   gtk_text_buffer_create_tag(pTextBuffer, "black_fg", "foreground",  "black", NULL);
   
   if(morph_data(CAMorphBuffer._Get_pBuffer(), CAMorphBuffer._Get_cBuffer(), pTextBuffer) != C_APP_READY){
      this->STResult.result = C_APP_ERROR;
   }else{
      this->STResult.result = C_APP_READY;
   }
   
   pthread_cleanup_pop(1);
   pthread_exit((void*)pthread_self());
}
//////////////////////////////////////////////////////////////////////////////
// [getPassEncrypt]  
//////////////////////////////////////////////////////////////////////////////
int C_App::getPassEncrypt(string* pKey){
   if(!pKey) return(C_APP_ERROR);
   
   string Key2;
   
   sedit.pass1.getText(pKey);
   sedit.pass2.getText(&Key2);
   
   if(pKey->length() < 1){
      dmessage("Error: pass1 < 1");
      return(C_APP_ERROR);
   }
   
   if(*pKey != Key2){
      dmessage("Error: pass1 != pass2");
      return(C_APP_ERROR);
   }

   if(pKey->length() > 16){
      dmessage("Warning: pass1 > 16");
      pKey->erase(16, pKey->length() - 16);
   }
   
   if(pKey->length() < 16){
      dmessage("Warning: pass1 < 16");
      while(pKey->length() < 16)
         pKey->insert(pKey->length(), *pKey, 0, 16 - pKey->length());
   }
   
   return(C_APP_READY);
}
//////////////////////////////////////////////////////////////////////////////
// [getPassDecrypt]  
//////////////////////////////////////////////////////////////////////////////
int C_App::getPassDecrypt(string* pKey){
   if(!pKey) return(C_APP_ERROR);
   
   sedit.pass1.getText(pKey);
   
   if(pKey->length() < 1){
      dmessage("Error: pass1 < 1");
      return(C_APP_ERROR);
   }
   
   if(pKey->length() > 16){
      dmessage("Warning: pass1 > 16");
      pKey->erase(16, pKey->length() - 16);
   }
   
   if(pKey->length() < 16){
      dmessage("Warning: pass1 < 16");
      while(pKey->length() < 16)
         pKey->insert(pKey->length(), *pKey, 0, 16 - pKey->length());
   }
   
   return(C_APP_READY);
}
//////////////////////////////////////////////////////////////////////////////
// [ getTreeByPath ]
//////////////////////////////////////////////////////////////////////////////
int C_App::getTreeByPath(char* psName, GtkTreeIter* pTreeIter){
   S_C_DArray*      pTemp      = 0;
   S_Crypt_VFolder* pSCVFolder = 0;
   
   if(!psName || !pTreeIter) return(C_APP_ERROR);
   
   /////////////////////////////
   
   string tstring = psName;
   
   for(int n = 0; n < CDA_VFolder._Get_nItems(); n++){
      pTemp      = CDA_VFolder._Get_pItem(n);
      pSCVFolder = (S_Crypt_VFolder*)pTemp->_pData->_Get_pBuffer();
      
      string tstring2 = pSCVFolder->Path;
      
      if(tstring2 == tstring){
	 *pTreeIter = pSCVFolder->TreeIter;
         return(C_APP_READY);
      }
   }
   return(C_APP_ERROR); // Not found
}
//////////////////////////////////////////////////////////////////////////////
// [ getPathByTree ]
//////////////////////////////////////////////////////////////////////////////
int C_App::getPathByTree(GtkTreeIter* pTreeIter, string* pPath){
   S_C_DArray*      pTemp      = 0;
   S_Crypt_VFolder* pSCVFolder = 0;
   
   if(!pPath || !pTreeIter) return(C_APP_ERROR);
      
   for(int n = 0; n < CDA_VFolder._Get_nItems(); n++){
      pTemp      = CDA_VFolder._Get_pItem(n);
      pSCVFolder = (S_Crypt_VFolder*)pTemp->_pData->_Get_pBuffer();
      
      if(pSCVFolder->TreeIter.user_data == pTreeIter->user_data){
	 *pPath = pSCVFolder->Path;
         return(C_APP_READY);
      }
   }
   return(C_APP_ERROR); // Not found
}
//////////////////////////////////////////////////////////////////////////////
// [ getNameByTree ]
//////////////////////////////////////////////////////////////////////////////
int C_App::getNameByTree(GtkTreeIter* pTreeIter, string* pName){
   S_C_DArray*      pTemp      = 0;
   S_Crypt_VFolder* pSCVFolder = 0;
   
   if(!pName || !pTreeIter) return(C_APP_ERROR);
   
   for(int n = 0; n < CDA_VFolder._Get_nItems(); n++){
      pTemp      = CDA_VFolder._Get_pItem(n);
      pSCVFolder = (S_Crypt_VFolder*)pTemp->_pData->_Get_pBuffer();
      
      if(pSCVFolder->TreeIter.user_data == pTreeIter->user_data){
	 *pName = pSCVFolder->Name;
         return(C_APP_READY);
      }
   }
   return(C_APP_ERROR); // Not found
}
//////////////////////////////////////////////////////////////////////////////
// [ getFolderByPath ]
//////////////////////////////////////////////////////////////////////////////
int C_App::getFolderByPath(char* psPath, C_DArray* pDA_Folder, int rekursion){
   S_C_DArray*      pTemp      = 0;
   S_Crypt_VFolder* pSCVFolder = 0;
   
   string str2, str3;
   
   if(!psPath || !pDA_Folder) return(C_APP_ERROR);
   
   str2 = psPath;

   if(!rekursion) pDA_Folder->_Clear();

   for(int n = 0; n < CDA_VFolder._Get_nItems(); n++){
      pTemp      = CDA_VFolder._Get_pItem(n);
      pSCVFolder = (S_Crypt_VFolder*)pTemp->_pData->_Get_pBuffer();
      
      string tstring2 = pSCVFolder->Path;
      string tstring3 = pSCVFolder->Name;

      int size = tstring2.size() - tstring3.size();
      
      if(size > 1) size--;
      
      str3 = tstring2.substr(0, size);
      
      if(str3 == str2){
         C_Array* pCA_Folder = pDA_Folder->_Add_Item(1, sizeof(S_Crypt_VFolder));
         memcpy(pCA_Folder->_Get_pBuffer(), pSCVFolder, sizeof(S_Crypt_VFolder));
	 if(rekursion) getFolderByPath(pSCVFolder->Path, pDA_Folder, C_APP_REKURSION); // Rekursion
      }
   }
   
   return(C_APP_READY);
}
//////////////////////////////////////////////////////////////////////////////
// [ getFilesByPath ]
//////////////////////////////////////////////////////////////////////////////
int C_App::getFilesByPath(char* psPath, C_DArray* pDA_Files, int rekursion){
   S_C_DArray*    pTemp    = 0;
   S_Crypt_VFile* pSCVFile = 0;
   
   string str2, str3;
   
   if(!psPath || !pDA_Files) return(C_APP_ERROR);
   
   str2 = psPath;
   
   if(!rekursion) pDA_Files->_Clear();
   
   for(int n = 0; n < CDA_VFiles._Get_nItems(); n++){
      pTemp    = CDA_VFiles._Get_pItem(n);
      pSCVFile = (S_Crypt_VFile*)pTemp->_pData->_Get_pBuffer();
  
      string tstring2 = pSCVFile->Path;
      string tstring3 = pSCVFile->Name;

      int size = tstring2.size() - tstring3.size();
      
      if(size > 1) size--;
      
      str3 = tstring2.substr(0, size);

      if(str3 == str2){
         C_Array* pCA_File = pDA_Files->_Add_Item(1, sizeof(S_Crypt_VFile));
         memcpy(pCA_File->_Get_pBuffer(), pSCVFile, sizeof(S_Crypt_VFile));
      }
   }
   
   return(C_APP_READY); 
}
//////////////////////////////////////////////////////////////////////////////
// [ getFileByOffset ]
//////////////////////////////////////////////////////////////////////////////
int C_App::getFileByOffset(unsigned long pData, S_Crypt_VFile* pSCVFile){
  
   if(!pSCVFile) return(C_APP_ERROR);
     
   for(int n = 0; n < CDA_VFiles._Get_nItems(); n++){
      S_C_DArray*    pTemp = CDA_VFiles._Get_pItem(n);
      S_Crypt_VFile* pSCVF = (S_Crypt_VFile*)pTemp->_pData->_Get_pBuffer();
     
      if(pSCVF->pData == pData){
	 memcpy(pSCVFile, pSCVF, sizeof(S_Crypt_VFile));
         return(C_APP_READY);
      }
   }
   
   return(C_APP_ERROR);
}
//////////////////////////////////////////////////////////////////////////////
// [ getNewName ]
//////////////////////////////////////////////////////////////////////////////
int C_App::getNewName(const char* pCaption, string* pNewName){
  
   if(!pCaption || !pNewName) return(C_APP_ERROR);
   
   GtkWidget* entry  = gtk_entry_new();
   GtkWidget* dialog = gtk_dialog_new();
  
   gtk_window_set_title(GTK_WINDOW(dialog), pCaption);
   gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(CWindow.getWindow()));
   gtk_dialog_add_action_widget(GTK_DIALOG(dialog), entry, 0xFF);
   gtk_widget_show(entry);
   
   if(gtk_dialog_run(GTK_DIALOG(dialog)) == 0xFF) 
      *pNewName = gtk_entry_get_text(GTK_ENTRY(entry));

   gtk_widget_destroy(dialog);
   
   if(pNewName->empty()) return(C_APP_ERROR);

   return(C_APP_READY);
}
//////////////////////////////////////////////////////////////////////////////
// [ createEditWindow ]
//////////////////////////////////////////////////////////////////////////////
int C_App::createEditWindow(unsigned long pData, C_Array* pCABuffer){
  
   C_GTK_Window CTWindow;
   C_GTK_TextView CEditTextView;

   if(!pCABuffer) return(C_APP_ERROR);
   
   string sbuffer = pCABuffer->_Get_pBuffer();

   GtkWidget* window = CTWindow.create();

   CTWindow.setSize(850, 800);
   CTWindow.setCaption("Edit");

   GtkWidget* vbox = gtk_vbox_new(FALSE, 0);
   gtk_container_add(GTK_CONTAINER(window), vbox);

   GtkWidget* text1  = CEditTextView.create();
   CEditTextView.setFont("Courier New 12");

   GtkWidget* scrolled_window1 = gtk_scrolled_window_new(NULL, NULL);
   gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window1), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

   gtk_container_add(GTK_CONTAINER(scrolled_window1), text1);

   gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(CWindow.getWindow()));

   GtkWidget* menubar  = gtk_menu_bar_new();
   GtkWidget* fileMenu = gtk_menu_new();
   GtkWidget* fileMi   = gtk_menu_item_new_with_label("File");
   GtkWidget* mSave    = gtk_menu_item_new_with_label("save");
   GtkWidget* quitMi   = gtk_menu_item_new_with_label("Quit");
   GtkWidget* sep      = gtk_separator_menu_item_new(); 

   gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileMi), fileMenu);
   gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), mSave);
   gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), sep);
   gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), quitMi);
   gtk_menu_shell_append(GTK_MENU_SHELL(menubar), fileMi);

   gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0); 
   gtk_box_pack_end(GTK_BOX(vbox), scrolled_window1, TRUE, TRUE, 0);

   if(CEditTextView.setText(&sbuffer) != C_GTK_TEXTVIEW_READY) return(C_APP_ERROR);

   gtk_widget_show_all(window);
   
   SEditFile.pWindow     = window;
   SEditFile.pTextBuffer = CEditTextView.getTextBuffer();
   SEditFile.pData       = pData;
   SEditFile.pQuit       = quitMi;
   SEditFile.pSave       = mSave;

   return(C_APP_READY);
}
//////////////////////////////////////////////////////////////////////////////
// [ add_drop ]
//////////////////////////////////////////////////////////////////////////////
int C_App::add_drop(){
  
   GtkTreeIter iter;
   GtkTreeModel *model;
   string TString;
   
   GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(CTreeView.getTree()));

   if(gtk_tree_selection_get_selected(selection, &model, &iter))
      if(!getPathByTree(&iter, &TString)) return(C_APP_ERROR);

   //////////////////////////////////////////////
   
   StrPath = TString;
   
   bExpand = true;
   StrExpandPath = StrPath;
   
   /////////////////////////////
   
   if(CThread._create(this, &C_App::add_drop_thread) != C_THREAD_READY){
      dmessage(ERROR_THREAD);
      return(C_APP_ERROR);
   }
   
   dmessage(WORKING);
   
   gtk_spinner_start(GTK_SPINNER(spinner));
   
   return(C_APP_READY); 
}
//////////////////////////////////////////////////////////////////////////////
// [ add_drop_thread ]
//////////////////////////////////////////////////////////////////////////////
void C_App::add_drop_thread(){
  
   pthread_cleanup_push(thread_exit, (void*)&this->STResult);
   
   this->STResult.type   = C_APP_TYPE_ADD_DROP;
   this->STResult.pClass = this;
   this->STResult.result = C_APP_READY;
   
   ////////////////////////////////////
   // Add Files
   
   for(int n = 0; n < CDA_AddFiles._Get_nItems(); n++){
      S_C_DArray* pTemp = CDA_AddFiles._Get_pItem(n);

      in_new = pTemp->_pData->_Get_pBuffer();
      
      is_new.open(in_new.c_str(), fstream::in | fstream::out);
   
      if(!is_new.is_open()){
         this->STResult.result = C_APP_ERROR;
         break;
      }
   
      if(CCrypt.add_file(&is_new, &is, &StrPath, &in_new) != C_CRYPT_READY){
         this->STResult.result = C_APP_ERROR;
	 break;
      }
      
      is_new.close();
   }
   
   ////////////////////////////////////
   // Add Folder
   
   for(int n = 0; n < CDA_AddFolder._Get_nItems(); n++){
      S_C_DArray* pTemp = CDA_AddFolder._Get_pItem(n);

      in_new = pTemp->_pData->_Get_pBuffer();
      
      if(CCrypt.add_folder(&is, &StrPath, &in_new) != C_CRYPT_READY){
         this->STResult.result = C_APP_ERROR;
	 break;
      }
   }
 
   /////////////////////////////

   pthread_cleanup_pop(1);
   pthread_exit((void*)pthread_self()); 
}

