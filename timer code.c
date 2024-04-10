#include<lpc21xx.h>
#include"header.h"
void delay_ms (unsigned int ms, unsigned int vpbdiv)
{
	int a[]={60,15,30,0,15};
	unsigned int pclk=a[vpbdiv]*1000;
	T0PC=0;
	T0PR=pclk-1;
	T0TC=0;
	T0TCR=1;
	while(T0TC<ms);
	T0TCR=0;
}

void delay_sec (unsigned int sec, unsigned int vpbdiv)
{
	int a[]={60,15,30,0,15};
	unsigned int pclk=a[vpbdiv]*1000000;
	T0PC=0;
	T0PR=pclk-1;
	T0TC=0;
	T0TCR=1;
	while(T0TC<sec);
	T0TCR=0;
}


