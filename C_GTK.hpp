//////////////////////////////////////////////////////////////////////////////////
// [ GTK_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
// GTK
//
// [::Last modi: 26.10.16 L.ey (µ~)::]
//
//
#ifndef _C_GTK_H_
 #define _C_GTK_H_
 
   #include <gtk/gtk.h>
 
   #include "C_GTK_Window.hpp"
   #include "C_GTK_Button.hpp"
   #include "C_GTK_Edit.hpp"
   #include "C_GTK_Label.hpp"
   #include "C_GTK_TextView.hpp"
   #include "C_GTK_TreeView.hpp"
   #include "C_GTK_ListView.hpp"
   #include "C_GTK_FileChooser.hpp"
   
//////////////////////////////////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////////////////////////////////
 
   const int C_GTK_ERROR = 0x00;
   const int C_GTK_READY = 0x01;
 
//////////////////////////////////////////////////////////////////////////////////
// STRUCT
//////////////////////////////////////////////////////////////////////////////////
 
   struct SButton {
      C_GTK_Button openfile;
      C_GTK_Button openfolder;
      C_GTK_Button encrypt;
      C_GTK_Button decrypt;
      C_GTK_Button decryptv;
   };
 
 //////////////////////////////
 
   struct SEdit {
      C_GTK_Edit pass1;
      C_GTK_Edit pass2;
   };
   
 //////////////////////////////
 
   struct SLabel {
      C_GTK_Label message;
      C_GTK_Label path;
   };
   
//////////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////////
   
 class C_GTK {
   
     public:

        C_GTK();
       ~C_GTK();
       
        int init(int *argc, char ***argv);
        int create();
	int main();
	
	int resize(int x, int y);
	
        C_GTK_Window CWindow;

	SButton sbutton;
	SEdit   sedit;
        SLabel  slabel;
	
	GtkWidget* layout;
	GtkWidget* darea;
	GtkWidget* scrolled_window1;
	GtkWidget* scrolled_window2;
	GtkWidget* scrolled_window3;
	
	GtkWidget* spinner;
	
	C_GTK_TextView CTextView;
	C_GTK_TreeView CTreeView;
	C_GTK_ListView CListView;
	
	C_GTK_FileChooser CFileChooser;
	
     private:
       
        int status;
 };

#endif // _C_GTK_H_