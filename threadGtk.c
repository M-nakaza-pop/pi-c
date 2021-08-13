/*********************************************************************************
*
*		threadGtk.c
*
*		gcc -W -Wall -o threadGtk threadGtk.c -lwiringPi -pthread `pkg-config --libs --cflags gtk+-3.0`
*		sudo ./threadGtk  --> run
*********************************************************************************/

#include <stdio.h>		//printf(),fprint(),perror()
#include <sys/types.h>		// usleep()
#include <sys/socket.h>		//soket(),bind(),accept(),listen()
#include <netinet/in.h>
#include <unistd.h>		// close(),
#include <sys/time.h>		//
#include <string.h>		// memcpy()
#include <arpa/inet.h>		// inet_ntoa()
#include <sys/ioctl.h>		// ioctl()
#include <wiringPi.h>		//
#include <pthread.h>

#include <gtk/gtk.h>
/***************************************************************************
*
* Global varibles and functions
*
***************************************************************************/
void    recvMessage(int sock);
void	*threadFunc(void *param);
void	*timeStamp(void *ptr);

extern	void *gtktest2(void *ptr);
char	button1,button2,button3;


int	system(const char *command);

pthread_mutex_t	mutex;

int	ycomm = 0;
int	tcomm = 0;

int	main(void){

	pthread_t	thread1, thread2;
	//useconds_t	time	= 5000000;


	pthread_create(&thread1, NULL, threadFunc, NULL);
	//pthread_create(&thread2, NULL, timeStamp, (void *)&time); /*ok*/
	pthread_create(&thread2,NULL,  gtktest2, NULL);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	return 0;

}//for main

/***********************************************************************
* Function Name:
* Description  :
* Arguments    : Code
* Return Value : None
************************************************************************/
void	*threadFunc(void *param){

	int 	sock0;
	struct 	sockaddr_in addr;
	struct 	sockaddr_in client;
	//int 	len;
	socklen_t	len;
	int 	sock;

	fd_set	fds,readfds;
	struct	timeval tv;
	int	retval;



 	sock0 = socket(AF_INET, SOCK_STREAM, 0);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(9001);
	addr.sin_addr.s_addr = INADDR_ANY;


	int	nonblock = 1;
	ioctl(sock0, FIONBIO, &nonblock);

	//fcntl(sock0, F_SETFL, O_NONBLOCK);

	bind(sock0, (struct sockaddr *)&addr, sizeof(addr));//0=sccess -1=err
	FD_ZERO(&readfds);		//&rfds CLR
	FD_SET(sock0,&readfds);		//sock0

	tv.tv_sec =	1;
	tv.tv_usec=	0;



	listen(sock0, 5);		//0=sccess -1=err
	puts("waiting connect client");

	while (1) {
		memcpy(&fds, &readfds, sizeof(fd_set));
		tv.tv_sec = 5;
		tv.tv_usec= 0;
		retval = select(sock0+1,&fds,NULL,NULL,&tv);
		//printf("retval=%d\n",retval);
		if(retval==0){
			puts("timeout");
			//break;
		}
		else if(FD_ISSET(sock0, &fds)){

			len = sizeof(client);
			sock = accept(sock0, (struct sockaddr *)&client, &len);
			printf("%d\n",sock);
			printf("accepted connection from %s, port=%d\n",
			inet_ntoa(client.sin_addr), ntohs(client.sin_port));
		/*else if(retval){*/
			recvMessage(sock);
			//send(sock, "HELLO", 5, 0);
			//close(sock);
		}
		//puts("loop");

		//close(sock);
	}
}// for threadFunc
/***********************************************************************
* Function Name:
* Description  :
* Arguments    : Code
* Return Value : None
************************************************************************/
void	blockRecv(int sock){

	char	buff[32];
	int 	ret;

	for(;;){
		memset(buff,0,sizeof(buff));
		ret= recv(sock, buff, sizeof(buff), 0);
		printf("ret=%d\n",ret);
		if(ret==0 || ret==-1){
			close(sock);
			break;
		}
		printf("%s\n",buff);
	}
}
/***********************************************************************
* Function Name:
* Description  :
* Arguments    : Code
* Return Value : None
************************************************************************/
void    recvMessage(int sock){

	char	buff[32];
	int	ret;

	for(;;){

		memset(buff,0,sizeof(buff));
		ret= recv(sock, buff, sizeof(buff), MSG_DONTWAIT);

		if(ret==-1){
			if(ycomm==1 && tcomm==1){
			puts("thread");
			pthread_mutex_lock(&mutex);
			tcomm=0;
			pthread_mutex_unlock(&mutex);
			}
			else{
			puts("wait");
			}
			delay(500);		//msec

		}else if(ret==0){
			puts("line off");
			close(sock);
			break;

		}else{
			printf("recv=%s\n",buff);

			ycomm = 1;
		}
	}
}
/***********************************************************************
* Function Name:
* Description  :
* Arguments    : Code
* Return Value : None
************************************************************************/
void	*timeStamp(void *ptr){

	useconds_t tick = *(int *)ptr;

	while(1){
		// printf("time\n"); ok
		// system("aplay /home/pi/voice1.wav"); OK
		if(ycomm== 1){
		pthread_mutex_lock(&mutex);
			tcomm= 1;
		pthread_mutex_unlock(&mutex);
		}
		usleep(tick);
	}
}
