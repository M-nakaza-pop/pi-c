#include	<stdio.h>
#include	<string.h>

#include	<unistd.h>
#include	<stdlib.h>
#include	<wiringPi.h>		//GPIOを制御用pi@raspberrypi ~ $ sudo apt-get install libi2c-dev
					//要I2Cライブラリ pi@raspberrypi ~ $ sudo apt-get install libi2c-dev
					//要WiringPi本体　ビルド必要
					//git経由なのでgitの確認　pi@raspberrypi ~ $ git --version
					//なければ　pi@raspberrypi ~ $ sudo apt-get install git-core
					//ソース取得　pi@raspberrypi ~ $ git clone git://git.drogon.net/wiringPi
					//pi@raspberrypi ~ $ cd wiringPi
					//pi@raspberrypi ~/wiringPi $ ./build
					// Ver確認　pi@raspberrypi ~ $ gpio -v
#include	<wiringSerial.h>
#include	<termios.h>		//parity set

#include	<sys/ioctl.h>
#include	<time.h>		//nanosleep()

// gcc -Wall -o serial serial.c -lwiringPi
// sudo ./serial  --> run
//
// command ls /dev/tty*でttyPortが確認できる。
//
#define	GPIO18	18
/***************************************************************************
*
* Global variables and Functions
*
***************************************************************************/
int		fd;
int		i;

char	sbuff[10]={"test\0"};
char	s1buff[10]={0x16,0x54,0x0d,0x00};
char	s2buff[10]={0x16,0x55,0x0d,0x00};
/***************************************************************************
*
*
*
***************************************************************************/
/***************************************************************************
* Function Name:
* Description  :
* Arguments    : Code
* Return Value : None
****************************************************************************/
int	main(void){

	for(;;){
		//fd= serialOpen("/dev/ttyUSB0",19200);
		fd= serialOpen("/dev/ttyACM0",115200);
		if(fd< 0){
			puts("can not open serialport");
			delay(100);
		}else{
			puts("open serialport");
			break;
		}
	}

	if(wiringPiSetupGpio() < 0){
		printf("ERROR:WiringPi initialize");
	}
	pinMode(GPIO18,OUTPUT);
	digitalWrite(GPIO18, 0);


	while(1){
		puts("LOOP");
		serialPuts(fd,s1buff);
		for(i=0;i<10000;i++){
			delay(1);
			if(serialDataAvail(fd)){
				printf("recive : %C\n",serialGetchar(fd));
			}
		}
		serialPuts(fd,s2buff);
		delay(2000);
	}	
}


#if 0
		serRead();
		enqComm();
		stxComm();



		while(serialDataAvail(fd)){
			printf("recive : %C",serialGetchar(fd));
			fflush(stdout);	//
		}

		serialPuts(fd,"hello world\n");
		delay(1000);
	}

/***************************************************************************
* Function Name
* Description  :
* Arguments    : Code
* Return Value : None
****************************************************************************/
void	serRead(){

	char	rcv=0;

	if(serialDataAvail(fd)){
		rcv= serialGetchar(fd);

		if(rxflg==0x03) rxflg=0x21;		//!
		if(rxflg==0x05 && in==2) rxflg=0x3f;	//?

		if(rcv== 0x02 && rxflg==0){
			in= 0;
			rxflg= 0x02;
		}
		if(rcv== 0x03 && rxflg== 0x02) rxflg=0x03;
		if(rcv== 0x05 && rxflg== 0){
			in= 0;
			rxflg= 0x05;
		}

		data[in]= rcv;
		if(++in> 20) in= 0;

		//sbuff[0]=rcv;
		//serialPuts(fd,sbuff);
		//printf("rcv:%C",rcv);
		//fflush(stdout);
	}
}//for serRead
/***************************************************************************
* Function Name:
* Description  :
* Arguments    : Code
* Return Value : None
****************************************************************************/
void	enqComm(){
	
	char	al;
	//int		ax;

	if(rxflg=='?'){

		//printf("rxflg");	//test
		//fflush(stdout);		//test

		rxflg= 0;

		if(data[1]==0x30 || data[1]==0x31 /*&& data[2]==0x30*/){

			al=aschexConvert(data[1]);
			al = al << 4;
			al = al | aschexConvert(data[2]);
			
			digitalWrite(GPIO18, 1);
			delay(2);
			sbuff[0]= 0x04;
			sbuff[1]= 0x00;
			serialPuts(fd,sbuff);
			//test();
 			WaitTxDone(3);
			digitalWrite(GPIO18, 0);
			printf("rcv=%d\n",al); 
			
			//puts("?");
		}

	}

}//for enqComm
/***************************************************************************
* Function Name:
* Description  :
* Arguments    : Code
* Return Value : None
****************************************************************************/
void	stxComm(){


	if(rxflg== '!'){
		rxflg= 0;
		if(data[1]== 0x30 || data[1]==0x31 /*&& data[2]==0x30*/){
			if(data[3]== 'V'){
				digitalWrite(GPIO18, 1);
				delay(5);
				serialFlush(fd);
				sbuff[0]=0x02;
				sbuff[1]='F';
				sbuff[2]='F';
				sbuff[3]='x';
				sbuff[4]='0';
				sbuff[5]='0';
				sbuff[6]=0x03;
				sbuff[7]=0x7b;
				sbuff[8]=0x00;
				serialPuts(fd,sbuff);
				txFlush(8);
				delay(5);
				digitalWrite(GPIO18, 0);
				puts("V");
			}
			if(data[3]=='Y') return;
			if(data[3]=='G'){
				digitalWrite(GPIO18, 1);
				delay(5);
				sbuff[0]= 0x06;
				sbuff[1]= 0x00;
				serialPuts(fd,sbuff);
				delay(5);
				digitalWrite(GPIO18, 0);
				puts("G");
			}
		}

	}
}//for stxComm

