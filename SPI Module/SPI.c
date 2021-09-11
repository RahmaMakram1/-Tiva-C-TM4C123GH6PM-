
#include "SPI.h"
uint32_t Recieved;
void init_SPI(void);

void Send_Data (uint32_t Data);
void Master_init(void);
void GPIO_SPI_init(void);
void Slave_init(void);
uint32_t Recieve_Data(void);

void GPIO_SPI_init(void){
SYSCTL->RCGCSSI |= (1<<1);
	SYSCTL->RCGCGPIO |= (1<<3);
	SYSCTL->RCGCGPIO |= (1<<5);
	GPIOD->AFSEL |= (1<<0)|(1<<3);
	GPIOD->PCTL &= ~0x0000F00F;
	GPIOD ->PCTL |= 0x00002002;
	GPIOD->DEN |= (1<<0)|(1<<3);
}
     
void Master_init (void){
	
SSI1->CR1 =0x0000;    // clear SSE before any operation and set SPI as Master
SSI1 -> CC =0; //Enable System clock
SSI1->CPSR = Prescale; //prescale 4 16MHz/4 =4MHZ
SSI1 ->CR0 |=   Data_Size_select | (SCR<<12) |(SPO <<6) |(SPH<<7); // Data is 16 bits ,SCR=0   ,Data is captured on the first clock edge transition, A steady state Low value is placed on the SSInClk pin
SSI0->CR1= (1<<1); // Enable SPI
}



void Slave_init(void){
SSI1->CR1 =0x0002;    // clear SSE before any operation and set SPI as Slave
SSI1 -> CC =0; //Enable System clock
SSI1->CPSR = Prescale; //prescale 4 16MHz/4 =4MHZ
SSI1 ->CR0 = (SSI1->CR0 & 0x0FF0) | Data_Size_select | (SCR<<12) |(SPO <<6) |(SPH<<7); // Data is 8 bits ,SCR=0   ,Data is captured on the first clock edge transition, A steady state Low value is placed on the SSInClk pin
SSI0->CR1= (1<<1); // Enable SPI
}
 
void Send_Data (uint32_t Data){
	
	while (!(SSI1->SR &(1<<1)));
	SSI1->DR = Data;
	while (!(SSI1->CR1 & (1<<4)));
	}
uint32_t Recieve_Data(){

while (SSI1->SR &(1<<3));

Recieved= SSI1->DR ;
	return Recieved;
}
