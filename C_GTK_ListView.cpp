//////////////////////////////////////////////////////////////////////////////////
// [ GTK_ListView_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_GTK_ListView.hpp"

//////////////////////////////////////////////////////////////////////////////////
// [Konstructor]  
//////////////////////////////////////////////////////////////////////////////////
C_GTK_ListView::C_GTK_ListView(){
   this->pListView = 0;
   this->status    = C_GTK_LISTVIEW_ERROR;
}
//////////////////////////////////////////////////////////////////////////////////
// [Destructor]
//////////////////////////////////////////////////////////////////////////////////
C_GTK_ListView::~C_GTK_ListView(){

}
//////////////////////////////////////////////////////////////////////////////////
// [create]
//////////////////////////////////////////////////////////////////////////////////
GtkWidget* C_GTK_ListView::create(){

   if(this->status == C_GTK_LISTVIEW_READY) return(this->pListView);

   this->pListView = gtk_tree_view_new();

   if(!this->pListView) return(C_GTK_LISTVIEW_ERROR);
   
   this->status = C_GTK_LISTVIEW_READY;
   
   return(this->pListView);
}
//////////////////////////////////////////////////////////////////////////////////
// [addColumn]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_ListView::addColumn(int pos, char* pText){
  

   GtkCellRenderer* pRenderer = 0;

   pRenderer = gtk_cell_renderer_text_new();
   
   gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(this->pListView), -1,      
                                               pText, pRenderer, "text", pos, NULL);

   return(C_GTK_LISTVIEW_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [addItem]
//////////////////////////////////////////////////////////////////////////////////
GtkTreeIter C_GTK_ListView::addItem(char* pText, int column, GtkListStore* pLStore, GtkTreeIter* pTreeIter){
  
   GtkTreeIter TreeIter;
   
   if(pTreeIter) TreeIter = *pTreeIter;
   else          gtk_list_store_append(pLStore, &TreeIter);
   
   GValue value = G_VALUE_INIT;
   g_value_init(&value, G_TYPE_STRING);
   g_value_set_string(&value, pText);
    
   gtk_list_store_set_value(pLStore, &TreeIter, column, &value);
   
   return(TreeIter);
}
//////////////////////////////////////////////////////////////////////////////////
// [addItem]
//////////////////////////////////////////////////////////////////////////////////
GtkTreeIter C_GTK_ListView::addItem(char* pText, int column, GtkTreeIter* pTreeIter){
   GtkTreeIter TreeIter;
   
   GtkTreeModel* pModel = gtk_tree_view_get_model(GTK_TREE_VIEW(this->pListView));
   
   if(pTreeIter) TreeIter = *pTreeIter;
   else          gtk_list_store_append(GTK_LIST_STORE(pModel), &TreeIter);

   GValue value = G_VALUE_INIT;
   g_value_init(&value, G_TYPE_STRING);
   g_value_set_string(&value, pText);
   
   gtk_list_store_set_value(GTK_LIST_STORE(pModel), &TreeIter, column, &value);
   
   return(TreeIter);
}
//////////////////////////////////////////////////////////////////////////////////
// [createLStore]
//////////////////////////////////////////////////////////////////////////////////
GtkListStore* C_GTK_ListView::createLStore(int nColumns, GType* pTypes){
   GtkListStore* pLStore = gtk_list_store_newv(nColumns, pTypes);
   return(pLStore);
}
//////////////////////////////////////////////////////////////////////////////////
// [addTStore]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_ListView::addLStore(GtkListStore* pLStore){
   
   gtk_tree_view_set_model(GTK_TREE_VIEW(this->pListView), GTK_TREE_MODEL(pLStore));
   g_object_unref(pLStore);
  
   return(C_GTK_LISTVIEW_READY);
}
/*
//////////////////////////////////////////////////////////////////////////////////
// [getSelect]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_TreeView::getSelect(){
  
   GtkTreeSelection* tsel = gtk_tree_view_get_selection(GTK_TREE_VIEW(this->pTreeView));
   GtkTreeIter iter;
   GtkTreeModel* tm;
   GtkTreePath* path;
   
   int* i;
   
   if(gtk_tree_selection_get_selected(tsel ,&tm ,&iter)){
     
      path = gtk_tree_model_get_path(tm , &iter);
      i = gtk_tree_path_get_indices(path);
      
      return i[0];
   }
   return -1;
}
*/
//////////////////////////////////////////////////////////////////////////////////
// [getListSelect]
//////////////////////////////////////////////////////////////////////////////////
GtkTreeSelection* C_GTK_ListView::getListSelect(){
  
  GtkTreeSelection* pSelect = 0;
  
  pSelect = gtk_tree_view_get_selection(GTK_TREE_VIEW(this->pListView));
  
  return(pSelect);
}
//////////////////////////////////////////////////////////////////////////////////
// [setListSelect]
//////////////////////////////////////////////////////////////////////////////////
GtkTreeSelection* C_GTK_ListView::setListSelect(GtkSelectionMode type){
  
  GtkTreeSelection* pSelect = 0;
  
  pSelect = this->getListSelect();
  
  gtk_tree_selection_set_mode(pSelect, type);
  
  return(pSelect);
}
//////////////////////////////////////////////////////////////////////////////////
// [clear]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_ListView::clear(){
  
   GtkTreeModel* pModel = gtk_tree_view_get_model(GTK_TREE_VIEW(this->pListView));
   
   gtk_list_store_clear(GTK_LIST_STORE(pModel));
   
   return(C_GTK_LISTVIEW_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [setFont]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_ListView::setFont(const char* pFont){
  
   if(this->status != C_GTK_LISTVIEW_READY || !pFont) return(C_GTK_LISTVIEW_ERROR);
   
   PangoFontDescription *font_desc;
   
   font_desc = pango_font_description_from_string(pFont);
   gtk_widget_modify_font(this->pListView, font_desc);
   pango_font_description_free(font_desc);

   return(C_GTK_LISTVIEW_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [setBackground]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_ListView::setBackground(const GdkRGBA* pColor){
  
   if(this->status != C_GTK_LISTVIEW_READY || !pColor) return(C_GTK_LISTVIEW_ERROR);

   gtk_widget_override_background_color(this->pListView, GTK_STATE_FLAG_NORMAL, pColor);

   return(C_GTK_LISTVIEW_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [setColor]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_ListView::setColor(const GdkRGBA* pColor){
  
   if(this->status != C_GTK_LISTVIEW_READY || !pColor) return(C_GTK_LISTVIEW_ERROR);
   
   gtk_widget_override_color(this->pListView, GTK_STATE_FLAG_NORMAL, pColor);

   return(C_GTK_LISTVIEW_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [hide]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_ListView::hide(){
  
   if(this->status != C_GTK_LISTVIEW_READY) return(C_GTK_LISTVIEW_ERROR);
   
   gtk_widget_hide(this->pListView);
   
   return(C_GTK_LISTVIEW_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [show]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_ListView::show(){
  
   if(this->status != C_GTK_LISTVIEW_READY) return(C_GTK_LISTVIEW_ERROR);
   
   gtk_widget_show(this->pListView);
   
   return(C_GTK_LISTVIEW_READY);
}