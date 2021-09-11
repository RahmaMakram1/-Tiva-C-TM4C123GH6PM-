#include "TM4C123.h"
#include <stdint.h>
void Send_Data (uint32_t Data);
void Master_init(void);
void GPIO_SPI_init(void);
void Slave_init(void);
uint32_t Recieve_Data(void);

#define Data_Size_select 0x000F
#define SCR 0
#define SPO 0
#define SPH 0
#define Prescale 4