/***************************************************************************
* Function Name:
* Description  :
* Arguments    : Code
* Return Value : None
****************************************************************************/
int	UART_putString(char* s){

	if(write(fd,s,strlen(s))!= 1) return -1;
	return 0;

}// for UART
/***************************************************************************
* Function Name:
* Description  :
* Arguments    : Code
* Return Value : None
****************************************************************************/
void	shutdown(void){
	system("shutdown -h now");
}

void	reboot(void){
	system("reboot");
}
/***************************************************************************
* Function Name:
* Description  :
* Arguments    : Code
* Return Value : None
****************************************************************************/
void	txFlush(unsigned int num){

	unsigned int	j;
	int		i=num;
	i=i-1;

	for(j= 0;j< 50;j++){

		delay(1);
		if(serialDataAvail(fd) > i) break;

	}
	serialFlush(fd);
	printf("txf=%d\n",j); 
}//for serialFlush 
/***************************************************************************
* Function Name:
* Description  :
* Arguments    : Code
* Return Value :None
****************************************************************************/
void	startFa(){

	char	rcv;

	while(1){

		if(serialDataAvail(fd)){
			rcv= serialGetchar(fd);
			if(rcv== 0xfa) return;
			if(rcv== 0x05){
				digitalWrite(GPIO18, 1);
				serialPuts(fd,sbuff);
				delay(30);
				digitalWrite(GPIO18, 0);
			}
		}
	}

}//for statFa
/***************************************************************************
* Function Name:
* Description  :
* Arguments    : Code
* Return Value : None
****************************************************************************/
char	aschexConvert(char	al){

	unsigned	char	i;
	unsigned	char	bl= -1;
	
	for(i=0; i<16; i++){
		
		if(aschextable[i].num == al){
			bl=aschextable[i].hex;
			break;
		}
	}
	return(bl);
}
/***************************************************************************
* Function Name:
* Description  :http://www.mech.tohoku-gakuin.ac.jp/rde/contents/linux/drivers/ioctl.html
* Arguments    : Code
* Return Value : None
****************************************************************************/
void	test(void){
	char	buff[100];
	//int		len;
	
	struct	oci_struct	ocis;
	
	ocis.p= buff;
	ioctl(fd,TIOCOUTQ,&ocis);
	printf("length=%d\n",ocis.len);
}
/***************************************************************************
* Function Name:https://stackoverflow.com/questions/46662953/socket-programming-with-ioctl-tiocoutq
* Description  :
* Arguments    : Code
* Return Value : None
****************************************************************************/
void WaitTxDone(unsigned int messageSize){
    
    int queued = 1;
    while (queued > 0){
        ioctl(fd, TIOCOUTQ, &queued); //wait for tty TX queue to become empty
	}
    //number of nanoseconds it takes to transmit messageSize bytes (10 is 8 bits + start + stop)
    //unsigned int txTime = ((float)(messageSize * 10) / m_baudrate) * 1000000000;
	unsigned int	txTime = ((float)(messageSize * 10) /19200) * 1000000000;
    //assuming it won't take longer than one second
    struct timespec delay = {0, txTime};
	nanosleep(&delay, NULL);

}
#endif
