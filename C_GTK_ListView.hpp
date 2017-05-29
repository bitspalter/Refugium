//////////////////////////////////////////////////////////////////////////////////
// [ GTK_ListView_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
// LISTVIEW
//
// [::Last modi: 26.10.16 L.ey (µ~)::]
//
//
#ifndef _C_GTK_LISTVIEW_H_
 #define _C_GTK_LISTVIEW_H_
 
 #include <gtk/gtk.h>
 
//////////////////////////////////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////////////////////////////////
 
 const int C_GTK_LISTVIEW_ERROR = 0x00;
 const int C_GTK_LISTVIEW_READY = 0x01;
 
//////////////////////////////////////////////////////////////////////////////////
// STRUCT
//////////////////////////////////////////////////////////////////////////////////
 
 class C_GTK_ListView {
   
     public:

        C_GTK_ListView();
       ~C_GTK_ListView();
       
        GtkWidget* create();
	
	int addColumn(int pos, char* pText);
	
	GtkListStore* createLStore(int nColumns, GType* pTypes);
	
	GtkTreeIter addItem(char* pText, int column, GtkListStore* pLStore, GtkTreeIter* pTreeIter);
	GtkTreeIter addItem(char* pText, int column, GtkTreeIter* pTreeIter);
	
	int addLStore(GtkListStore* pLStore);

	int setBackground(const GdkRGBA *color);
	int setColor(const GdkRGBA *color);
	int setFont(const char* pFont);
	
	int clear();
	int hide();
	int show();
	
	GtkWidget* getList(){return(this->pListView);};
	
	//int getSelect();
	
	GtkTreeSelection*  getListSelect();
	GtkTreeSelection*  setListSelect(GtkSelectionMode type);

     private:
       
        GtkWidget*    pListView;

	int status;
 };
 
#endif // _C_GTK_LISTVIEW_H_