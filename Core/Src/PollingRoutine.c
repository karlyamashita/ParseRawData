/*
 * PollingRoutine.c
 *
 *  Created on: Oct 24, 2023
 *      Author: karl.yamashita
 *
 *
 *      Template for projects.
 *
 */


#include "main.h"

extern UART_HandleTypeDef hlpuart1;
extern UartBufferStruct uart1_msg;

void PollingInit(void)
{
	UART_EnableRxInterrupt(&uart1_msg);

	NotifyUser(&uart1_msg, "STM32 Ready", true);
}

void PollingRoutine(void)
{
	UART_Parse(&uart1_msg);
	UART_TxMessage_IT(&uart1_msg);
	UART_CheckRxIntError(&uart1_msg);
}


void UART_Parse(UartBufferStruct *msg)
{
	char *ptr;
	char *token;
	char *token2;
	char *token3;
	char *token4;
	char *rest;
	char delim[] = ";,\n";
	static int package_nr = 0;
	char str[UART_TX_BYTE_BUFFER_SIZE] = {0};

	if(UART_RxMessagePending(msg))
	{
		ptr = (char*)msg->rx.msgToParse->data;

		if(strncmp(ptr, "e", strlen("e")) == 0)
		{
			NotifyUser(&uart1_msg, "Response begin", true);
		}
		else if(strncmp(ptr, "M", strlen("M")) == 0)
		{
			NotifyUser(&uart1_msg, "Measuring...", true);
		}
		else if(strncmp(ptr, "P", strlen("P")) == 0)
		{
			rest = ptr;
			token = strtok_r(rest, delim, &rest);
			token2 = strtok_r(rest, delim, &rest);
			token3 = strtok_r(rest, delim, &rest);
			token4 = strtok_r(rest, delim, &rest);

			if (package_nr++ == 0)
			{
				NotifyUser(&uart1_msg, "Receiving measurement responses:", true);
			}
			sprintf(str, "%d. %s %s %s %s", package_nr, token, token2, token3, token4);
			NotifyUser(&uart1_msg, str, true);
		}
		else if(strncmp(ptr, "*", strlen("*")) == 0)
		{
			NotifyUser(&uart1_msg, "Measurement completed.", true);
			sprintf(str, "%d data point(s) received.", package_nr);
			NotifyUser(&uart1_msg, str, true);
			package_nr = 0;
		}
		else
		{

		}
	}
}


