//*********************************************************************:
//	gtk学習
//	buttonをclickするとbutton_did_clicked（）を呼ぶ
//  引数に&n(nのアドレス）
//	表示後、加算
//
//	gcc -Wall -o "gtkTutorial1" "gtkTutorial1.c" -lwiringPi `pkg-config --libs --cflags gtk+-3.0`（ディレクトリ: /home/pi/WorkSpace）
//  
//**********************************************************************

#include	<gtk/gtk.h>

void	button_did_clicked(GtkWidget* widget, gpointer data){
			printf("%d\n", *((int *)data));
			*((int *)data)+=1;
}

int	main(int argc, char *arcv[]){
		GtkWidget *window;
		GtkWidget *button1;	
		int	n=0;
		gtk_init(&argc, &arcv);		//gtkの初期化
		
		window= gtk_window_new(GTK_WINDOW_TOPLEVEL);//親ウインド生成
		button1= gtk_button_new_with_label("AUQLUE");
		
		gtk_widget_set_size_request(window, 220, 30);
		gtk_container_add(GTK_CONTAINER(window), button1);
		
		g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit),NULL);
		g_signal_connect(button1, "clicked", G_CALLBACK(button_did_clicked), &n);
		

		gtk_widget_show_all(window);			//表示
		
		gtk_main();
		
		return 0;
}
