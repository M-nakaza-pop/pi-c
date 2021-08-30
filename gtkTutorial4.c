#include <gtk/gtk.h>

void button1_clicked (GtkWidget *widget, gpointer data){
    
    g_print("menu #1: You will get a kara-age bento!\n");
}
void button2_clicked (GtkWidget *widget, gpointer data){
    
    g_print("menu #2: You will get a maguro-don bento!\n");
}


int main (int argc, char *argv[]){

    GtkWidget *window, *vbox, *hbox, *label, *button1, *button2;
    gtk_init(&argc, &argv);

    
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);		//  �e�E�B���h�E�̐����i�{�E�����̎w��j
    g_signal_connect(window, "delete-event", G_CALLBACK(gtk_main_quit), NULL );
    
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);	//  �c�{�b�N�X�̐����i�{�e�E�B���h�E�ւ̔z�u�j
    gtk_container_add(GTK_CONTAINER(window), vbox);
    
    label = gtk_label_new("Menu");						//  ���x���̐����i�{���{�b�N�X�ւ̔z�u�j
    gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 0);
    
    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);	//  ���{�b�N�X�̐����i�{���{�b�N�X�ւ̔z�u�j
    gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 0);
    
    button1 = gtk_button_new_with_label("Chicken");		//  �{�^���̐����i�{�{�b�N�X�ւ̔z�u�j
    g_signal_connect(button1, "clicked", G_CALLBACK(button1_clicked), NULL );
    gtk_box_pack_start(GTK_BOX(hbox), button1, TRUE, TRUE, 0);
    
    button2 = gtk_button_new_with_label("Fish");
    g_signal_connect(button2, "clicked", G_CALLBACK(button2_clicked), NULL );
    gtk_box_pack_start(GTK_BOX(hbox), button2, TRUE, TRUE, 0);
    
    //  GUI �v�f�E�e�E�B���h�E�̕\��
    gtk_widget_show(button1);
    gtk_widget_show(button2);
    gtk_widget_show(hbox);
    gtk_widget_show(label);
    gtk_widget_show(vbox);
    gtk_widget_show(window);

    //  �C�x���g�������[�v
    gtk_main();
}