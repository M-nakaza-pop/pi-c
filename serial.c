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



// gcc -Wall -o serial serial.c -lwiringPi
// sudo ./serial  --> run
//
#define	GPIO18	18
/***************************************************************************
*
* Global variables and Functions
*
***************************************************************************/
char 		data[20];
unsigned short	in= 0;
char		sbuff[20]={	0xfa,0xfa,0xfa,0xfa,0xfa,
				0xfa,0xfa,0xfa,0xfa,0xfa,
				0xfa,0xfa,0xfa,0xfa,0xfa,
				0xfa,0xfa,0xfa,0xfa,0x00
				};
int		fd;

unsigned char		rxflg= 0;

/***************************************************************************
*
*
*
***************************************************************************/
void	serRead(void);
void	enqComm(void);
void	stxComm(void);
void	txFlush(unsigned int num);
void	startFa(void);

/***************************************************************************
* Function Name:
* Description  :
* Arguments    : Code
* Return Value : None
****************************************************************************/
int	main(void){

	struct  termios ttyparam;

	fd= serialOpen("/dev/ttyAMA0",19200);

	if(fd< 0){
		printf("can not open serialport");
	}

	tcgetattr(fd, &ttyparam);
        ttyparam.c_cflag |= PARENB;      //Enable Parity -even
        tcsetattr(fd, TCSANOW,&ttyparam);

	if(wiringPiSetupGpio() < 0){
		printf("ERROR:WiringPi initialize");
	}
	pinMode(GPIO18,OUTPUT);
	digitalWrite(GPIO18, 0);

	startFa();

	while(1){


		serRead();
		enqComm();
		stxComm();


#if 0
		while(serialDataAvail(fd)){
			printf("recive : %C",serialGetchar(fd));
			fflush(stdout);	//
		}

		serialPuts(fd,"hello world\n");
		delay(1000);
#endif
	}
}
/***************************************************************************
* Function Name:
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

	if(rxflg=='?'){

		//printf("rxflg");	//test
		//fflush(stdout);		//test

		rxflg= 0;

		if(data[1]==0x30 || data[1]==0x31 /*&& data[2]==0x30*/){

			digitalWrite(GPIO18, 1);
			delay(5);
			sbuff[0]= 0x04;
			sbuff[1]= 0x00;
			serialPuts(fd,sbuff);
			delay(5);
			digitalWrite(GPIO18, 0);
			puts("?");
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



