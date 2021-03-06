//////////////////////////////////////////////////////////////////////////////////
// [ GTK_Button_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
// BUTTON
//
// [::Last modi: 26.10.16 L.ey (µ~)::]
//
//
#ifndef _C_GTK_BUTTON_H_
 #define _C_GTK_BUTTON_H_
 
 #include <gtk/gtk.h>
 
//////////////////////////////////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////////////////////////////////
 
 const int C_GTK_BUTTON_ERROR = 0x00;
 const int C_GTK_BUTTON_READY = 0x01;
 
//////////////////////////////////////////////////////////////////////////////////
// STRUCT
//////////////////////////////////////////////////////////////////////////////////
 
 class C_GTK_Button {
   
     public:

        C_GTK_Button();
       ~C_GTK_Button();
       
        GtkWidget* create(const gchar *title);

	int setCaption(const gchar *title);
	int setSize(int x, int y);
	int setBackground(const GdkRGBA *color);
	int setColor(const GdkRGBA *color);
	
	GtkWidget* getButton();
	
	int hide();
	int show();

     private:
       
        GtkWidget* pbutton;
	 
	int status;
 };
 
 
#endif // _C_GTK_BUTTON_H_