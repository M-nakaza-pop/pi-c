#include	<stdio.h>
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
#define		PORT	27

// gcc -Wall -o rsens rsens.c -lwiringPi
// sudo ./rsens  --> run 
//
/***************************************************************************
* Function Name: 															
* Description  : 
* Arguments    : Code
* Return Value : None
****************************************************************************/
int	main(void){
	int	data;
	int	datap;
	
	if(wiringPiSetup() == -1){
		printf("No wiringPi.\n");
		exit(1);					//Setup Err
	}
	
	printf("ok wiringPi.\n");
	pinMode(PORT,INPUT);
	datap = digitalRead(PORT);
	
	for(;;){
		
		data = digitalRead(PORT);
		
		if(datap != data){
			printf("DATA = [%d]\n",data);
			datap = data;
		}
	}
}