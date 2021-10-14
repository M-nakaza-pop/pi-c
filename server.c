/*  
 * 	TCP Server
 * 	Kindle コピペでいけるRaspberryPI 
 * 
 * 
 */ 

  
#include <stdio.h>   
#include <stdlib.h>   
#include <string.h>
#include <stdlib.h>   
#include <string.h>   
#include <ctype.h>   
#include <errno.h>   
#include <sys/types.h>   
#include <sys/ioctl.h>   
#include <sys/socket.h>   
#include <netinet/in.h>   
#include <netdb.h>   
#include <sys/stat.h>   
#include <fcntl.h>   
#include <unistd.h>   
#include <malloc.h>
　
int     writer_len;     			// Data Write Length   
struct sockaddr_in reader_addr; 	// read socket   
// struct sockaddr_in writer_addr; 	// write socket   
struct sockaddr writer_addr; 		// write socket   
int     readedbytes;   
int     readbytes;
char    *p;
   
#define CNT_BYTES       (4)   
#define TESTPORT        (8001)
   



/*	Read specified bytes from client	*/   
void ReadBytes(int fd,char *buff,int bytes){
	
	int readbytes;
	int readedbytes;
	char *p;

	p = buff;
	readbytes = bytes;
	readedbytes = 0;
	bzero(buff,bytes);
	
	for(;;){
		if(0 > (readedbytes = read(fd,p,readbytes))){
			printf("ReadError in Line %d.\n",__LINE__);
		}
		if(bytes > readbytes){
			p = p + readedbytes;
			readbytes = readbytes - readedbytes;
		}else break;
	}
}



/*	Server Ready   */
int	ServerInit(int port){    							// return Socket
	
	int	yes = 1;
	int usv;
	int sockfd;
	
	if(0 > (sockfd = socket(PF_INET, SOCK_STREAM,0))){ 	// Make Socket
		perror("SERV: reader: socket");
		return -1;
	}
	bzero((char *)(&reader_addr),sizeof(reader_addr));  // clear struct for read   
	reader_addr.sin_family = PF_INET;                   // set value to struct
	reader_addr.sin_addr.s_addr = htonl(INADDR_ANY);    // set value to struct
	reader_addr.sin_port = htons(port);                 // set value to struct
	setsockopt(sockfd, SOL_SOCKET,SO_REUSEADDR,(const char *)&yes,sizeof(yes)); // Set Socket options   
	if(0 > bind(sockfd,(struct sockaddr *)(&reader_addr),sizeof(reader_addr))){ // set bind
		perror("SERV: reader: bind");
		return -1;
	}   
	if(0 != listen(sockfd,50)){      					// wait the connection
		perror("SERV: reader: listen");
		close(sockfd);
		return -1;
	}   
	return sockfd;
}   

int WaitIncomming(int waitsock){
	int new_sockfd;
	writer_len = sizeof(writer_addr);
	if(0 > (new_sockfd = accept(waitsock,(struct sockaddr *)(&(writer_addr)),(socklen_t *)(&(writer_len))))){
		perror("SERV: reader: accept");
		return(-1);
	}   
	return new_sockfd;
}   

void ServerReadDatas(char *DataBuffer,int fda){
	int cnt;
	int len;
	ReadBytes(fda,DataBuffer,CNT_BYTES);
	DataBuffer[CNT_BYTES]=(char)0;
}






/*   Server Sample Program   */   
main(){ 
	  
	char    Buffer[1024];   
	
	for(;;){           
		int	socky,sockx;           
		if(0 > (sockx = ServerInit(TESTPORT))){                   
			printf("SERV: Init fail\n");                   
			exit(1);                   
		}           
		if(0 > (socky = WaitIncomming(sockx))){                   
			printf("SERV: Wait Fail.\n");                   
			exit(1);                   
		}           
		ServerReadDatas(Buffer,socky);           
		printf("SERV: :DATA=[%s]\n",Buffer);           
		close(sockx);           
		close(socky);           
	}  
}
