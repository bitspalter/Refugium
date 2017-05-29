//////////////////////////////////////////////////////////////////////////////////
// [ GTK_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_GTK.hpp"

//////////////////////////////////////////////////////////////////////////////////
// [Konstructor]  
//////////////////////////////////////////////////////////////////////////////////
C_GTK::C_GTK(){
   this->status = C_GTK_ERROR;
}
//////////////////////////////////////////////////////////////////////////////////
// [Destructor]
//////////////////////////////////////////////////////////////////////////////////
C_GTK::~C_GTK(){

}
//////////////////////////////////////////////////////////////////////////////////
// [init]  
//////////////////////////////////////////////////////////////////////////////////
int C_GTK::init(int *argc, char ***argv){
   gtk_init(argc, argv);
}
//////////////////////////////////////////////////////////////////////////////////
// [create]  
//////////////////////////////////////////////////////////////////////////////////
int C_GTK::resize(int x, int y){
  
  if(y > 150)
     gtk_widget_set_size_request(scrolled_window1, x, y - 150);
   
   if(x > 243){
      gtk_widget_set_size_request(scrolled_window3, (x - 242) / 2, 130);
      gtk_widget_set_size_request(scrolled_window2, (x - 242) / 2, 130);
      gtk_layout_move(GTK_LAYOUT(layout), scrolled_window2, 240 + (x - 240) / 2, 22);
   }
}
//////////////////////////////////////////////////////////////////////////////////
// [create]  
//////////////////////////////////////////////////////////////////////////////////
int C_GTK::create(){
   const GdkRGBA bianco = { 0.5, 0.5, 0.5, 1 };
   const GdkRGBA putzi  = { 0.7, 0.7, 0.7, 1 };
   const GdkRGBA nero   = { 0, 0, 0, 1 };
  
   ////////////////
   // Window
   GtkWidget* window = CWindow.create();

   CWindow.setSize(238, 150);
   CWindow.setCaption("R3FUG1UM");
   //CWindow.setBackground(&nero);
   //CWindow.max();
   //gtk_widget_set_size_request(window, 238, 150);
   //gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

   ////////////////
   // Button
   GtkWidget* button0 = sbutton.openfile.create("open file");
   GtkWidget* button  = sbutton.openfolder.create("open folder");
   GtkWidget* button1 = sbutton.encrypt.create("encrypt");
   GtkWidget* button2 = sbutton.decrypt.create("decrypt");
   GtkWidget* button3 = sbutton.decryptv.create("decrypt virtual");

   sbutton.openfile.setSize(118, 20);
   sbutton.openfolder.setSize(118, 20);
   sbutton.encrypt.setSize(118, 20);
   sbutton.decrypt.setSize(118, 20);
   sbutton.decryptv.setSize(234, 20);
   
   ////////////////
   // Edit
   GtkWidget* edit1 = sedit.pass1.create(12);
   GtkWidget* edit2 = sedit.pass2.create(12);
   
   sedit.pass1.setPass("*");
   sedit.pass2.setPass("*");
   
   ////////////////
   // LABEL
   GtkWidget* label1 = slabel.message.create("Welcome");

   //gtk_misc_set_alignment(GTK_MISC(slabel.message.getLabel()), 0.5f, 0.5f);
   
   slabel.message.setAlignment(0.5f, 0.5f);
   
   //slabel.message.setBackground(&nero);
   //slabel.message.setColor(&bianco);
   slabel.message.setSize(234, 30);
   slabel.message.setFont("Courier New 12");
   
   ////////////////
   
   GtkWidget* label2 = slabel.path.create("");

   slabel.path.setAlignment(0.0f, 0.5f);
   //slabel.path.setBackground(&nero);
   //slabel.path.setColor(&bianco);
   slabel.path.setSize(120, 30);
   slabel.path.setFont("Courier New 12");
   
   ////////////////
   // Text View
   GtkWidget* text1 = CTextView.create();
   
   //CTextView.setSize(300, 300);
   //CTextView.setBackground(&nero);
   //CTextView.setColor(&putzi);
   CTextView.setFont("Courier New 12");
   
   GtkTextTag* pTag1 = gtk_text_buffer_create_tag(CTextView.getTextBuffer(), "black_bg", "background",  "black", NULL);
   GtkTextTag* pTag2 = gtk_text_buffer_create_tag(CTextView.getTextBuffer(), "green_fg", "foreground",  "green", NULL);
   GtkTextTag* pTag3 = gtk_text_buffer_create_tag(CTextView.getTextBuffer(), "white_bg", "background",  "white", NULL);
   GtkTextTag* pTag4 = gtk_text_buffer_create_tag(CTextView.getTextBuffer(), "black_fg", "foreground",  "black", NULL);
   

   
   
   //gtk_text_tag_table_remove(gtk_text_buffer_get_tag_table(pTBuffer), pTag1);
   //gtk_text_tag_table_remove(gtk_text_buffer_get_tag_table(pTBuffer), pTag2);
   
   
   scrolled_window1 = gtk_scrolled_window_new(NULL, NULL);
   gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window1), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC); 
   
   gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(scrolled_window1), 850);
   
   //gtk_widget_set_size_request(scrolled_window1, 300, 300);
   //gtk_widget_set_size_request(scrolled_window1, 800, 200);
   
   gtk_container_add(GTK_CONTAINER(scrolled_window1), text1);
   //gtk_container_set_border_width(GTK_CONTAINER(scrolled_window), 5);
  
   ////////////////
   // Tree View
   GtkWidget* tree1 = CTreeView.create();
   
   //CTreeView.setBackground(&nero);
   //CTreeView.setColor(&bianco);
   //CTreeView.setFont("Courier New 10");
   
   //gtk_widget_set_size_request(tree1, 240, 200);
   
   CTreeView.addColumn(0, "Folder");

   GtkTreeStore* pTStore = CTreeView.createTStore();
   
   /*
   GtkTreeIter TreeIter;
   
   TreeIter = CTreeView.addItem("Test0", pTStore, 0);
   TreeIter = CTreeView.addItem("Test1", pTStore, &TreeIter);
   CTreeView.addItem("Test2", pTStore, &TreeIter);
   CTreeView.addItem("Test3", pTStore, 0);
   CTreeView.addItem("Test4", pTStore, 0);
   CTreeView.addItem("Test5", pTStore, 0);
   CTreeView.addItem("Test6", pTStore, 0);
   CTreeView.addItem("Test7", pTStore, 0);
   CTreeView.addItem("Test8", pTStore, 0);
   CTreeView.addItem("Test9", pTStore, 0);
   */
   CTreeView.addTStore(pTStore);

   ////////////////
   
   scrolled_window3 = gtk_scrolled_window_new(NULL, NULL);
   gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window3), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC); 
   //gtk_widget_set_size_request(scrolled_window3, 240, 200);
   gtk_container_add(GTK_CONTAINER(scrolled_window3), tree1);
   
   ////////////////
   // List View
   GtkWidget* list1 = CListView.create();
   
   //CListView.setBackground(&nero);
   //CListView.setColor(&bianco);
   //CListView.setFont("Courier New 10");
   
   //gtk_widget_set_size_request(list1, 240, 200);
   
   CListView.addColumn(0, "File");
   CListView.addColumn(1, "Offset");
   
   GType Types[2];
   
   Types[0] = G_TYPE_STRING;
   Types[1] = G_TYPE_STRING;
   
   GtkListStore* pLStore = CListView.createLStore(2, Types);
   
   /*
   TreeIter = CListView.addItem("Anton", 0, pLStore, 0);
   CListView.addItem("30",    1, pLStore, &TreeIter);
   
   TreeIter = CListView.addItem("Tom",   0, pLStore, 0);
   CListView.addItem("10",    1, pLStore, &TreeIter);
   
   TreeIter = CListView.addItem("Tim",   0, pLStore, 0);
   CListView.addItem("20",    1, pLStore, &TreeIter);
   */
   CListView.addLStore(pLStore);

   ////////////////
   
   scrolled_window2 = gtk_scrolled_window_new(NULL, NULL);
   gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window2), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC); 
   //gtk_widget_set_size_request(scrolled_window2, 240, 200);
   gtk_container_add(GTK_CONTAINER(scrolled_window2), list1);
   
   
   ////////////////
   // Spinner
   
   spinner = gtk_spinner_new();
   
   ////////////////
   // Layout
   layout = gtk_layout_new(NULL, NULL);
   
   gtk_layout_put(GTK_LAYOUT(layout), button0,    0,   0);
   gtk_layout_put(GTK_LAYOUT(layout), button,   120,   0);
   gtk_layout_put(GTK_LAYOUT(layout), button1,    0,  60);
   gtk_layout_put(GTK_LAYOUT(layout), button2,  120,  60);
   gtk_layout_put(GTK_LAYOUT(layout), button3,    2,  90);
   
   gtk_layout_put(GTK_LAYOUT(layout), edit1,      2,  30);
   gtk_layout_put(GTK_LAYOUT(layout), edit2,    122,  30);
   
   gtk_layout_put(GTK_LAYOUT(layout), label1,     0, 120);
   gtk_layout_put(GTK_LAYOUT(layout), label2,   240,   0);
   
   gtk_layout_put(GTK_LAYOUT(layout), spinner,  5, 127);
   
   gtk_layout_put(GTK_LAYOUT(layout), scrolled_window1,   0, 152);
   gtk_layout_put(GTK_LAYOUT(layout), scrolled_window2, 340,  22);
   gtk_layout_put(GTK_LAYOUT(layout), scrolled_window3, 240,  22);

   gtk_container_add(GTK_CONTAINER(window), layout);
}
//////////////////////////////////////////////////////////////////////////////////
// [main]  
//////////////////////////////////////////////////////////////////////////////////
int C_GTK::main(){
   gtk_widget_show_all(CWindow.getWindow());
   CTreeView.hide();
   CListView.hide();
   gtk_main();
}