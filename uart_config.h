#pragma once
#include <iostream>
#include <termios.h> 
#include <unistd.h>
//#include <string.h>
//#include <stdlib.h>
#include <errno.h>
//#include <stdint.h>
#include <fcntl.h>



class UartConfig
{
public:
	//UartConfig(char* dev_name, int speed, int databits, int stopbits, char parity);
	UartConfig(char* dev_name, int speed);
	int UartConfigOpen(bool block_flag, int vtime, int vmin);
	bool UartConfigSetSpeed(int Baud);
private:
	bool UartConfigSetParity(int databits, int stopbits, char parity);
	~UartConfig();
	char* dev_name;
	int speed;
	int databits;
	int stopbits;
	char parity;
	int fd_uart;
	int speed_arr[15];
	int speed_int[15];
	bool block_flag;
	int vtime;
	int vmin;
	termios *pterm;
};