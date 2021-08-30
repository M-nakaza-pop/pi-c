#include <gtk/gtk.h>

void button_clicked (GtkWidget *widget, gpointer data){

    g_print("The button was clicked!\n");
}

int main (int argc, char *argv[]){

    
    GtkWidget *window, *box, *label, *button;		//  親ウィンドウと GUI 要素（のポインタ変数）
    gtk_init(&argc, &argv);

    
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);	//  親ウィンドウの生成（＋殺し方の指定）
    g_signal_connect(window, "delete-event", G_CALLBACK(gtk_main_quit), NULL );
    
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);	//  縦ボックスの生成（＋親ウィンドウへの配置）
    gtk_container_add(GTK_CONTAINER(window), box);
    
    label = gtk_label_new("Hello, world!");			//  ラベルの生成（＋ボックスへの配置）
    gtk_box_pack_start(GTK_BOX(box), label, TRUE, TRUE, 0);
    
    button = gtk_button_new_with_label("Push me!");	//  ボタンの生成（＋ボックスへの配置）
    g_signal_connect(button, "clicked", G_CALLBACK(button_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(box), button, TRUE, TRUE, 0);
    
    
    gtk_widget_show(label);		//  GUI 要素・親ウィンドウの表示
    gtk_widget_show(button);
    gtk_widget_show(box);
    gtk_widget_show(window);

    gtk_main();
    
}gtkTutorial3
