/*********************************************************************************
*		https://reerishun.com/makerblog/?p=605
*		serverHost.c
*********************************************************************************/
x#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <unistd.h> /* for close */
#include <arpa/inet.h>
 
#define PORT_TEMP 9001
 
 
//転送用構造体
typedef struct {
    float prm_pc_to_soc[20];        //PC→SoCへの転送パラメータ
    float prm_soc_to_pc[20];        //SoC→PChへの転送パラメータ
} Transfer_data;

char        buf[1024];


int main(){

    int sock0;
    struct sockaddr_in addr;
    struct sockaddr_in client;
    int len;
    int sock;
    int yes = 1;
    int fd;
    struct ifreq ifr;
 
    fd = socket(AF_INET, SOCK_DGRAM, 0);
 
    ioctl(fd, SIOCGIFADDR, &ifr);
    close(fd);
    
    //パラメータ転送構造体
    Transfer_data td_recv;
    Transfer_data td_send;
 
    //転送テスト用データ
    float send0 = 30;
    float send1 = 444;
 
    //ソケット作成
    sock0 = socket(AF_INET, SOCK_STREAM, 0);
 
    //ソケット作成エラーチェック
    if(sock0 < 0) {
        perror("socket");
        return -1;
    }   
 
    //接続先情報設定
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT_TEMP);
    addr.sin_addr.s_addr = INADDR_ANY;
 
    //ソケットのオプション設定
    setsockopt(sock0, SOL_SOCKET, SO_REUSEADDR, (const char *)&yes,  sizeof(yes));
 
    //アドレスとソケットディスクリプタを関連付け
    if (bind(sock0, (struct sockaddr *)&addr, sizeof(addr)) != 0) {
        perror("bind");
        return -1;
    }
 
    //接続待ちに設定（acceptにて接続要求を受ける）
    if(listen(sock0, 5) != 0) {
        perror("listen");
        return -1;
    }
    printf("IP addr:%s\n", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
	puts("waiting connect client...");
 
    //接続要求待ちループ
    while (1) {
        len = sizeof(client);
        sock = accept(sock0, (struct sockaddr *)&client, &len);
 
        //接続エラー判定
        if (sock < 0) {
            //エラー時はループを抜けて処理を終了
            perror("accept");
            break;
        }
 
        //データ受信
		// 送られてきたメッセージ(COMMAND)を受け取ります
		memset(buf, 0, 1024);
		recv(sock, buf, 1024, 0);
		if (buf[0] == '\0')
			strcpy(buf, "NULL");
		buf[0]='t';
		buf[1]='e';
		buf[2]='s';
		buf[3]='t';
		buf[4]='\0';
		puts(buf);
    }
 
    //ソケットクローズ
    close(sock0);
 
    printf( "Close socket...\n" );
 
    return 0;
}
