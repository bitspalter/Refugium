//////////////////////////////////////////////////////////////////////////////////
// [ GTK_TextView_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
// TEXTVIEW
//
// [::Last modi: 24.11.16 L.ey (µ~)::]
//
//
#ifndef _C_GTK_TEXTVIEW_H_
 #define _C_GTK_TEXTVIEW_H_
 
 #include <string>
 using namespace std;

 #include <gtk/gtk.h>
 
//////////////////////////////////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////////////////////////////////
 
 const int C_GTK_TEXTVIEW_ERROR = 0x00;
 const int C_GTK_TEXTVIEW_READY = 0x01;
 
//////////////////////////////////////////////////////////////////////////////////
// STRUCT
//////////////////////////////////////////////////////////////////////////////////
 
 class C_GTK_TextView {
   
     public:

        C_GTK_TextView();
       ~C_GTK_TextView();
       
        GtkWidget* create();

	GtkWidget* getTextView();
	
	GtkTextBuffer* getTextBuffer();
	int setTextBuffer(GtkTextBuffer* pTextBuffer);
	
	
	int getText(string* pOut);
	int setText(string* pIn);
	
	int clear();
	
	int setSize(int x, int y);
	int setBackground(const GdkRGBA *color);
	int setColor(const GdkRGBA *color);
	int setFont(const char* pFont);
	
	int hide();
	int show();
	
     private:
       
        GtkWidget*      ptextview;
	GtkTextBuffer*  ptextbuffer;
	
	int status;
 };

#endif // _C_GTK_TEXTVIEW_H_