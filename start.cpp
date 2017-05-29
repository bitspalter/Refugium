#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

using namespace std;

//////////////////////////////////////////////////////////////////
//
//
// Cryptainer V0.03
//
// L.ey : 28.12.2016 : µ~
//
//
//////////////////////////////////////////////////////////////////

   #include "C_App.hpp"

   C_App CApp;

   void destroy(GtkWidget *widget, gpointer data);
   void configure_callback(GtkWindow *window, GdkEvent *event, gpointer data);
   
   void tree_selection_changed_cb(GtkTreeSelection *selection, gpointer data);

   gboolean view_onButtonPressed(GtkWidget *treeview, GdkEventButton *event, gpointer userdata);
   gboolean view_onPopupMenu(GtkWidget *treeview, gpointer userdata);
   void view_popup_menu(GtkWidget *treeview, GdkEventButton *event, gpointer userdata);
   
   void view_popup_menu_onShowTxt(GtkWidget *menuitem, gpointer userdata);
   void view_popup_menu_onShowHex(GtkWidget *menuitem, gpointer userdata);
   
   void view_popup_menu_onExEnc(GtkWidget *menuitem, gpointer userdata);
   void view_popup_menu_onExDec(GtkWidget *menuitem, gpointer userdata);
   
   void view_popup_menu_onAddFolder(GtkWidget *menuitem, gpointer userdata);
   void view_popup_menu_onDelFolder(GtkWidget *menuitem, gpointer userdata);
   
   void view_popup_menu_onDelFile(GtkWidget *menuitem, gpointer userdata);
   void view_popup_menu_onAddFile(GtkWidget *menuitem, gpointer userdata);
   
   void view_popup_menu_onNewFile(GtkWidget *menuitem, gpointer userdata);
   void view_popup_menu_onNewFolder(GtkWidget *menuitem, gpointer userdata);
   
   void view_popup_menu_onEditFile(GtkWidget *menuitem, gpointer userdata);
   
   void sub_popup_menu_onQuit(GtkWidget *menuitem, gpointer userdata);
   void sub_popup_menu_onSave(GtkWidget *menuitem, gpointer userdata);
   
   void on_open_file();
   void on_open_folder();
   
   void on_encrypt();
   void on_decrypt();
   
   void on_decrypt_filev();
   
   /////////////////////////////////////////
   // Drag And Drop
   #define MAX_XDS_ATOM_VAL_LEN 4096
   #define XDS_ATOM   gdk_atom_intern  ("XdndDirectSave0", FALSE)
   #define TEXT_ATOM  gdk_atom_intern  ("text/plain", FALSE)
   #define OCTET_ATOM gdk_atom_intern  ("application/octet-stream", FALSE)
   #define XFR_ATOM   gdk_atom_intern  ("XdndFileRoller0", FALSE)
   
   enum {
      TARGET_URI_LIST,
      TARGET_UTF8_STRING,
      TARGET_TEXT,
      TARGET_COMPOUND_TEXT,
      TARGET_STRING,
      TARGET_TEXT_PLAIN,
      TARGET_MOZ_URL,
      TARGET_APPLICATION_X_COLOR,
      TARGET_GTK_NOTEBOOK_TAB,
      TARGET_XDS
   };

   struct _GtkSelectionData {
      GdkAtom selection;
      GdkAtom target;
      GdkAtom type;
      gint    format;
      guchar *data;
      gint    length;
   };
   
   struct _GdkDragContext {
      GdkDragProtocol protocol;
      gboolean        is_source;
      GdkWindow       *source_window;
      GdkWindow       *dest_window;
      GList           *targets;
      GdkDragAction   actions;
      GdkDragAction   suggested_action;
      GdkDragAction   action;
      guint32         start_time;
   };

   const int DROP_FOLDER = 0x01;
   const int DROP_FILE   = 0x02;
   
   int create_DragNDrop(GtkWidget* widget, int type);

   gboolean on_db_drag_motion(GtkWidget* widget, GdkDragContext* dc, gint x, gint y, guint time, gpointer user_data);
   void     on_db_drag_leave(GtkWidget* widget, GdkDragContext* dc, guint time, gpointer user_data);
   gboolean on_db_drag_drop(GtkWidget* widget, GdkDragContext* dc, gint x, gint y, guint time, gpointer user_data);
   void     on_db_drag_data_received(GtkWidget* widget, GdkDragContext* dc, gint x, gint y, GtkSelectionData* sd, guint info, guint time, gpointer user_data);

   void     on_db_drag_begin(GtkWidget* widget, GdkDragContext *dc, gpointer user_data);
   void     on_db_drag_data_get(GtkWidget* widget, GdkDragContext *dc, GtkSelectionData *sd, guint info, guint timestamp, gpointer user_data);
   void     on_db_drag_data_delete(GtkWidget* widget, GdkDragContext *dc, gpointer user_data);
   void     on_db_drag_end(GtkWidget* widget, GdkDragContext *dc, gpointer user_data);
   
