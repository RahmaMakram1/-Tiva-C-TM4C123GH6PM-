#include <TM4C123.h>
#include <stdint.h>
#define SLAVE_ADDRESS 0x9

void I2C_Init(void);
void I2C_Master_write(uint32_t DATA);
void Stop_condition(void);
uint32_t I2C_Read(void);