#include "uart_config.h"

using namespace std;

/*
UartConfig::UartConfig(char* dev_name, int speed, int databits, int stopbits, char parity)
{
	for (int i = 0; i < 15; i++)
	{
		switch (i)
		{
		case 0:
			speed_arr[i] = B1200;
			speed_int[i] = 1200;
			break;
		case 1:
			speed_arr[i] = B1800;
			speed_int[i] = 1800;
			break;
		case 2:
			speed_arr[i] = B2400;
			speed_int[i] = 2400;
			break;
		case 3:
			speed_arr[i] = B4800;
			speed_int[i] = 4800;
			break;
		case 4:
			speed_arr[i] = B9600;
			speed_int[i] = 9600;
			break;
		case 5:
			speed_arr[i] = B19200;
			speed_int[i] = 19200;
			break;
		case 6:
			speed_arr[i] = B38400;
			speed_int[i] = 38400;
			break;
		case 7:
			speed_arr[i] = B57600;
			speed_int[i] = 57600;
			break;
		case 8:
			speed_arr[i] = B115200;
			speed_int[i] = 115200;
			break;
		case 9:
			speed_arr[i] = B230400;
			speed_int[i] = 230400;
			break;
		case 10:
			speed_arr[i] = B460800;
			speed_int[i] = 460800;
			break;
		case 11:
			speed_arr[i] = B500000;
			speed_int[i] = 500000;
			break;
		case 12:
			speed_arr[i] = B576000;
			speed_int[i] = 576000;
			break;
		case 13:
			speed_arr[i] = B921600;
			speed_int[i] = 921600;
			break;
		case 14:
			speed_arr[i] = B1000000;
			speed_int[i] = 1000000;
			break;
		}
	}
	dev_name = dev_name;
	speed = speed;
	databits = databits;
	parity = parity;
	stopbits = stopbits;
	fd_uart = -1;
}
*/
UartConfig::UartConfig(char* dev_name, int speed)
{
	for (int i = 0; i < 15; i++)
	{
		switch (i)
		{
		case 0:
			speed_arr[i] = B1200;
			speed_int[i] = 1200;
			break;
		case 1:
			speed_arr[i] = B1800;
			speed_int[i] = 1800;
			break;
		case 2:
			speed_arr[i] = B2400;
			speed_int[i] = 2400;
			break;
		case 3:
			speed_arr[i] = B4800;
			speed_int[i] = 4800;
			break;
		case 4:
			speed_arr[i] = B9600;
			speed_int[i] = 9600;
			break;
		case 5:
			speed_arr[i] = B19200;
			speed_int[i] = 19200;
			break;
		case 6:
			speed_arr[i] = B38400;
			speed_int[i] = 38400;
			break;
		case 7:
			speed_arr[i] = B57600;
			speed_int[i] = 57600;
			break;
		case 8:
			speed_arr[i] = B115200;
			speed_int[i] = 115200;
			break;
		case 9:
			speed_arr[i] = B230400;
			speed_int[i] = 230400;
			break;
		case 10:
			speed_arr[i] = B460800;
			speed_int[i] = 460800;
			break;
		case 11:
			speed_arr[i] = B500000;
			speed_int[i] = 500000;
			break;
		case 12:
			speed_arr[i] = B576000;
			speed_int[i] = 576000;
			break;
		case 13:
			speed_arr[i] = B921600;
			speed_int[i] = 921600;
			break;
		case 14:
			speed_arr[i] = B1000000;
			speed_int[i] = 1000000;
			break;
		default:
			std::cout << "Baud set error !" << std::endl;
			break;
		}
	}
	dev_name = dev_name;
	speed = speed;
	databits = 8;
	parity = 'N';
	stopbits = 1;
	fd_uart = -1;
	block_flag = false;
	vtime = 0;
	vmin = 0;
	pterm = new termios;
}