//////////////////////////////////////////////////////////////////////////////
// [ main ]
//////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]){

   CApp.init(&argc, &argv);
   CApp.create();

   g_signal_connect(CApp.sbutton.openfile.getButton(),   "clicked",  G_CALLBACK(on_open_file),       NULL);
   g_signal_connect(CApp.sbutton.openfolder.getButton(), "clicked",  G_CALLBACK(on_open_folder),     NULL);
   g_signal_connect(CApp.sbutton.encrypt.getButton(),    "clicked",  G_CALLBACK(on_encrypt),         NULL);
   g_signal_connect(CApp.sbutton.decrypt.getButton(),    "clicked",  G_CALLBACK(on_decrypt),         NULL);
   g_signal_connect(CApp.sbutton.decryptv.getButton(),   "clicked",  G_CALLBACK(on_decrypt_filev),   NULL);
   g_signal_connect(CApp.CWindow.getWindow(),   "destroy",           G_CALLBACK(destroy),            NULL);
   g_signal_connect(CApp.CWindow.getWindow(),   "configure-event",   G_CALLBACK(configure_callback), NULL);
  
   //////////////////////////////////
   
   GtkTreeSelection* pSelect = CApp.CTreeView.setTreeSelect(GTK_SELECTION_SINGLE);
   g_signal_connect(G_OBJECT(pSelect), "changed", G_CALLBACK(tree_selection_changed_cb), NULL);
   
   g_signal_connect(CApp.CTreeView.getTree(), "button-press-event", (GCallback)view_onButtonPressed, NULL);
   g_signal_connect(CApp.CListView.getList(), "button-press-event", (GCallback)view_onButtonPressed, NULL);

   /////////////////////////////////
   
   create_DragNDrop(CApp.CTreeView.getTree(), DROP_FOLDER);
   create_DragNDrop(CApp.CListView.getList(), DROP_FILE);
   //create_DragNDrop(CApp.sbutton.openfile.getButton(), 0x01);
   /////////////////////////////////
   
   CApp.main();

   return 0;
}
//////////////////////////////////////////////////////////////////////////////
// [ on_db_drag_data_received ]
//////////////////////////////////////////////////////////////////////////////
void on_db_drag_data_received(GtkWidget* widget, GdkDragContext* dc, gint x, gint y, GtkSelectionData* sd, guint info, guint time, gpointer user_data){

   gchar  *text;
   gchar **uris;
   gchar  *filename;

   switch(info){
     case TARGET_STRING:               cout << "TARGET_STRING"        << endl; break;
     case TARGET_UTF8_STRING:          cout << "TARGET_UTF8_STRING"   << endl; break;
     case TARGET_COMPOUND_TEXT:        cout << "TARGET_COMPOUND_TEXT" << endl; break;
     case TARGET_TEXT:                 cout << "TARGET_TEXT"          << endl; break;
     case TARGET_TEXT_PLAIN:           cout << "TARGET_TEXT_PLAIN"    << endl; break;
     case TARGET_MOZ_URL:              cout << "TARGET_MOZ_URL"       << endl; break;
     case TARGET_URI_LIST: {

        if(sd->format != 8 || sd->length == 0) break;
	
        /////////////////////////////
        
        C_Array* pCA_Folder = 0;
        C_Array* pCA_File   = 0;
   
        CApp.CDA_AddFolder._Clear();
        CApp.CDA_AddFiles._Clear();
	
	/////////////////////////////

        text = g_strndup((const gchar*)sd->data, sd->length);
        uris = g_uri_list_extract_uris(text);
        g_free(text);

	//////////////////////////////

        for(int n = 0; uris[n] != NULL; ++n){
           filename = g_filename_from_uri(uris[n], NULL, NULL);

	   struct stat st;
           stat(filename, &st);

           if(st.st_mode & S_IFREG){
              if(!(pCA_File = CApp.CDA_AddFiles._Add_Item(1, strlen(filename) + 1))) return;
              memcpy(pCA_File->_Get_pBuffer(), filename, strlen(filename) + 1);
	       
	   }else 
	   if(st.st_mode & S_IFDIR){
              if(!(pCA_Folder = CApp.CDA_AddFolder._Add_Item(1, strlen(filename) + 1))) return;
              memcpy(pCA_Folder->_Get_pBuffer(), filename, strlen(filename) + 1);
	   }

           if(G_LIKELY(filename != NULL)) g_free(uris[n]);
        }
        
        CApp.add_drop();

        break;
     }
     case TARGET_APPLICATION_X_COLOR:  cout << "TARGET_APPLICATION_X_COLOR" << endl; break;
     case TARGET_GTK_NOTEBOOK_TAB:     cout << "TARGET_GTK_NOTEBOOK_TAB"    << endl; break;
   }

   gtk_drag_finish(dc, true, false, time);
}
//////////////////////////////////////////////////////////////////////////////
// [ on_db_drag_drop ]
//////////////////////////////////////////////////////////////////////////////
gboolean on_db_drag_drop(GtkWidget* widget, GdkDragContext* dc, gint x, gint y, guint time, gpointer user_data){
   GdkAtom target_atom = GDK_NONE;
   int type = GPOINTER_TO_UINT(user_data);
   
   switch(type){
      case 0x00: target_atom = gdk_atom_intern("text/html", TRUE); break;
      case 0x01: target_atom = gdk_atom_intern("STRING",    TRUE); break;
      case 0x02: target_atom = gdk_atom_intern("number",    TRUE); break;
   }
   
   if(target_atom != GDK_NONE) gtk_drag_get_data(widget, dc, target_atom, time);
   
   return(true);
}
//////////////////////////////////////////////////////////////////////////////
// [ on_db_drag_leave ]
//////////////////////////////////////////////////////////////////////////////
void on_db_drag_leave(GtkWidget* widget, GdkDragContext* dc, guint time, gpointer user_data){
   gtk_drag_unhighlight(widget);
   g_object_set_data(G_OBJECT(widget), "highlighted", (gpointer)FALSE);
}
//////////////////////////////////////////////////////////////////////////////
// [ on_db_drag_motion ]
//////////////////////////////////////////////////////////////////////////////
gboolean on_db_drag_motion(GtkWidget* widget, GdkDragContext* dc, gint x, gint y, guint time, gpointer user_data){
  
   GdkAtom target_atom;
   static int counter = 0;

   target_atom = gtk_drag_dest_find_target(widget, dc, NULL);
   
   if(target_atom != GDK_NONE){
      gdk_drag_status(dc, GDK_ACTION_PRIVATE, time);
      if(!g_object_get_data(G_OBJECT(widget), "highlighted")){
         g_object_set_data(G_OBJECT(widget), "highlighted", (gpointer)TRUE);
         gtk_drag_highlight(widget);
      }

      return(TRUE);
   }
   
   gdk_drag_status(dc, GDK_ACTION_PRIVATE, time);

   return(FALSE);
}
//////////////////////////////////////////////////////////////////////////////
// [ create_DragNDrop ]
//////////////////////////////////////////////////////////////////////////////
int create_DragNDrop(GtkWidget* widget, int type){
  
   static const GtkTargetEntry targets[] = {
      {"text/uri-list",       0, TARGET_URI_LIST},
      {"text/x-moz-url",      0, TARGET_MOZ_URL},
      {"UTF8_STRING",         0, TARGET_UTF8_STRING},
      {"TEXT",                0, TARGET_TEXT},
      {"COMPOUND_TEXT",       0, TARGET_COMPOUND_TEXT},
      {"STRING",              0, TARGET_STRING},
      {"text/plain",          0, TARGET_TEXT_PLAIN},
      {"application/x-color", 0, TARGET_APPLICATION_X_COLOR},
      {"GTK_NOTEBOOK_TAB",    GTK_TARGET_SAME_APP, TARGET_GTK_NOTEBOOK_TAB},};

   gtk_drag_dest_set(GTK_WIDGET(widget),
                     GtkDestDefaults(GTK_DEST_DEFAULT_MOTION|GTK_DEST_DEFAULT_HIGHLIGHT|GTK_DEST_DEFAULT_DROP),
                     targets, G_N_ELEMENTS(targets),
                     GdkDragAction(GDK_ACTION_COPY|GDK_ACTION_LINK|GDK_ACTION_MOVE));
  
   g_signal_connect(G_OBJECT(widget), "drag-data-received", G_CALLBACK(on_db_drag_data_received), GUINT_TO_POINTER(type));
   //g_signal_connect(G_OBJECT(widget), "drag-drop",          G_CALLBACK(on_db_drag_drop),          GUINT_TO_POINTER(type));
   //g_signal_connect(G_OBJECT(widget), "drag-motion",        G_CALLBACK(on_db_drag_motion),        GUINT_TO_POINTER(type));
   //g_signal_connect(G_OBJECT(widget), "drag-leave",         G_CALLBACK(on_db_drag_leave),         GUINT_TO_POINTER(type));

   //////////////////////////////////////////////////////
   
   /* Target types for dragging from the view */
   static const GtkTargetEntry drag_targets[] = {
      {"XdndDirectSave0", 0, TARGET_XDS,},};

   gtk_drag_source_set(GTK_WIDGET(widget), GDK_BUTTON1_MASK, 
		       drag_targets, G_N_ELEMENTS(drag_targets), 
		       GdkDragAction(GDK_ACTION_COPY|GDK_ACTION_MOVE|GDK_ACTION_LINK));
   
   g_signal_connect(G_OBJECT(widget), "drag-begin",       G_CALLBACK(on_db_drag_begin),       GUINT_TO_POINTER(type));
   g_signal_connect(G_OBJECT(widget), "drag-data-get",    G_CALLBACK(on_db_drag_data_get),    GUINT_TO_POINTER(type));
   g_signal_connect(G_OBJECT(widget), "drag-data-delete", G_CALLBACK(on_db_drag_data_delete), GUINT_TO_POINTER(type));
   g_signal_connect(G_OBJECT(widget), "drag-end",         G_CALLBACK(on_db_drag_end),         GUINT_TO_POINTER(type));
  
   return 0;
}
//////////////////////////////////////////////////////////////////////////////
// [ on_db_drag_begin ]
//////////////////////////////////////////////////////////////////////////////
void on_db_drag_begin(GtkWidget* widget, GdkDragContext *dc, gpointer user_data){

   char* xds_filename = "test.xtc";
   
   gdk_property_change(gdk_drag_context_get_source_window(dc), 
                       XDS_ATOM, TEXT_ATOM, 8, GDK_PROP_MODE_REPLACE, 
                       (guchar*) xds_filename, strlen(xds_filename));
   
}
//////////////////////////////////////////////////////////////////////////////
// [ on_db_drag_data_get ]
//////////////////////////////////////////////////////////////////////////////
void on_db_drag_data_get(GtkWidget* widget, GdkDragContext *dc, GtkSelectionData *sd, guint info, guint timestamp, gpointer user_data){

   char  *uri;
   GFile *destination;
   GFile *destination_folder;
   
   if(!dc) return;
   if(!gdk_drag_context_get_source_window(dc)) return;

   gdk_property_get(gdk_drag_context_get_source_window(dc), 
                    XDS_ATOM, TEXT_ATOM, 0, MAX_XDS_ATOM_VAL_LEN, FALSE, 
                    NULL, NULL, NULL, (unsigned char **) &uri);
   
   if(!uri) return;

   destination = g_file_new_for_uri(uri);
   destination_folder = g_file_get_parent(destination);

   g_object_unref(destination);

   /////////////////////////////////////////////////////////////////
   
   GtkTreeIter iter;
   GtkTreeModel *model;
   gchar *author;

   GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(widget));

   if(gtk_tree_selection_get_selected(selection, &model, &iter)){
      if(GPOINTER_TO_UINT(user_data) == DROP_FOLDER){
	 string TString;
         if(CApp.getPathByTree(&iter, &TString)){
            CApp.extract_folder_decrypt(&TString, g_file_get_path(destination_folder));
	 }
      }else
      if(GPOINTER_TO_UINT(user_data) == DROP_FILE){ 
         gtk_tree_model_get(model, &iter, 1, &author, -1);
         unsigned long pData = strtoul(author, NULL, 0);
	 CApp.extract_file_decrypt(pData, g_file_get_path(destination_folder));
	 g_free(author);
      }
   }

   g_object_unref(destination_folder);
}
//////////////////////////////////////////////////////////////////////////////
// [ on_db_drag_data_delete ]
//////////////////////////////////////////////////////////////////////////////
void on_db_drag_data_delete(GtkWidget* widget, GdkDragContext *dc, gpointer user_data){
   //cout << "on_db_drag_data_delete" << endl;
}
//////////////////////////////////////////////////////////////////////////////
// [ on_db_drag_end ]
//////////////////////////////////////////////////////////////////////////////
void on_db_drag_end(GtkWidget* widget, GdkDragContext *dc, gpointer user_data){
   gdk_property_delete(gdk_drag_context_get_source_window(dc), XDS_ATOM);
}  
//////////////////////////////////////////////////////////////////////////////
// [ on_open_folder ]
//////////////////////////////////////////////////////////////////////////////
void on_open_folder(){
   CApp.open_folder();
}
//////////////////////////////////////////////////////////////////////////////
// [ on_open_file ]
//////////////////////////////////////////////////////////////////////////////
void on_open_file(){
   CApp.open_file();
}
//////////////////////////////////////////////////////////////////////////////
// [ on_encrypt ]
//////////////////////////////////////////////////////////////////////////////
void on_encrypt(){
   CApp.encrypt();
}
//////////////////////////////////////////////////////////////////////////////
// [ on_decrypt ]
//////////////////////////////////////////////////////////////////////////////
void on_decrypt(){
   CApp.decrypt();
}
//////////////////////////////////////////////////////////////////////////////
// [ on_decrypt_filev ]
//////////////////////////////////////////////////////////////////////////////
void on_decrypt_filev(){
   CApp.decrypt_filev();
}
//////////////////////////////////////////////////////////////////////////////
// [ view_onButtonPressed ]
//////////////////////////////////////////////////////////////////////////////
gboolean view_onButtonPressed(GtkWidget *treeview, GdkEventButton *event, gpointer userdata){
   if(event->type == GDK_BUTTON_PRESS  &&  event->button == 3){
      GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
      
      if(gtk_tree_selection_count_selected_rows(selection) <= 1){
         GtkTreePath *path;
         if(gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(treeview), (gint)event->x, (gint)event->y, &path, NULL, NULL, NULL)){
            gtk_tree_selection_unselect_all(selection);
            gtk_tree_selection_select_path(selection, path);
            gtk_tree_path_free(path);
         }
      }
      view_popup_menu(treeview, event, userdata);
      return(TRUE); /* we handled this */
   }else
   if(event->type == GDK_BUTTON_PRESS  &&  event->button == 1){
     
     /*
      cout << "Linke Maustaste" << endl; 
      
      GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
      GtkTreePath* path;
      GtkTreeIter iter;
      GtkTreeModel* model;
      
      if(gtk_tree_selection_get_selected(selection, &model, &iter)){
	 path = gtk_tree_model_get_path(model, &iter);
         gchar* pBuffer = gtk_tree_path_to_string(path);
      
         cout << pBuffer << endl;
      
         g_free(pBuffer);
	 
	 gtk_tree_view_expand_row(GTK_TREE_VIEW(treeview), path, false);
         gtk_tree_path_free(path);
      }
      */
   }
     
   return(FALSE); /* we did not handle this */
}
//////////////////////////////////////////////////////////////////////////////
// [ view_popup_menu ]
//////////////////////////////////////////////////////////////////////////////
void view_popup_menu(GtkWidget *treeview, GdkEventButton *event, gpointer userdata){
  
   if((GtkWidget*)treeview == CApp.CTreeView.getTree()){
     
      GtkWidget* mMain    = gtk_menu_new();
      
      ///////////////////////////////////////////////////////////////7
      GtkWidget* mSubExtr = gtk_menu_new();
      GtkWidget* mExtr    = gtk_menu_item_new_with_label("extract");
      GtkWidget* mEnc     = gtk_menu_item_new_with_label("encrypt");
      GtkWidget* mDec     = gtk_menu_item_new_with_label("decrypt");
      
      gtk_menu_item_set_submenu(GTK_MENU_ITEM(mExtr), mSubExtr);
      gtk_menu_shell_append(GTK_MENU_SHELL(mSubExtr), mEnc);
      gtk_menu_shell_append(GTK_MENU_SHELL(mSubExtr), mDec);
      
      ///////////////////////////////////////////////////////////////7
      GtkWidget* mSubAdd  = gtk_menu_new();
      GtkWidget* mAdd     = gtk_menu_item_new_with_label("add");
      GtkWidget* mfolder  = gtk_menu_item_new_with_label("folder");
      GtkWidget* mfile    = gtk_menu_item_new_with_label("file");
      
      gtk_menu_item_set_submenu(GTK_MENU_ITEM(mAdd), mSubAdd);
      gtk_menu_shell_append(GTK_MENU_SHELL(mSubAdd), mfolder);
      gtk_menu_shell_append(GTK_MENU_SHELL(mSubAdd), mfile);
      
      ///////////////////////////////////////////////////////////////7
      GtkWidget* mSubNew  = gtk_menu_new();
      GtkWidget* mNew     = gtk_menu_item_new_with_label("create");
      GtkWidget* mNfolder = gtk_menu_item_new_with_label("folder");
      GtkWidget* mNfile   = gtk_menu_item_new_with_label("file");
      
      gtk_menu_item_set_submenu(GTK_MENU_ITEM(mNew), mSubNew);
      gtk_menu_shell_append(GTK_MENU_SHELL(mSubNew), mNfolder);
      gtk_menu_shell_append(GTK_MENU_SHELL(mSubNew), mNfile);
      
      ///////////////////////////////////////////////////////////////7
      GtkWidget* mDelete  = gtk_menu_item_new_with_label("delete");
      GtkWidget* mCancel  = gtk_menu_item_new_with_label("cancel");
      GtkWidget* mSep     = gtk_separator_menu_item_new();
      
      ///////////////////////////////////////////////////////////////7
      gtk_menu_shell_append(GTK_MENU_SHELL(mMain), mExtr);
      gtk_menu_shell_append(GTK_MENU_SHELL(mMain), mAdd);
      gtk_menu_shell_append(GTK_MENU_SHELL(mMain), mNew);
      gtk_menu_shell_append(GTK_MENU_SHELL(mMain), mDelete);
      gtk_menu_shell_append(GTK_MENU_SHELL(mMain), mSep);
      gtk_menu_shell_append(GTK_MENU_SHELL(mMain), mCancel);

      gtk_widget_show_all(mMain);

      gtk_menu_popup(GTK_MENU(mMain), NULL, NULL, NULL, NULL, (event != NULL) ? event->button : 0, gdk_event_get_time((GdkEvent*)event));
       
      ///////////////////////////////////////////////////////////////7
      g_signal_connect(mEnc,     "activate", (GCallback)view_popup_menu_onExEnc,     treeview);
      g_signal_connect(mDec,     "activate", (GCallback)view_popup_menu_onExDec,     treeview);
      g_signal_connect(mfolder,  "activate", (GCallback)view_popup_menu_onAddFolder, treeview);
      g_signal_connect(mfile,    "activate", (GCallback)view_popup_menu_onAddFile,   treeview);
      g_signal_connect(mNfolder, "activate", (GCallback)view_popup_menu_onNewFolder, treeview);
      g_signal_connect(mNfile,   "activate", (GCallback)view_popup_menu_onNewFile,   treeview);
      g_signal_connect(mDelete,  "activate", (GCallback)view_popup_menu_onDelFolder, treeview);
      
   }else
   if((GtkWidget*)treeview == CApp.CListView.getList()){

      GtkWidget* mMain    = gtk_menu_new();
      
      ///////////////////////////////////////////////////////////////7
      GtkWidget* mSubShow = gtk_menu_new();
      GtkWidget* mShow    = gtk_menu_item_new_with_label("show");
      GtkWidget* mTxt     = gtk_menu_item_new_with_label("as txt");
      GtkWidget* mHex     = gtk_menu_item_new_with_label("as hex");
      
      gtk_menu_item_set_submenu(GTK_MENU_ITEM(mShow), mSubShow);
      gtk_menu_shell_append(GTK_MENU_SHELL(mSubShow), mTxt);
      gtk_menu_shell_append(GTK_MENU_SHELL(mSubShow), mHex);
      
      ///////////////////////////////////////////////////////////////7
      GtkWidget* mSubExtr = gtk_menu_new();
      GtkWidget* mExtr    = gtk_menu_item_new_with_label("extract");
      GtkWidget* mEnc     = gtk_menu_item_new_with_label("encrypt");
      GtkWidget* mDec     = gtk_menu_item_new_with_label("decrypt");
      
      gtk_menu_item_set_submenu(GTK_MENU_ITEM(mExtr), mSubExtr);
      gtk_menu_shell_append(GTK_MENU_SHELL(mSubExtr), mEnc);
      gtk_menu_shell_append(GTK_MENU_SHELL(mSubExtr), mDec);
      
      ///////////////////////////////////////////////////////////////7
      GtkWidget* mEdit    = gtk_menu_item_new_with_label("edit");
      GtkWidget* mDelete  = gtk_menu_item_new_with_label("delete");
      GtkWidget* mCancel  = gtk_menu_item_new_with_label("cancel");
      GtkWidget* mSep     = gtk_separator_menu_item_new();
      
      ///////////////////////////////////////////////////////////////7
      gtk_menu_shell_append(GTK_MENU_SHELL(mMain), mShow);
      gtk_menu_shell_append(GTK_MENU_SHELL(mMain), mExtr);
      gtk_menu_shell_append(GTK_MENU_SHELL(mMain), mEdit);
      gtk_menu_shell_append(GTK_MENU_SHELL(mMain), mDelete);
      gtk_menu_shell_append(GTK_MENU_SHELL(mMain), mSep);
      gtk_menu_shell_append(GTK_MENU_SHELL(mMain), mCancel);

      gtk_widget_show_all(mMain);

      gtk_menu_popup(GTK_MENU(mMain), NULL, NULL, NULL, NULL, (event != NULL) ? event->button : 0, gdk_event_get_time((GdkEvent*)event));
      
      ///////////////////////////////////////////////////////////////7
      g_signal_connect(mTxt,    "activate", (GCallback)view_popup_menu_onShowTxt,  treeview);
      g_signal_connect(mHex,    "activate", (GCallback)view_popup_menu_onShowHex,  treeview);
      g_signal_connect(mEnc,    "activate", (GCallback)view_popup_menu_onExEnc,    treeview);
      g_signal_connect(mDec,    "activate", (GCallback)view_popup_menu_onExDec,    treeview);
      g_signal_connect(mEdit,   "activate", (GCallback)view_popup_menu_onEditFile, treeview);
      g_signal_connect(mDelete, "activate", (GCallback)view_popup_menu_onDelFile,  treeview);
   }
}
//////////////////////////////////////////////////////////////////////////////
// [ view_popup_menu_onDelFile ]
//////////////////////////////////////////////////////////////////////////////
void view_popup_menu_onDelFile(GtkWidget *menuitem, gpointer userdata){
   GtkTreeIter iter;
   GtkTreeModel *model;
   gchar *author;

   GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(userdata));

   if(gtk_tree_selection_get_selected(selection, &model, &iter)){
      if((GtkWidget*)userdata == CApp.CListView.getList()){
         gtk_tree_model_get(model, &iter, 1, &author, -1);
         unsigned long pData = strtoul(author, NULL, 0);
	 CApp.del_file(pData);
	 g_free(author);
      }
   }
}
//////////////////////////////////////////////////////////////////////////////
// [ view_popup_menu_onDelFolder ]
//////////////////////////////////////////////////////////////////////////////
void view_popup_menu_onDelFolder(GtkWidget *menuitem, gpointer userdata){
   GtkTreeIter iter;
   GtkTreeModel *model;
   string TString;

   GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(userdata));

   if(gtk_tree_selection_get_selected(selection, &model, &iter))
      if((GtkWidget*)userdata == CApp.CTreeView.getTree())
         if(CApp.getPathByTree(&iter, &TString)) CApp.del_folder(&TString);
}
//////////////////////////////////////////////////////////////////////////////
// [ view_popup_menu_onAddFolder ]
//////////////////////////////////////////////////////////////////////////////
void view_popup_menu_onAddFolder(GtkWidget *menuitem, gpointer userdata){
   GtkTreeIter iter;
   GtkTreeModel *model;
   string TString;
   
   GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(userdata));

   if(gtk_tree_selection_get_selected(selection, &model, &iter))
      if((GtkWidget*)userdata == CApp.CTreeView.getTree())
         if(CApp.getPathByTree(&iter, &TString)) CApp.add_folder(&TString);
}
//////////////////////////////////////////////////////////////////////////////
// [ view_popup_menu_onAddFile ]
//////////////////////////////////////////////////////////////////////////////
void view_popup_menu_onAddFile(GtkWidget *menuitem, gpointer userdata){
   GtkTreeIter iter;
   GtkTreeModel *model;
   string TString;
   
   GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(userdata));

   if(gtk_tree_selection_get_selected(selection, &model, &iter))
      if((GtkWidget*)userdata == CApp.CTreeView.getTree())
         if(CApp.getPathByTree(&iter, &TString)) CApp.add_file(&TString);
}
//////////////////////////////////////////////////////////////////////////////
// [ view_popup_menu_onExEnc ]
//////////////////////////////////////////////////////////////////////////////
void view_popup_menu_onExEnc(GtkWidget *menuitem, gpointer userdata){
   GtkTreeIter iter;
   GtkTreeModel *model;
   gchar *author;

   GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(userdata));

   if(gtk_tree_selection_get_selected(selection, &model, &iter)){
      if((GtkWidget*)userdata == CApp.CTreeView.getTree()){
	 string TString;
         if(CApp.getPathByTree(&iter, &TString)){
            CApp.extract_folder_encrypt(&TString);
	 }
      }else
      if((GtkWidget*)userdata == CApp.CListView.getList()){
         gtk_tree_model_get(model, &iter, 1, &author, -1);
         unsigned long pData = strtoul(author, NULL, 0);
	 CApp.extract_file_encrypt(pData);
	 g_free(author);
      }
   }
}
//////////////////////////////////////////////////////////////////////////////
// [ view_popup_menu_onExDec ]
//////////////////////////////////////////////////////////////////////////////
void view_popup_menu_onExDec(GtkWidget *menuitem, gpointer userdata){
   GtkTreeIter iter;
   GtkTreeModel *model;
   gchar *author;

   GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(userdata));

   if(gtk_tree_selection_get_selected(selection, &model, &iter)){
      if((GtkWidget*)userdata == CApp.CTreeView.getTree()){
	 string TString;
         if(CApp.getPathByTree(&iter, &TString)){
            CApp.extract_folder_decrypt(&TString, 0);
	 }
      }else
      if((GtkWidget*)userdata == CApp.CListView.getList()){
         gtk_tree_model_get(model, &iter, 1, &author, -1);
         unsigned long pData = strtoul(author, NULL, 0);
	 CApp.extract_file_decrypt(pData, 0);
	 g_free(author);
      }
   }
}
//////////////////////////////////////////////////////////////////////////////
// [ view_popup_menu_onShowTxt ]
//////////////////////////////////////////////////////////////////////////////
void view_popup_menu_onShowTxt(GtkWidget *menuitem, gpointer userdata){
   GtkTreeIter iter;
   GtkTreeModel *model;
   gchar *author;

   GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(userdata));

   if(gtk_tree_selection_get_selected(selection, &model, &iter)){
      if((GtkWidget*)userdata == CApp.CListView.getList()){
         gtk_tree_model_get(model, &iter, 1, &author, -1);
         unsigned long pData = strtoul(author, NULL, 0);
	 CApp.decrypt_filevF(pData, C_APP_TXT);
      }
      g_free(author);
   }
}
//////////////////////////////////////////////////////////////////////////////
// [ view_popup_menu_onShowHex ]
//////////////////////////////////////////////////////////////////////////////
void view_popup_menu_onShowHex(GtkWidget *menuitem, gpointer userdata){
   GtkTreeIter iter;
   GtkTreeModel *model;
   gchar *author;

   GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(userdata));

   if(gtk_tree_selection_get_selected(selection, &model, &iter)){
      if((GtkWidget*)userdata == CApp.CListView.getList()){
         gtk_tree_model_get(model, &iter, 1, &author, -1);
         unsigned long pData = strtoul(author, NULL, 0);
	 CApp.decrypt_filevF(pData, C_APP_HEX);
      }
      g_free(author);
   }
}
//////////////////////////////////////////////////////////////////////////////
// [ view_popup_menu_onNewFile ]
//////////////////////////////////////////////////////////////////////////////
void view_popup_menu_onNewFile(GtkWidget *menuitem, gpointer userdata){
   GtkTreeIter iter;
   GtkTreeModel *model;
   string TString;
   
   GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(userdata));

   if(gtk_tree_selection_get_selected(selection, &model, &iter))
      if((GtkWidget*)userdata == CApp.CTreeView.getTree())
         if(CApp.getPathByTree(&iter, &TString)) CApp.new_file(&TString);
}
//////////////////////////////////////////////////////////////////////////////
// [ view_popup_menu_onNewFolder ]
//////////////////////////////////////////////////////////////////////////////
void view_popup_menu_onNewFolder(GtkWidget *menuitem, gpointer userdata){
   GtkTreeIter iter;
   GtkTreeModel *model;
   string TString;
   
   GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(userdata));

   if(gtk_tree_selection_get_selected(selection, &model, &iter))
      if((GtkWidget*)userdata == CApp.CTreeView.getTree())
         if(CApp.getPathByTree(&iter, &TString)) CApp.new_folder(&TString);
}
//////////////////////////////////////////////////////////////////////////////
// [ view_popup_menu_onEditFile ]
//////////////////////////////////////////////////////////////////////////////
void view_popup_menu_onEditFile(GtkWidget *menuitem, gpointer userdata){
   GtkTreeIter iter;
   GtkTreeModel *model;
   gchar *author;

   GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(userdata));

   if(gtk_tree_selection_get_selected(selection, &model, &iter)){
      if((GtkWidget*)userdata == CApp.CListView.getList()){
         gtk_tree_model_get(model, &iter, 1, &author, -1);
         unsigned long pData = strtoul(author, NULL, 0);
	 if(CApp.edit_file(pData)){
	    g_signal_connect(G_OBJECT(CApp.SEditFile.pQuit), "activate", G_CALLBACK(sub_popup_menu_onQuit), CApp.SEditFile.pWindow);
            g_signal_connect(G_OBJECT(CApp.SEditFile.pSave), "activate", G_CALLBACK(sub_popup_menu_onSave), &CApp.SEditFile);
	 }
	 g_free(author);
      }
   }
}
//////////////////////////////////////////////////////////////////////////////
// [ tree_selection_changed_cb ]
//////////////////////////////////////////////////////////////////////////////
void tree_selection_changed_cb(GtkTreeSelection *selection, gpointer data){
   GtkTreeIter iter;
   GtkTreeModel *model;
   C_DArray CDA_Files;
   string TString;
   if(gtk_tree_selection_get_selected(selection, &model, &iter)){
      CApp.CListView.clear();
      if(CApp.getPathByTree(&iter, &TString)){
	 CApp.getFilesByPath((char*)TString.c_str(), &CDA_Files, 0);
	 for(int n = 0; n < CDA_Files._Get_nItems(); n++){
	    S_C_DArray* pTemp = CDA_Files._Get_pItem(n);
	    S_Crypt_VFile* pSCVFile = (S_Crypt_VFile*)pTemp->_pData->_Get_pBuffer();
	    // Add Name
	    GtkTreeIter TreeIter = CApp.CListView.addItem(pSCVFile->Name, 0, 0);
	    // Int to String
	    stringstream ss;
            ss << pSCVFile->pData;
            string str = ss.str();
            // Add Offset
	    CApp.CListView.addItem((char*)str.c_str(), 1, &TreeIter);
	 }
      }
   }
}
//////////////////////////////////////////////////////////////////////////////
// [ destroy ]
//////////////////////////////////////////////////////////////////////////////
void destroy(GtkWidget *widget, gpointer data){
   gtk_main_quit();
}
//////////////////////////////////////////////////////////////////////////////
// [ configure_callback ]
//////////////////////////////////////////////////////////////////////////////
void configure_callback(GtkWindow *window, GdkEvent *event, gpointer data){
   CApp.resize(event->configure.width, event->configure.height);
}
//////////////////////////////////////////////////////////////////////////////
// [ sub_popup_menu_onQuit ]
//////////////////////////////////////////////////////////////////////////////
void sub_popup_menu_onQuit(GtkWidget *menuitem, gpointer userdata){
   gtk_window_close((GtkWindow*)userdata);
}
//////////////////////////////////////////////////////////////////////////////
// [ sub_popup_menu_onSave ]
//////////////////////////////////////////////////////////////////////////////
void sub_popup_menu_onSave(GtkWidget *menuitem, gpointer userdata){
   GtkTextIter start_find, end_find;
   
   gtk_text_buffer_get_start_iter(((S_Edit_File*)userdata)->pTextBuffer, &start_find);
   gtk_text_buffer_get_end_iter(((S_Edit_File*)userdata)->pTextBuffer, &end_find);
   
   string NewData = gtk_text_buffer_get_text(((S_Edit_File*)userdata)->pTextBuffer, &start_find, &end_find, false);

   CApp.save_file(((S_Edit_File*)userdata)->pData, &NewData);

   gtk_window_close((GtkWindow*)((S_Edit_File*)userdata)->pWindow);
}