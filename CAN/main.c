
#include "CAN.h"



uint8_t Data[8] = 	{0x11, 0x22, 0x33, 0x44, 0x44, 0x44 , 0x12,0x23};
uint8_t Data2[8] = 	{0x12, 0x12, 0x23, 0x44, 0x44, 0x54 , 0x12,0x23};
int main(void) {

	Can_bits BitTime = {5, 2, 2, 4};
	
	CAN_INIT();       // initialize CAN
	
	CAN_BIT_TIMING(&BitTime);        //initialize clock
	  
	  Enable_CAN ();
	
	/*Transmite Data */
	
	 msgobjct msg_obj_T = {0x5, 8,Data};   
	 
   	 /*message 1
       MsgID = 0x5, 
  	   datal length =8
 		   Data is transmitted data */
	 
	  CAN_Set_Transmit_object(0x01,&msg_obj_T); 
	 
	 /*message 2 */
	 
	 msgobjct msg_obj_T2 ={0x8 , 8 , Data2};
	 
	 CAN_Set_Transmit_object(0x02,&msg_obj_T2);
	 
	 msgobjct msg_obj_R={0x01 ,8,0 };
	
	 CAN_Set_Recieve_object (0x03,&msg_obj_R);
		
	 msgobjct msg_obj_R2={0x02 ,8,0 };
	 
	CAN_Set_Recieve_object (0x04,&msg_obj_R2);        // Recieve message2
		
		// Enable_CAN ();
	 
	 
	
	while (1){
		
		 CAN_Transmit(0x01,&msg_obj_T);
		for(int i = 0; i<100000000; i++){}
		CAN_Transmit(0x02,&msg_obj_T2);
		for(int i = 0; i<100000000; i++){}
	}
}