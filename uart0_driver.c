#include <lpc21xx.h>
#include "header.h"
#define THRE ((U0LSR>>5)&1)
#define RDR ((U0LSR)&1)

// UART0 INITIALISATION //
void uart0_init(unsigned int baud)
{
	int a[]={15,60,30,0,15};
	unsigned int pclk=a[VPBDIV]*1000000; // set pclk
	unsigned int res=0;
	res=pclk/(16*baud);
	PINSEL0|=0x5; // set p0.0=tx and p0.1=rx
	U0LCR=0x83; // unlock baudrate settings
	U0DLL=res&0xFF; //set value for baud rate (LSB)
	U0DLM=(res>>8)&0xFF; // set value for baud rate (MSB)
	U0LCR=0x03; // lock baudrate settings
}

// UART0 TRANSMIT DATA //
void uart0_tx (unsigned char data)
{
	U0THR=data; // tranfer data to U0THR
	while(THRE==0); // wait till data transmit and U0THR is empty.
}

// UART0 STRING TRANSMIT //
void uart0_tx_string (s8*ptr)
{
	while (*ptr)
		{
			uart0_tx(*ptr);
			ptr++;
		}
}

//UART0 RECEIVE DATA //
u8 uart0_rx (void)
{
	while (RDR==0);
	return U0RBR;
}

// UART0 GIVE ASCII //
void uart0_ascii (u8 temp)
{
	uart0_tx((temp/100)+48);
	uart0_tx(((temp/10)%10)+48);
	uart0_tx((temp%10)+48);
}

// UART0 GIVE HEX //
void uart0_hex (u8 temp)
{
		u8 h;
		uart0_tx_string("0x");
		//temp=temp-48;
		h=((temp/(16*16))+48);
		if (h>57)
			h=h+7;
		uart0_tx(h);
		h=(((temp/16)%16)+48);
		if (h>57)
			h=h+7;
		uart0_tx(h);
		h=((temp%16)+48);
		if (h>57)
			h=h+7;
		uart0_tx(h);
}

// UART0 INTEGER TRANSMIT //
void uart0_tx_int (s32 num)
{
	s32 a[100],i=0;
	if (num==0)
	{
		uart0_tx('0');
		return;
	}
	if (num<0)
	{
		uart0_tx('-');
		num=-num;
	}
	while (num>0)
	{
		a[i]=num%10;
		num=num/10;
		i++;
	}
	for (--i ; i>=0 ; i--)
	{
		uart0_tx(a[i]+48);
	}
}

// UART0 FLOAT TRANSMIT//
void uart0_tx_float (float f)
{
	if (f==0)
	{
		uart0_tx('0');
		return;
	}
	if (f<0)
	{
		uart0_tx('-');
		f=-f;
	}
	if (f>0)
	{
		signed int k=0,j,l;
		j=f;
		uart0_tx_int(j);
		uart0_tx ('.');
		for (k=0; k<5 ; k++)
		{
			f=f*10;
			j=f;
			l=j%10;
			uart0_tx_int(l);
		}
	}
}

// UART0 STRING RECEIVE //
void uart0_rx_string (s8*ptr,s32 max)
{
	s32 i;
	s8 arr[100];
	for (i=0 ; i<max ; i++)
	{
		ptr[i]=uart0_rx();
		arr[i]=ptr[i];
		uart0_tx (ptr[i]);
	}
	arr[i]=0;
	uart0_tx_string(arr);
}
