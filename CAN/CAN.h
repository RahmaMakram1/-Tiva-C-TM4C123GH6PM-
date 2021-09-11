
#include "common.h"
#include <TM4C123.h>
#include <stdint.h>




#define CAN_Base CAN0
#define CAN_Mode Real_Mode
#define Baud_rate_extention 0
#define CAN_STANDARD_ID		0x7FF             // 11 bits
#define	CAN_EXTENDED_ID		0x1FFFFFFF           // 29 bits
#define CAN_DATA_LENGTH		8
#define frame Standard
extern uint8_t Data[8];
extern uint8_t Data2[8];

typedef struct {uint32_t TSEG2;
	      uint32_t TSEG1;
	      uint32_t Sjw;
	      uint32_t BRP ;

}Can_bits;


typedef struct {uint32_t msgid;
	uint32_t msglength;
	uint8_t *Data;   // Data array of bytes
}msgobjct;


//Prototypes 
void CAN_GPIO(void);
void  CAN_INIT(void);

void Enable_CAN (void);
void CAN_Set_Transmit_object(uint32_t msg_obj_id,msgobjct*message);
void CAN_Transmit(uint32_t msg_obj_id,msgobjct*message);
void CAN_Set_Recieve_object ( uint32_t msg_obj_id,msgobjct*message_Recieve);
void  CAN_Recieve ( uint32_t msg_obj_id,msgobjct*message_Recieve);
void  CAN_BIT_TIMING(Can_bits*Bits);
void Disable_CAN (void);
