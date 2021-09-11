#include <stdint.h>
#include "TM4C123.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
void delay_ms(int time);
void UART_init (void);
void UART0_Transmitter(unsigned char x);
void printstring(char *str);
void UART0_Handler (void){
unsigned char x=0;
UART0->ICR &= ~(0x010); // Clear receive interrupt
x = UART0 ->DR ; // get the received data byte
	if(islower(x)){
		toupper(x);
	}
	
 UART0_Transmitter(x);
 }
void delay_ms(int time){
for (int i=0; i<time ; i++){

for (int j=0 ; j<7900; j++);
}
}
void UART_init (void){
SYSCTL->RCGCUART |= (1<<0);  
SYSCTL->RCGCGPIO |= 0x01;  
UART0->CTL=0;
UART0->IBRD = 104; /*9600=(16MHZ /16 x baud divisor) , baud divisor = 104.1667 , integer value will be stored in IBRD */
UART0->FBRD =11; /* 0.166 x 64 +0.5 =11 , thr fractional will be stored in FBRD */

UART0->CTL |= (1<<0)|(1<<8)|(1<<9);
UART0->LCRH=0;
UART0->LCRH |= (1<<5)|(1<<6);
UART0->CTL |= (1<<0)|(1<<8)|(1<<9);
 
GPIOA->DEN |=0x03;
GPIOA->AFSEL |=0x03;
GPIOA->PCTL|= 0x00000011;

UART5->ICR &= ~(0x0780);
	UART0->IM |=(1<<4);
NVIC->ISER[1] |= 0x20000000;
	
}
 
int main (){
	UART_init();
	printf("Enter a character");
	delay_ms(5);
	while(1){}
}
void UART0_Transmitter(unsigned char x  )
{
    while((UART5->FR & (1<<5)) != 0); /* wait until Tx buffer not full */
    UART5->DR = x;                  /* before giving it another byte */
}
void printstring(char *str)
{
  while(*str)
	{
		UART0_Transmitter(*(str++));
	}
}
