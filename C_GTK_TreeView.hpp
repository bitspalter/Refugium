//////////////////////////////////////////////////////////////////////////////////
// [ GTK_TreeView_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
// TREEVIEW
//
// [::Last modi: 22.11.16 L.ey (µ~)::]
//
//
#ifndef _C_GTK_TREEVIEW_H_
 #define _C_GTK_TREEVIEW_H_
 
 #include <gtk/gtk.h>
 
//////////////////////////////////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////////////////////////////////
 
 const int C_GTK_TREEVIEW_ERROR = 0x00;
 const int C_GTK_TREEVIEW_READY = 0x01;
 
//////////////////////////////////////////////////////////////////////////////////
// STRUCT
//////////////////////////////////////////////////////////////////////////////////
 
 class C_GTK_TreeView {
   
     public:

        C_GTK_TreeView();
       ~C_GTK_TreeView();
       
        GtkWidget* create();
	
	int addColumn(int pos, char* pText);
	
	GtkTreeStore* createTStore();
	
	int addItem(char* pText, GtkTreeStore* pTStore, GtkTreeIter* pParentTreeIter, GtkTreeIter* pNewTreeIter);
	int addItem(char* pText, GtkTreeIter* pParentTreeIter, GtkTreeIter* pNewTreeIter);
	
	int addTStore(GtkTreeStore* pTStore);
	
	int getSelect();
	
	int setBackground(const GdkRGBA *color);
	int setColor(const GdkRGBA *color);
	int setFont(const char* pFont);
	
	int clear();
	int hide();
	int show();
	
	GtkWidget* getTree(){return(this->pTreeView);};

	GtkTreeSelection*  getTreeSelect();
	GtkTreeSelection*  setTreeSelect(GtkSelectionMode type);
	
	GtkTreeModel* getModel();
	
	GtkTreePath* getTreePathByIter(GtkTreeIter* pTreeIter);

	int expandAll();
	int collapseAll();
	
	int expandRow(GtkTreePath* pTreePath);

	int setSelectByTreePath(GtkTreePath* pTreePath);
	
	int setPathDown(GtkTreePath* pTreePath);
	int setPathUp(GtkTreePath* pTreePath);
	
	int setPathNext(GtkTreePath* pTreePath);
	int setPathPrev(GtkTreePath* pTreePath);
	
	int scrollToPath(GtkTreePath* pTreePath);
	
	int setPathFree(GtkTreePath* pTreePath);
	
     private:
       
        GtkWidget*    pTreeView;

	int status;
 };
 
 
#endif // _C_GTK_TREEVIEW_H_