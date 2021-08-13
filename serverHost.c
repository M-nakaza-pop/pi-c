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
 
 
//�]���p�\����
typedef struct {
    float prm_pc_to_soc[20];        //PC��SoC�ւ̓]���p�����[�^
    float prm_soc_to_pc[20];        //SoC��PCh�ւ̓]���p�����[�^
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
    
    //�p�����[�^�]���\����
    Transfer_data td_recv;
    Transfer_data td_send;
 
    //�]���e�X�g�p�f�[�^
    float send0 = 30;
    float send1 = 444;
 
    //�\�P�b�g�쐬
    sock0 = socket(AF_INET, SOCK_STREAM, 0);
 
    //�\�P�b�g�쐬�G���[�`�F�b�N
    if(sock0 < 0) {
        perror("socket");
        return -1;
    }   
 
    //�ڑ�����ݒ�
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT_TEMP);
    addr.sin_addr.s_addr = INADDR_ANY;
 
    //�\�P�b�g�̃I�v�V�����ݒ�
    setsockopt(sock0, SOL_SOCKET, SO_REUSEADDR, (const char *)&yes,  sizeof(yes));
 
    //�A�h���X�ƃ\�P�b�g�f�B�X�N���v�^���֘A�t��
    if (bind(sock0, (struct sockaddr *)&addr, sizeof(addr)) != 0) {
        perror("bind");
        return -1;
    }
 
    //�ڑ��҂��ɐݒ�iaccept�ɂĐڑ��v�����󂯂�j
    if(listen(sock0, 5) != 0) {
        perror("listen");
        return -1;
    }
    printf("IP addr:%s\n", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
	puts("waiting connect client...");
 
    //�ڑ��v���҂����[�v
    while (1) {
        len = sizeof(client);
        sock = accept(sock0, (struct sockaddr *)&client, &len);
 
        //�ڑ��G���[����
        if (sock < 0) {
            //�G���[���̓��[�v�𔲂��ď������I��
            perror("accept");
            break;
        }
 
        //�f�[�^��M
		// �����Ă������b�Z�[�W(COMMAND)���󂯎��܂�
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
 
    //�\�P�b�g�N���[�Y
    close(sock0);
 
    printf( "Close socket...\n" );
 
    return 0;
}
