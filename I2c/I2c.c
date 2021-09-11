
#include "I2c.h"
uint32_t Data;

void I2C_Init(void){
SYSCTL->RCGCI2C |= (1<<3);
SYSCTL -> RCGCGPIO |= (1<<3);
GPIOD ->AFSEL |=	(1<<0)|(1<<1);
GPIOD ->DEN |= (1<<1) |(1<<0);
GPIOD ->ODR |= (1<<1);
GPIOD ->PCTL = 0x00000033;
I2C3 -> SOAR = 0x7;   // My address if any Master want to call me 
I2C3->MTPR  = 0x07 ;
	/* Configure I2C 3 clock frequency
(1 + TIME_PERIOD ) = SYS_CLK /(2*
( SCL_LP + SCL_HP ) * I2C_CLK_Freq )
TIME_PERIOD = 16 ,000 ,000/(2(6+4) *100000) - 1 = 7 */
}


void I2C_Master_write(uint32_t DATA)
{ I2C3 -> MCR |= (1<<4); //Master Mode
	
 I2C3 -> MSA = (SLAVE_ADDRESS <<1);
 I2C3 -> MCS |=(1<<1)|(1<<0); //START condition followed by Transmit(master goes to the Master Transmit state)
 I2C3->MDR =DATA;
 I2C3 ->MCS &= ~(1<<1);
 I2C3 ->MCS |=(1<<0)|(1<<3);
	/*RECEIVE operation (master goes to Master Receive state to get ack from slave ) */
	while (I2C3->MCS &(1<<3)); //polling until master get ack from slave
}
void Stop_condition(){
	I2C3->MCS |=(1<<2); //master goes to Idle state
}
uint32_t I2C_Read(){
	I2C3 -> MCR |= (1<<4); //Master Mode
	I2C3 -> MCS |=(1<<1)|(1<<0)|(1<<3); //Start condition followed by Recieve
	I2C3 -> MSA = (SLAVE_ADDRESS <<1)|(1<<0); // slave address and Read Mode
	
	while (I2C3 ->MCS &(1<<5));//polling till I2C is not Busy 
	Data=I2C3->MDR;
	return Data;
}
