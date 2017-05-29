//////////////////////////////////////////////////////////////////////////////////
// [ GTK_Text_View_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_GTK_TextView.hpp"

//////////////////////////////////////////////////////////////////////////////////
// [Konstructor]  
//////////////////////////////////////////////////////////////////////////////////
C_GTK_TextView::C_GTK_TextView(){
   this->ptextview   = 0;
   this->ptextbuffer = 0;
   this->status      = C_GTK_TEXTVIEW_ERROR;
}
//////////////////////////////////////////////////////////////////////////////////
// [Destructor]
//////////////////////////////////////////////////////////////////////////////////
C_GTK_TextView::~C_GTK_TextView(){

}
//////////////////////////////////////////////////////////////////////////////////
// [create]
//////////////////////////////////////////////////////////////////////////////////
GtkWidget* C_GTK_TextView::create(){
  
   if(this->status == C_GTK_TEXTVIEW_READY) return(this->ptextview);
   
   this->ptextbuffer = gtk_text_buffer_new(NULL);
   gtk_text_buffer_set_text(this->ptextbuffer, "", -1);
    
   this->ptextview = gtk_text_view_new_with_buffer(this->ptextbuffer);

   if(!this->ptextview) return(C_GTK_TEXTVIEW_ERROR);
  
   gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(this->ptextview), GTK_WRAP_WORD); 

   this->status = C_GTK_TEXTVIEW_READY;

   return(this->ptextview);
}
//////////////////////////////////////////////////////////////////////////////////
// [getTextView]
//////////////////////////////////////////////////////////////////////////////////
GtkWidget* C_GTK_TextView::getTextView(){
  
   if(this->status != C_GTK_TEXTVIEW_READY) return(C_GTK_TEXTVIEW_ERROR);
   
   return(this->ptextview);
}
//////////////////////////////////////////////////////////////////////////////////
// [getTextBuffer]
//////////////////////////////////////////////////////////////////////////////////
GtkTextBuffer* C_GTK_TextView::getTextBuffer(){
  
   if(this->status != C_GTK_TEXTVIEW_READY) return(C_GTK_TEXTVIEW_ERROR);
   
   return(this->ptextbuffer);
}
//////////////////////////////////////////////////////////////////////////////////
// [setTextBuffer]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_TextView::setTextBuffer(GtkTextBuffer* pTextBuffer){
  
   if(this->status != C_GTK_TEXTVIEW_READY || !pTextBuffer) return(C_GTK_TEXTVIEW_ERROR);
  
   gtk_text_view_set_buffer(GTK_TEXT_VIEW(this->ptextview), pTextBuffer);
   
   return(C_GTK_TEXTVIEW_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [getText]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_TextView::getText(string* pOut){
  
   if(this->status != C_GTK_TEXTVIEW_READY) return(C_GTK_TEXTVIEW_ERROR);
   
   if(!pOut) return(C_GTK_TEXTVIEW_ERROR);
   
   GtkTextIter start_find, end_find;
   
   gtk_text_buffer_get_start_iter(this->ptextbuffer, &start_find);
   gtk_text_buffer_get_end_iter(this->ptextbuffer, &end_find);
   
   *pOut = gtk_text_buffer_get_text(this->ptextbuffer, &start_find, &end_find, false);
   
   return(C_GTK_TEXTVIEW_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [setText]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_TextView::setText(string* pIn){
  
   if(this->status != C_GTK_TEXTVIEW_READY) return(C_GTK_TEXTVIEW_ERROR);
   
   if(!pIn) return(C_GTK_TEXTVIEW_ERROR);
   
   if(!g_utf8_validate(pIn->c_str(), pIn->length(), NULL)) return(C_GTK_TEXTVIEW_ERROR);
   
   gtk_text_buffer_set_text(this->ptextbuffer, pIn->c_str(), pIn->length());

   return(C_GTK_TEXTVIEW_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [clear]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_TextView::clear(){
  
   GtkTextIter start_find, end_find;
   
   gtk_text_buffer_get_start_iter(this->ptextbuffer, &start_find);
   gtk_text_buffer_get_end_iter(this->ptextbuffer, &end_find);
   
   gtk_text_buffer_delete(this->ptextbuffer, &start_find, &end_find);
   
   return(C_GTK_TEXTVIEW_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [setFont]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_TextView::setFont(const char* pFont){
  
   if(this->status != C_GTK_TEXTVIEW_READY || !pFont) return(C_GTK_TEXTVIEW_ERROR);
   
   PangoFontDescription *font_desc;
   
   font_desc = pango_font_description_from_string(pFont);
   gtk_widget_modify_font(this->ptextview, font_desc);
   pango_font_description_free(font_desc);

   return(C_GTK_TEXTVIEW_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [setBackground]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_TextView::setBackground(const GdkRGBA* pColor){
  
   if(this->status != C_GTK_TEXTVIEW_READY || !pColor) return(C_GTK_TEXTVIEW_ERROR);

   gtk_widget_override_background_color(this->ptextview, GTK_STATE_FLAG_NORMAL, pColor);

   return(C_GTK_TEXTVIEW_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [setColor]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_TextView::setColor(const GdkRGBA* pColor){
  
   if(this->status != C_GTK_TEXTVIEW_READY || !pColor) return(C_GTK_TEXTVIEW_ERROR);
   
   gtk_widget_override_color(this->ptextview, GTK_STATE_FLAG_NORMAL, pColor);

   return(C_GTK_TEXTVIEW_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [set_size]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_TextView::setSize(int x, int y){
  
   if(this->status != C_GTK_TEXTVIEW_READY) return(C_GTK_TEXTVIEW_ERROR);
   
   gtk_widget_set_size_request(this->ptextview, x, y);
   
   return(C_GTK_TEXTVIEW_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [hide]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_TextView::hide(){
  
   if(this->status != C_GTK_TEXTVIEW_READY) return(C_GTK_TEXTVIEW_ERROR);
   
   gtk_widget_hide(this->ptextview);
   
   return(C_GTK_TEXTVIEW_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [show]
//////////////////////////////////////////////////////////////////////////////////
int C_GTK_TextView::show(){
  
   if(this->status != C_GTK_TEXTVIEW_READY) return(C_GTK_TEXTVIEW_ERROR);
   
   gtk_widget_show(this->ptextview);
   
   return(C_GTK_TEXTVIEW_READY);
}