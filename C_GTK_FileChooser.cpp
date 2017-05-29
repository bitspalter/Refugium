//////////////////////////////////////////////////////////////////////////////////
// [ GTK_FileChooser_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_GTK_FileChooser.hpp"

//////////////////////////////////////////////////////////////////////////////////
// [Konstructor]  
//////////////////////////////////////////////////////////////////////////////////
C_GTK_FileChooser::C_GTK_FileChooser(){

}
//////////////////////////////////////////////////////////////////////////////////
// [Destructor]
//////////////////////////////////////////////////////////////////////////////////
C_GTK_FileChooser::~C_GTK_FileChooser(){

}
//////////////////////////////////////////////////////////////////////////////////
// [getFolder]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_FileChooser::getFolder(const char* pCaption, GtkWindow* pParent, string* pFolder){
  
   if(!pCaption || !pParent || !pFolder) return(C_GTK_FILECHOOSER_ERROR);
  
   GtkWidget *pChooser = gtk_file_chooser_dialog_new(pCaption, pParent, 
                                                     GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, 
                                                     GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, 
                                                     GTK_STOCK_OPEN, 
                                                     GTK_RESPONSE_OK, 
                                                     NULL);

   gint result = gtk_dialog_run(GTK_DIALOG(pChooser));
   
   switch(result){
      case GTK_RESPONSE_OK:
         *pFolder = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(pChooser));
         gtk_widget_destroy(pChooser);
         break;
      default:
         gtk_widget_destroy(pChooser);
         return(C_GTK_FILECHOOSER_ERROR);
   }

   return(C_GTK_FILECHOOSER_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [getFile]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_FileChooser::getFile(const char* pCaption, GtkWindow* pParent, string* pFile){
  
   if(!pCaption || !pParent || !pFile) return(C_GTK_FILECHOOSER_ERROR);
   
   GtkWidget* pChooser = gtk_file_chooser_dialog_new(pCaption, pParent, 
                                                     GTK_FILE_CHOOSER_ACTION_OPEN, 
                                                     GTK_STOCK_CANCEL, 
                                                     GTK_RESPONSE_CANCEL, 
                                                     GTK_STOCK_OPEN, 
                                                     GTK_RESPONSE_OK, 
                                                     NULL);
  
   gint result = gtk_dialog_run(GTK_DIALOG(pChooser));
  
   switch(result){
      case GTK_RESPONSE_OK:
         *pFile = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(pChooser));
         gtk_widget_destroy(pChooser);
         break;
      default:
         gtk_widget_destroy(pChooser);
         return(C_GTK_FILECHOOSER_ERROR);
   }
   
   return(C_GTK_FILECHOOSER_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [setFile]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_FileChooser::setFile(const char* pCaption, GtkWindow* pParent, string* pFile){
  
   if(!pCaption || !pParent || !pFile) return(C_GTK_FILECHOOSER_ERROR);
   
   GtkWidget * pChooser = gtk_file_chooser_dialog_new(pCaption, pParent, 
                                                      GTK_FILE_CHOOSER_ACTION_SAVE, 
                                                      GTK_STOCK_CANCEL, 
                                                      GTK_RESPONSE_CANCEL, 
                                                      GTK_STOCK_OPEN, 
                                                      GTK_RESPONSE_OK, 
                                                      NULL);
   
   gint result = gtk_dialog_run(GTK_DIALOG(pChooser));
   
   switch(result){
      case GTK_RESPONSE_OK:
         *pFile = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(pChooser));
         gtk_widget_destroy(pChooser);
         break;
      default:
         gtk_widget_destroy(pChooser);
         return(C_GTK_FILECHOOSER_ERROR);
   }
   
   return(C_GTK_FILECHOOSER_READY);
}