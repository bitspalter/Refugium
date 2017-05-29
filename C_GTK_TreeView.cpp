//////////////////////////////////////////////////////////////////////////////////
// [ GTK_TreeView_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_GTK_TreeView.hpp"

//////////////////////////////////////////////////////////////////////////////////
// [Konstructor]  
//////////////////////////////////////////////////////////////////////////////////
C_GTK_TreeView::C_GTK_TreeView(){
   this->pTreeView = 0;
   this->status    = C_GTK_TREEVIEW_ERROR;
}
//////////////////////////////////////////////////////////////////////////////////
// [Destructor]
//////////////////////////////////////////////////////////////////////////////////
C_GTK_TreeView::~C_GTK_TreeView(){

}
//////////////////////////////////////////////////////////////////////////////////
// [create]
//////////////////////////////////////////////////////////////////////////////////
GtkWidget* C_GTK_TreeView::create(){

   if(this->status == C_GTK_TREEVIEW_READY) return(this->pTreeView);

   this->pTreeView = gtk_tree_view_new();

   if(!this->pTreeView) return(C_GTK_TREEVIEW_ERROR);
   
   this->status = C_GTK_TREEVIEW_READY;
   
   return(this->pTreeView);
}
//////////////////////////////////////////////////////////////////////////////////
// [addColumn]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_TreeView::addColumn(int pos, char* pText){
  
   if(this->status != C_GTK_TREEVIEW_READY || !pText) return(C_GTK_TREEVIEW_ERROR);
   
   GtkCellRenderer* pRenderer = gtk_cell_renderer_text_new();
   
   gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(this->pTreeView), -1,      
                                               pText, pRenderer, "text", pos, NULL);
   
   return(C_GTK_TREEVIEW_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [addItem]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_TreeView::addItem(char* pText, GtkTreeStore* pTStore, 
			    GtkTreeIter* pParentTreeIter, GtkTreeIter* pNewTreeIter){
  
   if(this->status != C_GTK_TREEVIEW_READY || !pText || !pTStore || !pNewTreeIter) return(C_GTK_TREEVIEW_ERROR);
   
   gtk_tree_store_append(pTStore, pNewTreeIter, pParentTreeIter);
   gtk_tree_store_set(pTStore, pNewTreeIter, 0, pText, -1);
   
   return(C_GTK_TREEVIEW_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [addItem]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_TreeView::addItem(char* pText, GtkTreeIter* pParentTreeIter, GtkTreeIter* pNewTreeIter){

   if(this->status != C_GTK_TREEVIEW_READY || !pText || !pNewTreeIter) return(C_GTK_TREEVIEW_ERROR);
   
   GtkTreeModel* pModel = gtk_tree_view_get_model(GTK_TREE_VIEW(this->pTreeView));
   
   gtk_tree_store_append(GTK_TREE_STORE(pModel), pNewTreeIter, pParentTreeIter);
   gtk_tree_store_set(GTK_TREE_STORE(pModel), pNewTreeIter, 0, pText, -1);
   
   return(C_GTK_TREEVIEW_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [createTStore]
//////////////////////////////////////////////////////////////////////////////////
GtkTreeStore* C_GTK_TreeView::createTStore(){
  
   if(this->status != C_GTK_TREEVIEW_READY) return(C_GTK_TREEVIEW_ERROR);
   
   GtkTreeStore* pTStore = gtk_tree_store_new(1, G_TYPE_STRING);
   
   return(pTStore);
}
//////////////////////////////////////////////////////////////////////////////////
// [addTStore]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_TreeView::addTStore(GtkTreeStore* pTStore){
  
   if(this->status != C_GTK_TREEVIEW_READY || !pTStore) return(C_GTK_TREEVIEW_ERROR);
   
   gtk_tree_view_set_model(GTK_TREE_VIEW(this->pTreeView), GTK_TREE_MODEL(pTStore));
   g_object_unref(pTStore);
  
   return(C_GTK_TREEVIEW_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [getSelect]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_TreeView::getSelect(){
  
   GtkTreeIter iter;
   GtkTreeModel* tm;
   GtkTreePath* path;
   
   if(this->status != C_GTK_TREEVIEW_READY) return(C_GTK_TREEVIEW_ERROR);
      
   GtkTreeSelection* pSelect = this->getTreeSelect();
   
   int* i;
   
   if(gtk_tree_selection_get_selected(pSelect ,&tm ,&iter)){
     
      path = gtk_tree_model_get_path(tm , &iter);
      i = gtk_tree_path_get_indices(path);
      
      return i[0];
   }
   return -1;
}
//////////////////////////////////////////////////////////////////////////////////
// [getTreeSelect]
//////////////////////////////////////////////////////////////////////////////////
GtkTreeSelection* C_GTK_TreeView::getTreeSelect(){

   if(this->status != C_GTK_TREEVIEW_READY) return(C_GTK_TREEVIEW_ERROR);
   
   GtkTreeSelection* pSelect = gtk_tree_view_get_selection(GTK_TREE_VIEW(this->pTreeView));
  
   return(pSelect);
}
//////////////////////////////////////////////////////////////////////////////////
// [setTreeSelect]
//////////////////////////////////////////////////////////////////////////////////
GtkTreeSelection* C_GTK_TreeView::setTreeSelect(GtkSelectionMode type){

   if(this->status != C_GTK_TREEVIEW_READY) return(C_GTK_TREEVIEW_ERROR);

   GtkTreeSelection* pSelect = this->getTreeSelect();
  
   gtk_tree_selection_set_mode(pSelect, type);
  
   return(pSelect);
}
//////////////////////////////////////////////////////////////////////////////////
// [getModel]
//////////////////////////////////////////////////////////////////////////////////
GtkTreeModel* C_GTK_TreeView::getModel(){
  
   if(this->status != C_GTK_TREEVIEW_READY) return(C_GTK_TREEVIEW_ERROR);

   return(gtk_tree_view_get_model(GTK_TREE_VIEW(this->pTreeView)));
}
//////////////////////////////////////////////////////////////////////////////////
// [getTreePathByIter]
//////////////////////////////////////////////////////////////////////////////////
GtkTreePath* C_GTK_TreeView::getTreePathByIter(GtkTreeIter* pTreeIter){
  
   if(this->status != C_GTK_TREEVIEW_READY || !pTreeIter) return(C_GTK_TREEVIEW_ERROR);
  
   GtkTreeModel* pTreeModel = gtk_tree_view_get_model(GTK_TREE_VIEW(this->pTreeView));
   
   return(gtk_tree_model_get_path(pTreeModel, pTreeIter));
}
//////////////////////////////////////////////////////////////////////////////////
// [setSelectByTreePath]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_TreeView::setSelectByTreePath(GtkTreePath* pTreePath){
  
   if(this->status != C_GTK_TREEVIEW_READY || !pTreePath) return(C_GTK_TREEVIEW_ERROR);
   
   GtkTreeSelection* pTreeSelect = gtk_tree_view_get_selection(GTK_TREE_VIEW(this->pTreeView));
   gtk_tree_selection_select_path(pTreeSelect, pTreePath);
  
   return(C_GTK_TREEVIEW_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [expandRow]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_TreeView::expandRow(GtkTreePath* pTreePath){
  
   if(this->status != C_GTK_TREEVIEW_READY || !pTreePath) return(C_GTK_TREEVIEW_ERROR);
   
   gtk_tree_view_expand_to_path(GTK_TREE_VIEW(this->pTreeView), pTreePath);
   
   //gtk_tree_view_expand_row(GTK_TREE_VIEW(this->pTreeView), pTreePath, false);
   
   return(C_GTK_TREEVIEW_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [setPathDown]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_TreeView::setPathDown(GtkTreePath* pTreePath){
  
   if(this->status != C_GTK_TREEVIEW_READY || !pTreePath) return(C_GTK_TREEVIEW_ERROR);
   
   gtk_tree_path_down(pTreePath);
   
   return(C_GTK_TREEVIEW_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [setPathUp]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_TreeView::setPathUp(GtkTreePath* pTreePath){
  
   if(this->status != C_GTK_TREEVIEW_READY || !pTreePath) return(C_GTK_TREEVIEW_ERROR);
  
   if(!gtk_tree_path_up(pTreePath)) return(C_GTK_TREEVIEW_ERROR);
      
   return(C_GTK_TREEVIEW_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [setPathNext]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_TreeView::setPathNext(GtkTreePath* pTreePath){
  
   if(this->status != C_GTK_TREEVIEW_READY || !pTreePath) return(C_GTK_TREEVIEW_ERROR);
  
   gtk_tree_path_next(pTreePath);
   
   return(C_GTK_TREEVIEW_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [setPathPrev]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_TreeView::setPathPrev(GtkTreePath* pTreePath){
  
   if(this->status != C_GTK_TREEVIEW_READY || !pTreePath) return(C_GTK_TREEVIEW_ERROR);
  
   if(!gtk_tree_path_prev(pTreePath)) return(C_GTK_TREEVIEW_ERROR);
   
   return(C_GTK_TREEVIEW_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [scrollToPath]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_TreeView::scrollToPath(GtkTreePath* pTreePath){
  
   if(this->status != C_GTK_TREEVIEW_READY || !pTreePath) return(C_GTK_TREEVIEW_ERROR);
  
   gtk_tree_view_scroll_to_cell(GTK_TREE_VIEW(this->pTreeView), pTreePath, NULL, false, 0, 0);
   
   return(C_GTK_TREEVIEW_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [setPathFree]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_TreeView::setPathFree(GtkTreePath* pTreePath){
 
   if(this->status != C_GTK_TREEVIEW_READY || !pTreePath) return(C_GTK_TREEVIEW_ERROR);
   
   gtk_tree_path_free(pTreePath);
   
   return(C_GTK_TREEVIEW_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [expandAll]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_TreeView::expandAll(){
  
   if(this->status != C_GTK_TREEVIEW_READY) return(C_GTK_TREEVIEW_ERROR);
   
   gtk_tree_view_expand_all(GTK_TREE_VIEW(this->pTreeView));
   
   return(C_GTK_TREEVIEW_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [collapseAll]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_TreeView::collapseAll(){
  
   if(this->status != C_GTK_TREEVIEW_READY) return(C_GTK_TREEVIEW_ERROR);
   
   gtk_tree_view_collapse_all(GTK_TREE_VIEW(this->pTreeView));
   
   return(C_GTK_TREEVIEW_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [clear]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_TreeView::clear(){
  
   if(this->status != C_GTK_TREEVIEW_READY) return(C_GTK_TREEVIEW_ERROR);
   
   GtkTreeModel* pModel = gtk_tree_view_get_model(GTK_TREE_VIEW(this->pTreeView));
   
   gtk_tree_store_clear(GTK_TREE_STORE(pModel));
   
   return(C_GTK_TREEVIEW_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [setFont]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_TreeView::setFont(const char* pFont){
  
   if(this->status != C_GTK_TREEVIEW_READY || !pFont) return(C_GTK_TREEVIEW_ERROR);
   
   PangoFontDescription *font_desc;
   
   font_desc = pango_font_description_from_string(pFont);
   gtk_widget_modify_font(this->pTreeView, font_desc);
   pango_font_description_free(font_desc);

   return(C_GTK_TREEVIEW_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [setBackground]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_TreeView::setBackground(const GdkRGBA* pColor){
  
   if(this->status != C_GTK_TREEVIEW_READY || !pColor) return(C_GTK_TREEVIEW_ERROR);

   gtk_widget_override_background_color(this->pTreeView, GTK_STATE_FLAG_NORMAL, pColor);

   return(C_GTK_TREEVIEW_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [setColor]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_TreeView::setColor(const GdkRGBA* pColor){
  
   if(this->status != C_GTK_TREEVIEW_READY || !pColor) return(C_GTK_TREEVIEW_ERROR);
   
   gtk_widget_override_color(this->pTreeView, GTK_STATE_FLAG_NORMAL, pColor);

   return(C_GTK_TREEVIEW_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [hide]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_TreeView::hide(){
  
   if(this->status != C_GTK_TREEVIEW_READY) return(C_GTK_TREEVIEW_ERROR);
   
   gtk_widget_hide(this->pTreeView);
   
   return(C_GTK_TREEVIEW_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [show]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_TreeView::show(){
  
   if(this->status != C_GTK_TREEVIEW_READY) return(C_GTK_TREEVIEW_ERROR);
   
   gtk_widget_show(this->pTreeView);
   
   return(C_GTK_TREEVIEW_READY);
}