bool UartConfig::UartConfigSetSpeed(int Baud)
{
	if (-1 == tcgetattr(fd_uart, pterm))
	{
		cout << "tcgetattr failed !" << endl;
		return false;
	}
	cfmakeraw(pterm);
	for (int i = 0; i < 15; i++)
	{
		if (Baud == speed_int[i])
		{
			tcflush(fd_uart, TCIOFLUSH);
			if (-1 == cfsetspeed(pterm, speed_arr[i]))
			{
				cout << "cfsetspeed failed !" << endl;
				return false;
			}
			if (-1 == tcsetattr(fd_uart, TCSANOW, pterm))
			{
				cout << "tcsetattr failed !" << endl;
				return false;
			}
			tcflush(fd_uart, TCIOFLUSH);
			return true;
		}
	}
	cout << "Baud set error ! support Baud : ";
	for (int i = 0; i < 15; i++)
	{
		cout << speed_int[i] << ",";
	}
	cout << endl;
	return false;
}

bool UartConfig::UartConfigSetParity(int databits, int stopbits, char parity)
{
	if (-1 == tcgetattr(fd_uart, pterm))
	{
		cout << "tcgetattr failed !" << endl;
		return false;
	}
	pterm->c_cflag &= ~CSIZE;
	pterm->c_oflag = 0;
	pterm->c_lflag &= ~(ECHO|ICANON);
	tcflush(fd_uart, TCIOFLUSH);
	switch (databits)
	{
	case 7:
		pterm->c_cflag |= CS7;
		break;
	case 8:
		pterm->c_cflag |= CS8;
		break;
	default:
		cout << "databits set error ! only allow 7 or 8" << endl;
		break;
	}
	switch (parity)
	{
	case 'n':
	case 'N':
		pterm->c_cflag &= ~PARENB;
		pterm->c_iflag &= ~INPCK;
		break;
	case 'o':
	case 'O':
		pterm->c_cflag |= (PARODD | PARENB);
		pterm->c_iflag |= INPCK;
		break;
	case 'e':
	case 'E':
		pterm->c_cflag |= PARENB;
		pterm->c_cflag &= ~PARODD;
		pterm->c_iflag |= INPCK;
		break;
	case 's':
	case 'S':
		pterm->c_cflag &= ~PARENB;
		pterm->c_cflag &= ~CSTOPB;
		break;
	default:
		cout << "parity set error ! only allow N E O S" << endl;
		return false;
	}
	switch (stopbits)
	{
	case 1:
		pterm->c_cflag &= ~CSTOPB;
		break;
	case 2:
		pterm->c_cflag |= CSTOPB;
		break;
	default:
		cout << "stopbits set error ! only allow 1 or 2" << endl;
		break;
	}
	if (!block_flag)
	{
		pterm->c_cc[VTIME] = vtime;
		pterm->c_cc[VMIN] = vmin;
	}
	else
	{
		pterm->c_cc[VTIME] = 0;
		pterm->c_cc[VMIN] = 0;
	}
	if (-1 == tcsetattr(fd_uart, TCSANOW, pterm))
	{
		cout << "parity tcsetattr failed !" << endl;
		return false;
	}
	tcflush(fd_uart, TCIOFLUSH);
	return true;
}

int UartConfig::UartConfigOpen(bool block_flag, int vtime, int vmin)
{
	block_flag = block_flag;
	vtime = vtime;
	vmin = vmin;
	if (!block_flag)
		fd_uart = open(dev_name, O_RDWR | O_NOCTTY | O_NDELAY);
	else
		fd_uart = open(dev_name, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);
	if (-1 == fd_uart)
	{
		std::cout << "Uart Open failed !" << std::endl;
	}
	else
	{
		if (!UartConfigSetSpeed(speed))
		{
			cout << "UartConfigSetSpeed failed !" << endl;
			return -1;
		}
		if (!UartConfigSetParity(8, 1, 'N'))
		{
			cout << "UartConfigSetParity failed !" << endl;
			return -1;
		}
	}
	return fd_uart;
}

UartConfig::~UartConfig()
{
	close(fd_uart);
	delete pterm;
}